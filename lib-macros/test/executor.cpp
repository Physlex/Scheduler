/** @file `gbox_executor.cpp`
 *  @brief This file implements tests for the gbox executor attribute parser.
 */

#include <gtest/gtest.h>

#include <clang/Tooling/Tooling.h>

#include "gbox/macros/plugins.hpp"


TEST(runToolOnCode, CanSyntaxCheckCode) {
    clang::tooling::runToolOnCode(
        std::make_unique<FindMyAttributeAction>(), "class X {}; [[gbox::runtime]] int main() { return 0; }"
    );
}
