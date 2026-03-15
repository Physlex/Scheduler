#ifndef GBOX_MACROS_PLUGINS_HPP_
#define GBOX_MACROS_PLUGINS_HPP_

/** @file `plugins.hpp`
 *  @brief TODO: DOCS
 */

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/Basic/IdentifierTable.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Lex/Token.h>

#include <memory>
#include <vector>

/**
 *  @brief TODO: DOCS
 */
class HandleFuncDecl : public clang::ast_matchers::MatchFinder::MatchCallback {
   public:
    //! @brief TODO: DOCS
    void run(const clang::ast_matchers::MatchFinder::MatchResult& res) override;
};

/**
 *  @brief TODO: DOCS
 */
class ProcMacroConsumer : public clang::ASTConsumer {
   public:
    /// Override to handle each translation unit according to the consumer.
    void HandleTranslationUnit(clang::ASTContext& ctx) override;
};

/// This class really just exists as the hook-up boilerplate for the sake of the clang
/// compilation process.
class ProcMacroPlugin : public clang::PluginASTAction {
   protected:
    inline std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& _ci,
        llvm::StringRef _in_file
    ) override {
        return std::make_unique<ProcMacroConsumer>();
    }

    inline bool ParseArgs(
        const clang::CompilerInstance& ci,
        const std::vector<std::string>& args
    ) override {
        return true;
    }

    ActionType getActionType() override { return AddAfterMainAction; }
};

#endif  // GBOX_MACROS_PLUGINS_HPP_
