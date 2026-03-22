/**
 * @brief This file defines the entry point to the gbox clang wrapper.
 */

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <stdint.h>

#include "gbox/macros/plugins.hpp"

static llvm::cl::OptionCategory GBoxCategory("gbclang options");

int32_t main(int argc, const char **argv) {
    auto expected_parser =
        clang::tooling::CommonOptionsParser::create(argc, argv, GBoxCategory);

    if (!expected_parser) {
        llvm::errs() << expected_parser.takeError();
        return 1;
    }

    clang::tooling::CommonOptionsParser &options = expected_parser.get();

    clang::tooling::ClangTool proc_macro_tool(
        options.getCompilations(), options.getSourcePathList()
    );

    auto proc_macro_action =
        clang::tooling::newFrontendActionFactory<gbox::ProcMacroAction>();

    if (proc_macro_tool.run(proc_macro_action.get()) != 0) {
        llvm::errs() << "gbclang: macro expansion failed\n";
        return 1;
    }

    // TODO: What do I put here??

    return 0;
}
