#ifndef GBOX_MACROS_DRIVER_HPP_
#define GBOX_MACROS_DRIVER_HPP_

#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>

#include <optional>
#include <string>

#include "gbox/macros/plugins.hpp"

namespace gbox {
namespace cli {

// TODO: DOCS
extern std::string stripGBFromPath(std::string path);

// TODO: DOCS
extern std::string clangPathFromName(std::string executable_path);

// TODO: DOCS
extern void fixupDiagPrefixExeName(
    clang::TextDiagnosticPrinter *diag_client, const std::string &path
);

// TODO: DOCS
extern std::optional<std::string> createTempFile(
    clang::CompilerInvocation &invocation, gbox::ProcMacroAction &action
);

}  // namespace cli
}  // namespace gbox

#endif  // GBOX_MACROS_DRIVER_HPP_
