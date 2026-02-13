/** @file `gbox_executor.cpp`
 *  @brief This file implements tests for the gbox executor attribute parser.
 */

#include <clang/Tooling/Tooling.h>
#include <gtest/gtest.h>

#include "gbox/macros/plugins.hpp"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

static constexpr const char* src =
    ""
    "#define gbox_executor [[clang::annotate(\"runtime\")]]\n"
    "class X {};\n"
    "gbox_executor int main() { return 0; }"
    "";

TEST(runToolOnCode, CanSyntaxCheckCode) {
    clang::tooling::runToolOnCode(std::make_unique<FindMyAttributeAction>(),
                                  src);
}
