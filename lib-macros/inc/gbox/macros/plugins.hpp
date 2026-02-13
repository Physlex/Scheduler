#ifndef GBOX_MACROS_PLUGINS_HPP_
#define GBOX_MACROS_PLUGINS_HPP_

/** @file `plugins.hpp`
 *  @brief TODO: DOCS
 */

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Attr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/FrontendPluginRegistry.h>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

class FindMyAttributeVisitor
    : public RecursiveASTVisitor<FindMyAttributeVisitor> {
   public:
    FindMyAttributeVisitor(ASTContext& ctx) : ctx(ctx) {}

    // Visit an attribute.
    bool VisitAttr(Attr* attr) {
        attr->printPretty(llvm::outs(), ctx.getPrintingPolicy());
        return true;
    }

   private:
    ASTContext& ctx;
};

class FindMyAttributeConsumer : public ASTConsumer {
   public:
    virtual void HandleTranslationUnit(ASTContext& ctx) {
        FindMyAttributeVisitor visitor(ctx);
        visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    }
};

class FindMyAttributeAction : public ASTFrontendAction {
   public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance& Compiler, StringRef InFile) {
        return std::make_unique<FindMyAttributeConsumer>();
    }
};

struct ExampleAttributeInfo : public ParsedAttrInfo {
    ExampleAttributeInfo() {
        static constexpr Spelling attrib_spellings[3] = {
            {ParsedAttr::AS_CXX11, "runtime"},
            {ParsedAttr::AS_C23, "gbox::runtime"},
            {ParsedAttr::AS_GNU, "runtime"},
        };

        this->Spellings = attrib_spellings;
    }
};

#endif  // GBOX_MACROS_PLUGINS_HPP_
