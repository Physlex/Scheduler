#ifndef GBOX_MACROS_DRIVER_HPP_
#define GBOX_MACROS_DRIVER_HPP_

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>

#include <string>

#include "clang/Basic/DiagnosticIDs.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

namespace gbox::cli {

/**
 *  TODO: DOCS
 */
class Diagnostics {
  public:
    Diagnostics();

  private:
    std::unique_ptr<clang::DiagnosticOptions> opts;
    std::unique_ptr<clang::TextDiagnosticPrinter> printer;
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> ids;
};

/**
 *  This class implements a builder interface for mutating the command line arguments
 * provided to the driver program.
 *
 *  TODO: EXAMPLE USAGE
 */
class DiagBuilder {
  public:
    DiagBuilder() : clang_ident_(clang_ident), clang_path_(clang_path) {}

    /// Parses the clang identifer and absolute path from the binary
    DiagBuilder with_opts(std::unique_ptr<clang::DiagnosticOptions>) &&;

  private:
    /// The identifer of the clang driver used. Examples being "clang", "clang++"
    std::string clang_ident_;
    /// The absolute path to the OS-deciphered clang binary of the specified program name
    std::string clang_path_;
};

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

}  // namespace gbox::cli

#endif  // GBOX_MACROS_DRIVER_HPP_
