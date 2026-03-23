/**
 * @brief This file defines the entry point to the gbox clang wrapper.
 */

#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/Program.h>
#include <llvm/TargetParser/Host.h>
#include <stdint.h>
#include <unistd.h>

#include <memory>

#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendAction.h"
#include "gbox/macros/plugins.hpp"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/raw_ostream.h"

namespace gbox {
namespace cl {

std::string stripGBFromPath(std::string path) {
    const auto gb_substr = std::string("gbclang");
    const auto gb_replace = std::string("clang");
    size_t pos = path.find(gb_substr);

    if (pos == std::string::npos) return "";

    const auto clang_path = path.replace(pos, gb_substr.length(), gb_replace);
    return clang_path.substr(pos, gb_replace.length());
}

std::string clangPathFromName(std::string executable_path) {
    if (!llvm::sys::fs::exists(executable_path)) {
        const auto path = llvm::sys::findProgramByName(executable_path);
        if (path) executable_path = *path;
    }

    return std::string(executable_path);
}

static void fixupDiagPrefixExeName(
    clang::TextDiagnosticPrinter *diag_client, const std::string &path
) {
    auto exe_basename = llvm::StringRef(llvm::sys::path::stem(path));
    if (exe_basename.equals_insensitive("cl")) {
        exe_basename = "clang-cl";
    }

    diag_client->setPrefix(std::string(exe_basename));
}

static bool createTempFile(
    clang::CompilerInvocation &invocation, gbox::ProcMacroAction &action
) {
    const std::string &output = invocation.getFrontendOpts().OutputFile;
    auto output_dir = std::string(llvm::sys::path::parent_path(output));

    llvm::SmallString<128> virtual_dir(output_dir);
    llvm::sys::path::append(virtual_dir, "virtual");
    if (auto err = llvm::sys::fs::create_directories(virtual_dir)) {
        return false;
    }

    llvm::SmallString<128> temp_model(virtual_dir);
    llvm::sys::path::append(
        temp_model,
        llvm::sys::path::stem(invocation.getFrontendOpts().Inputs[0].getFile())
    );

    auto tempfile = llvm::sys::fs::TempFile::create(temp_model + "-%%%%%%.c");
    if (!tempfile) {
        llvm::errs() << "failed to create temp file: "
                     << llvm::toString(tempfile.takeError()) << "\n";
        return false;
    }

    llvm::raw_fd_ostream out(tempfile->FD, false);
    auto buf = action.getRewriteBuffer();
    if (!buf) {
        // No buffer rewrite has occured
        return false;
    }

    return true;
}

}  // namespace cl
}  // namespace gbox

int32_t main(int argc, const char **argv) {
    // All of our clang wrappers will be "gb" + <name>, so skip the "gb"
    std::string interrupted_path = gbox::cl::stripGBFromPath(argv[0]);
    std::string abs_clang_path = gbox::cl::clangPathFromName(interrupted_path);
    auto driver_args = llvm::SmallVector<const char *, 256>(argv, argv + argc);

    std::unique_ptr<clang::DiagnosticOptions> diag_opts =
        clang::CreateAndPopulateDiagOpts(driver_args);
    diag_opts->DiagnosticSuppressionMappingsFile.clear();

    auto *diag_client = new clang::TextDiagnosticPrinter(llvm::errs(), *diag_opts);
    gbox::cl::fixupDiagPrefixExeName(diag_client, abs_clang_path);
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

    for (const auto &job : compilation->getJobs()) {
        const auto *cmd = llvm::dyn_cast<clang::driver::Command>(&job);
        if (!cmd) continue;

        const llvm::opt::ArgStringList &args = cmd->getArguments();
        if (!llvm::is_contained(args, llvm::StringRef("-emit-obj"))) continue;

        const auto args_slice =
            llvm::ArrayRef<const char *>(args.data() + 1, args.size() - 1);

        auto invocation = std::make_shared<clang::CompilerInvocation>();
        if (!clang::CompilerInvocation::CreateFromArgs(*invocation, args_slice, *diag)) {
            llvm::errs() << "Failed to create compiler invocation\n";
            return 1;
        }

        auto ci = clang::CompilerInstance(invocation);
        ci.setDiagnostics(diag.get());

        auto action = gbox::ProcMacroAction();
        if (!ci.ExecuteAction(action)) {
            llvm::errs() << "failed to execute proc macro action\n";
            return 1;
        }

        if (ci.getDiagnostics().hasErrorOccurred()) {
            llvm::errs() << "errors occurred during compilation\n";
            return 1;
        }

        gbox::cl::createTempFile(*invocation, action);
    }

    // TODO: What do I put here??

    return 0;
}
