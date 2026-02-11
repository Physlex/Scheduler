#ifndef GBOX_MACROS_PLUGINS_HPP_
#define GBOX_MACROS_PLUGINS_HPP_

/** @file `plugins.hpp`
 *  @brief TODO: DOCS
 */

#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Attr.h>

class FindMyAttributeVisitor : public clang::RecursiveASTVisitor<FindMyAttributeVisitor> {
public:
    explicit FindMyAttributeVisitor(clang::ASTContext &ctx) : ctx(ctx) {}

    bool VisitAttr(clang::Attr *attr) {
        // Print any attribute
        attr->printPretty(llvm::outs(), ctx.getPrintingPolicy());
        llvm::outs() << "\n";
        return true;
    }

private:
    clang::ASTContext &ctx;
};

class FindMyAttributeConsumer : public clang::ASTConsumer {
public:
    explicit FindMyAttributeConsumer(clang::ASTContext &ctx) : visitor(ctx) {}

    void HandleTranslationUnit(clang::ASTContext &ctx) override {
        visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    }

private:
    FindMyAttributeVisitor visitor;
};

class FindMyAttributeAction : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &CI, llvm::StringRef InFile
    ) override {
        return std::make_unique<FindMyAttributeConsumer>(CI.getASTContext());
    }
};

/** @struct GBoxPlugins
 *  @brief TODO: DOCS
 */
class GBoxPlugin : public clang::PluginASTAction {
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &CI, llvm::StringRef InFile
    ) override {
        return std::make_unique<FindMyAttributeConsumer>(CI.getASTContext());
    }

    bool ParseArgs(
        const clang::CompilerInstance &CI, const std::vector<std::string> &arg
    ) override {
        return true;
    }
};

static clang::FrontendPluginRegistry::Add<GBoxPlugin>
    X("gbox-plugin", "Finds [[gbox::]] attributes");

#endif  // GBOX_MACROS_PLUGINS_HPP_
