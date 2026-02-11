/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::attr;
using namespace llvm;

class FindMyAttributeVisitor
    : public RecursiveASTVisitor<FindMyAttributeVisitor> {
  public:
    FindMyAttributeVisitor(ASTContext &ctx): ctx(ctx) {}

    // Visit an attribute.
    bool VisitAttr(Attr *attr) {
        attr->printPretty(llvm::outs(), ctx.getPrintingPolicy());
        return true;
    }

  private:
    ASTContext &ctx;
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

struct ExampleAttributeInfo : public ParsedAttributeInfo {
    ExampleAttributeInfo() {
        static constexpr s[] = {
            {ParsedAttr::AS_C23, "example"},
            {ParsedAttr::AS_CXX11, "example"},
            {ParsedAttr::AS_CXX11, "gbox::example"},
        };

        Spellings = S;
    }
};

