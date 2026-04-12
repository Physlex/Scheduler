/**
 * @brief This file defines the entry point to the gbox clang wrapper.
 */

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
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TargetParser/Host.h>
#include <stdint.h>
#include <unistd.h>

#include <memory>
#include <vector>

#include "gbox/macros/action.hpp"
#include "gbox/macros/action/plugins.hpp"
#include "gbox/macros/cli.hpp"

/**
 * @brief Entry point to the program.
 */
int32_t main(int argc, const char **argv) {
    // All of our clang wrappers will be "gb" + <name>, so skip the "gb"
    std::string interrupted_path = gbox::cli::stripGBFromPath(argv[0]);
    std::string abs_clang_path = gbox::cli::clangPathFromName(interrupted_path);
    auto driver_args = llvm::SmallVector<const char *, 256>(argv, argv + argc);

    std::unique_ptr<clang::DiagnosticOptions> diag_opts =
        clang::CreateAndPopulateDiagOpts(driver_args);
    diag_opts->DiagnosticSuppressionMappingsFile.clear();

    auto *diag_client = new clang::TextDiagnosticPrinter(llvm::errs(), *diag_opts);
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diag_id(new clang::DiagnosticIDs());
    llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag(
        new clang::DiagnosticsEngine(diag_id, *diag_opts, diag_client)
    );

    auto driver =
        clang::driver::Driver(abs_clang_path, llvm::sys::getDefaultTargetTriple(), *diag);

    auto target_and_mode =
        clang::driver::ToolChain::getTargetAndModeFromProgramName(argv[0]);

    driver.setTargetAndMode(target_and_mode);

    std::unique_ptr<clang::driver::Compilation> compilation(
        driver.BuildCompilation(driver_args)
    );
    if (!compilation || diag->hasErrorOccurred()) {
        return 1;
    }

    gbox::cli::fixupDiagPrefixExeName(diag_client, abs_clang_path);

    std::vector<std::string> true_list;
    for (const auto &job : compilation->getJobs()) {
        const auto *cmd = llvm::dyn_cast<clang::driver::Command>(&job);
        if (!cmd) continue;

        const llvm::opt::ArgStringList &args = cmd->getArguments();
        if (!llvm::is_contained(args, llvm::StringRef("-emit-obj"))) continue;

        const char *const *begin = args.data() + 1;
        const char *const *end = begin + args.size() - 1;
        auto args_slice = std::vector<const char *>(begin, end);

        auto action = gbox::action::Action(*diag);
        action.execute(args_slice);
    }

    llvm::SmallVector<llvm::StringRef> args(argv, argv + argc);
    return llvm::sys::ExecuteAndWait(abs_clang_path, args);
}
