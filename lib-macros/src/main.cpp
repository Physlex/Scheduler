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

#include <memory>

#include "gbox/macros/plugins.hpp"
#include "llvm/ADT/SmallVector.h"

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

}  // namespace cl
}  // namespace gbox

int32_t main(int argc, const char **argv) {
    // All of our clang wrappers will be "gb" + <name>, so skip the "gb"
    std::string interrupted_path = gbox::cl::stripGBFromPath(argv[0]);
    std::string abs_clang_path = gbox::cl::clangPathFromName(interrupted_path);
    llvm::outs() << abs_clang_path << "\n";
    auto driver_args = llvm::SmallVector<const char *, 256>(argv, argv + argc);

    std::unique_ptr<clang::DiagnosticOptions> diag_opts =
        clang::CreateAndPopulateDiagOpts(driver_args);
    diag_opts->DiagnosticSuppressionMappingsFile.clear();

    auto *diag_client = new clang::TextDiagnosticPrinter(llvm::errs(), *diag_opts);
    gbox::cl::fixupDiagPrefixExeName(diag_client, abs_clang_path);
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diag_id(new clang::DiagnosticIDs());
    auto diag = clang::DiagnosticsEngine(diag_id, *diag_opts, diag_client);
    auto driver =
        clang::driver::Driver(abs_clang_path, llvm::sys::getDefaultTargetTriple(), diag);

    auto target_and_mode =
        clang::driver::ToolChain::getTargetAndModeFromProgramName(argv[0]);
    driver.setTargetAndMode(target_and_mode);
    std::unique_ptr<clang::driver::Compilation> compilation(
        driver.BuildCompilation(driver_args)
    );

    // clang::tooling::CommonOptionsParser &options = expected_parser.get();
    // clang::tooling::ClangTool proc_macro_tool(
    //     options.getCompilations(), options.getSourcePathList()
    // );

    // auto proc_macro_action =
    //     clang::tooling::newFrontendActionFactory<gbox::ProcMacroAction>();

    // if (proc_macro_tool.run(proc_macro_action.get()) != 0) {
    //     llvm::errs() << "gbclang: macro expansion failed\n";
    //     return 1;
    // }

    // TODO: What do I put here??

    return 0;
}
