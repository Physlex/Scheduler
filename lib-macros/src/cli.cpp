/**
 * TODO: DOCS
 */

// FIXME: AI-GENERATED (Too tired to give a fuck)

#include "gbox/macros/cli.hpp"

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>

namespace gbox {
namespace cli {

std::string stripGBFromPath(std::string path) {
    const auto gb_substr = std::string("gbclang");
    const auto gb_replace = std::string("clang");
    size_t pos = path.find(gb_substr);

    if (pos == std::string::npos) return "";

    const auto clang_path = path.replace(pos, gb_substr.length(), gb_replace);
    return clang_path.substr(pos, gb_replace.length());
}

std::string clangPathFromName(std::string executable_path) {
    if (!llvm::sys::fs::exists(executable_path)) {
        const auto path = llvm::sys::findProgramByName(executable_path);
        if (path) executable_path = *path;
    }

    return std::string(executable_path);
}

void fixupDiagPrefixExeName(
    clang::TextDiagnosticPrinter *diag_client, const std::string &path
) {
    auto exe_basename = llvm::StringRef(llvm::sys::path::stem(path));
    if (exe_basename.equals_insensitive("cl")) {
        exe_basename = "clang-cl";
    }

    diag_client->setPrefix(std::string(exe_basename));
}

std::string writeVirtualFile(
    const std::string &input_file, const std::string &rewritten_content,
    const std::string &output_dir
) {
    auto virtual_path = llvm::SmallString<128>(output_dir);
    llvm::sys::path::append(
        virtual_path, llvm::sys::path::stem(input_file) + ".gbox.cpp"
    );

    std::error_code ec;
    auto out = llvm::raw_fd_ostream(virtual_path.str(), ec);
    if (ec) {
        llvm::outs() << "ERROR\n";
        return "";
    }

    out << rewritten_content;

    return std::string(virtual_path);
}

}  // namespace cli
}  // namespace gbox
