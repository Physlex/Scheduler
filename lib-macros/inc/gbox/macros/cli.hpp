#ifndef GBOX_MACROS_DRIVER_HPP_
#define GBOX_MACROS_DRIVER_HPP_

#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>

#include <string>

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
std::string writeVirtualFile(
    const std::string &input_file, const std::string &rewritten_file,
    const std::string &output_dir
);

}  // namespace cli
}  // namespace gbox

#endif  // GBOX_MACROS_DRIVER_HPP_
