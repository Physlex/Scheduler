/**
 * @brief This module implements tests for the plugins.cpp implementation.
 */

#include "gbox/macros/action/plugins.hpp"

#include <clang/Tooling/Tooling.h>
#include <gtest/gtest.h>

TEST(PluginsTestSuite, TestSimple) {
    const char *code =
        ""
        "#define gbox_executor clang::annotate(\"runtime\")\n"
        "\n"
        "[[gbox_executor]]\n"
        "int main() { return 0; }"
        "";

    clang::tooling::runToolOnCode(
        std::make_unique<gbox::plugins::ProcMacroAction>(), code
    );
}
