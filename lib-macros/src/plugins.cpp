/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "gbox/macros/plugins.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

std::vector<clang::Token> proc_macro_executor(
    std::vector<clang::Token> tokens
) {
    llvm::outs() << "invoked executor proc macro!";

    clang::Token tok = clang::Token();
    std::vector<clang::Token> vec = {tok};
    return vec;
}

void HandleFuncDecl::run(
    const clang::ast_matchers::MatchFinder::MatchResult& res
) {
    const clang::FunctionDecl* func_decl =
        res.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl");
    if (!func_decl) {
        return;
    }

    const clang::FunctionDecl* fdef = func_decl->getDefinition();
    if (!fdef) {
        return;
    }

    std::vector<llvm::StringRef> annotations;

    for (auto attr : fdef->getAttrs()) {
        const auto* annotated_attr = dyn_cast<clang::AnnotateAttr>(attr);
        if (annotated_attr) {
            annotations.push_back(annotated_attr->getAnnotation());
        }
    }

    // Skip functions that have no annotations
    if (annotations.size() == 0) {
        return;
    }

    clang::ASTContext* ctx = res.Context;
    clang::SourceManager& sm = ctx->getSourceManager();

    clang::SourceRange src_range = fdef->getSourceRange();
    clang::SourceLocation src_begin = src_range.getBegin();
    clang::SourceLocation src_end = src_range.getEnd();

    unsigned start = sm.getFileOffset(src_begin);
    unsigned end = sm.getFileOffset(src_end);

    clang::FileID file = sm.getFileID(src_begin);
    llvm::StringRef buffer = sm.getBufferData(file);
    llvm::StringRef func_src_text = buffer.substr(start, end - start + 1);

    llvm::MemoryBufferRef func_src_buff =
        llvm::MemoryBufferRef(func_src_text, "<proc_macro_attribute function slice>");

    std::vector<clang::Token> token_stream;
    clang::Lexer lexer =
        clang::Lexer(file, func_src_buff, sm, res.Context->getLangOpts());

    clang::Token tok;
    while (!lexer.LexFromRawLexer(tok)) {
        const uint32_t tok_start = sm.getFileOffset(tok.getLocation());
        const uint32_t tok_length = tok.getLength();
        const char* text_start = func_src_buff.getBufferStart();

        llvm::StringRef slice = llvm::StringRef(text_start + tok_start, tok_length);
        llvm::outs() << "clang::Token: " << slice << " Kind: " << tok.getName() << "\n";

        token_stream.push_back(tok);
    }

    for (auto annotation : annotations) {
        if (annotation == "executor") {
            proc_macro_executor(token_stream);
        }
    }
}

void ProcMacroConsumer::HandleTranslationUnit(
    clang::ASTContext& ctx
) {
    HandleFuncDecl handler;
    clang::ast_matchers::MatchFinder finder;

    // Match against function definitions
    finder.addMatcher(clang::ast_matchers::functionDecl().bind("funcDecl"), &handler);
    finder.matchAST(ctx);
}

static clang::FrontendPluginRegistry::Add<ProcMacroPlugin>
    X("gbox-macros-plugin", "Procedural macro scheme for C/C++");
