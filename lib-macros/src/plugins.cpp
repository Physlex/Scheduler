/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/ParsedAttrInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::attr;
using namespace llvm;
