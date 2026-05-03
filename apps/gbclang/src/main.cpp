//! TODO: Comprehensive docs on usage, invocation, and design process.

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TargetParser/Host.h>
#include <stdint.h>
#include <unistd.h>

#include <cstdlib>
#include <memory>
#include <vector>

#include "gbclang/cli.hpp"
#include "gbox/core/result.hpp"
#include "gbox/proc/action.hpp"

using namespace gbox;
using namespace gbclang;
using namespace gbox::result;

/// Typed response for the `configure_diag` method.
struct ConfigureDiagRes {
    /// Fully configured diagnostic engine
    llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag;

    /// Path to be used by the clang driver, stripped of gb influence
    std::string path;
};

/// Configure the clang diagnostics system to gracefully handle gbclang invocations
Result<ConfigureDiagRes, int> build_diag(std::vector<const char *> &args) {
    auto diag_opts = clang::CreateAndPopulateDiagOpts(args);

    auto diag_client =
        std::make_unique<clang::TextDiagnosticPrinter>(llvm::errs(), *diag_opts);

    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diag_id(new clang::DiagnosticIDs());

    llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag(
        new clang::DiagnosticsEngine(diag_id, *diag_opts, diag_client.get())
    );

    auto strip_res = cli::stripGBFromPath(args[0]);
    if (strip_res.is_err()) {
        return Err(1);
    }

    auto stripped_pathname = strip_res.unwrap();

    auto clean_res = cli::clangPathFromName(stripped_pathname);
    if (clean_res.is_err()) {
        return Err(1);
    }

    auto clean_pathname = clean_res.unwrap();

    auto exe_basename = llvm::StringRef(llvm::sys::path::stem(clean_pathname));
    if (exe_basename.equals_insensitive("cl")) {
        exe_basename = "clang-cl";
    }

    diag_client->setPrefix(std::string(exe_basename));

    return Ok(ConfigureDiagRes{diag, clean_pathname});
}

int32_t main(int argc, const char **argv) {
    auto args_vec = std::vector<const char *>(argv, argv + argc);

    auto diag_config_res = build_diag(args_vec);
    if (diag_config_res.is_err()) {
        return diag_config_res.unwrap_err();
    }

    auto [diag, clean_path] = diag_config_res.unwrap();
    auto driver =
        clang::driver::Driver(clean_path, llvm::sys::getDefaultTargetTriple(), *diag);
    auto target_and_mode =
        clang::driver::ToolChain::getTargetAndModeFromProgramName(clean_path);
    driver.setTargetAndMode(target_and_mode);

    auto compilation = driver.BuildCompilation(args_vec);
    if (!compilation || diag->hasErrorOccurred()) {
        return 1;
    }

    for (const auto &job : compilation->getJobs()) {
        const auto *cmd = llvm::dyn_cast<clang::driver::Command>(&job);
        if (!cmd) continue;

        const llvm::opt::ArgStringList &args = cmd->getArguments();
        if (!llvm::is_contained(args, llvm::StringRef("-emit-obj"))) continue;

        const char *const *begin = args.data() + 1;
        const char *const *end = begin + args.size() - 1;
        auto args_slice = std::vector<const char *>(begin, end);

        // TODO: Refactor the string into a map of files rewritten -> rewritten code
        auto action_exec_res = gbox::action::Action(*diag).execute(args_slice);
        std::string rewritten = "";
        gbox::result::match_result(
            std::move(action_exec_res),
            [&](Ok<std::string> &&o) { rewritten = std::move(o.value); },
            [&](Err<gbox::action::ErrorKind> &&e) {
                llvm::errs() << "Failed to execute action";
            }
        );
    }

    llvm::SmallVector<llvm::StringRef> args(argv, argv + argc);
    return llvm::sys::ExecuteAndWait(clean_path, args);
}
