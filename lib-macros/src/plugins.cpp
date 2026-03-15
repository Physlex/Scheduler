/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Lex/Token.h>

#include <vector>

using namespace clang;
using namespace clang::attr;
using namespace clang::ast_matchers;
using namespace llvm;

std::vector<Token> proc_macro_executor(std::vector<Token> tokens) {
    llvm::outs() << "invoked executor proc macro!";

    Token tok = Token();
    std::vector<Token> vec = {tok};
    return vec;
}

/**
 *  @brief TODO: DOCS
 */
class HandleFuncDecl : public MatchFinder::MatchCallback {
   public:
    //! @brief TODO: DOCS
    void run(const MatchFinder::MatchResult& res) override {
        const FunctionDecl* func_decl =
            res.Nodes.getNodeAs<FunctionDecl>("funcDecl");
        if (!func_decl) {
            return;
        }

        const FunctionDecl* fdef = func_decl->getDefinition();
        if (!fdef) {
            return;
        }

        std::vector<StringRef> annotations;

        for (auto attr : fdef->getAttrs()) {
            const AnnotateAttr* annotated_attr = dyn_cast<AnnotateAttr>(attr);
            if (annotated_attr) {
                annotations.push_back(annotated_attr->getAnnotation());
            }
        }

        // Skip functions that have no annotations
        if (annotations.size() == 0) {
            return;
        }

        ASTContext* ctx = res.Context;
        SourceManager& sm = ctx->getSourceManager();

        SourceRange src_range = fdef->getSourceRange();
        SourceLocation src_begin = src_range.getBegin();
        SourceLocation src_end = src_range.getEnd();

        unsigned start = sm.getFileOffset(src_begin);
        unsigned end = sm.getFileOffset(src_end);

        FileID file = sm.getFileID(src_begin);
        StringRef buffer = sm.getBufferData(file);
        StringRef func_src_text = buffer.substr(start, end - start + 1);

        MemoryBufferRef func_src_buff = MemoryBufferRef(
            func_src_text, "<proc_macro_attribute function slice>");

        std::vector<Token> token_stream;

        Lexer lexer =
            Lexer(file, func_src_buff, sm, res.Context->getLangOpts());
        Token tok;
        while (!lexer.LexFromRawLexer(tok)) {
            StringRef text = StringRef(func_src_buff.getBufferStart() +
                                           tok.getLocation().getRawEncoding(),
                                       tok.getLength());

            llvm::outs() << "Token: " << text << " Kind: " << tok.getName()
                         << "\n";
            token_stream.push_back(tok);
        }

        for (auto annotation : annotations) {
            if (annotation == "executor") {
                proc_macro_executor(token_stream);
            }
        }

        llvm::outs() << func_src_text << "\n";
    }
};

/**
 *  @brief TODO: DOCS
 */
class ProcMacroConsumer : public ASTConsumer {
   public:
    ProcMacroConsumer(std::string FileName) : FileName(std::move(FileName)) {}

    void HandleTranslationUnit(ASTContext& ctx) override {
        HandleFuncDecl handler;
        MatchFinder finder;

        // Match against function definitions
        finder.addMatcher(functionDecl().bind("funcDecl"), &handler);
        finder.matchAST(ctx);
    }

   private:
    std::string FileName;
};

class ProcMacroPlugin : public PluginASTAction {
   protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance& CI, llvm::StringRef InFile) override {
        return std::make_unique<ProcMacroConsumer>(InFile.str());
    }

    bool ParseArgs(const CompilerInstance& CI,
                   const std::vector<std::string>& Args) override {
        return true;
    }

    ActionType getActionType() override { return AddAfterMainAction; }
};

static FrontendPluginRegistry::Add<ProcMacroPlugin> X(
    "gbox-macros-plugin", "Procedural macro scheme for C/C++");
