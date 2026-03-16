#ifndef GBOX_MACROS_PLUGINS_HPP_
#define GBOX_MACROS_PLUGINS_HPP_

/** @file `plugins.hpp`
 *  @brief TODO: DOCS
 */

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendPluginRegistry.h>

#include <memory>
#include <vector>

/// Boilerplate to handle clang
class HandleFuncDecl : public clang::ast_matchers::MatchFinder::MatchCallback {
  public:
    //! @brief Ru
    void run(const clang::ast_matchers::MatchFinder::MatchResult &res) override;
};

/**
 *  @brief This method implements the "frontend" logic for parsing each t-unit
 */
class ProcMacroConsumer : public clang::ASTConsumer {
  public:
    /// Override to handle each translation unit according to the consumer.
    void HandleTranslationUnit(clang::ASTContext &ctx) override;
};

/// This class really just exists as the hook-up boilerplate for the sake of the clang
/// compilation process.
class ProcMacroPlugin : public clang::PluginASTAction {
  protected:
    inline std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &_ci, llvm::StringRef _in_file
    ) override {
        return std::make_unique<ProcMacroConsumer>();
    }

    inline bool ParseArgs(
        const clang::CompilerInstance &_ci, const std::vector<std::string> &_args
    ) override {
        return true;
    }

    ActionType getActionType() override { return AddAfterMainAction; }
};

#endif  // GBOX_MACROS_PLUGINS_HPP_
