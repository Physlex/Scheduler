/**
 * @brief This module implements tests for the plugins.cpp implementation.
 */

#include <gtest/gtest.h>
#include <clang/Tooling/Tooling.h>

#include "gbox/macros/plugins.hpp"

TEST(PluginsTestSuite, TestSimple) {
    const char *code = 
    ""
    "#define gbox_executor clang::annotate(\"runtime\")\n"
    "[[gbox_executor]]\n"
    "int main() { return 0; }"
    "";

    clang::tooling::runToolOnCode(std::make_unique<ProcMacroPlugin>(), code);
}
