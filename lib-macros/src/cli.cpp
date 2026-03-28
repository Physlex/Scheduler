/**
 * TODO: DOCS
 */

// FIXME: AI-GENERATED (Too tired to give a fuck)

#include "gbox/macros/cli.hpp"

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>

#include "gbox/macros/plugins.hpp"

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

std::optional<std::string> createTempFile(
    clang::CompilerInvocation &invocation, gbox::ProcMacroAction &action
) {
    std::string rewritten = action.getRewritten();
    if (rewritten.length() == 0) return std::nullopt;

    const std::string &output = invocation.getFrontendOpts().OutputFile;
    llvm::SmallString<128> virtual_dir(llvm::sys::path::parent_path(output));
    llvm::sys::path::append(virtual_dir, "virtual");

    if (llvm::sys::fs::create_directories(virtual_dir)) {
        llvm::errs() << "failed to create virtual dir\n";
        return std::nullopt;
    }

    llvm::SmallString<128> temp_model(virtual_dir);
    llvm::sys::path::append(
        temp_model,
        llvm::sys::path::stem(invocation.getFrontendOpts().Inputs[0].getFile())
    );

    auto tempfile = llvm::sys::fs::TempFile::create(temp_model + "-%%%%%%.c");
    if (!tempfile) {
        llvm::errs() << "failed to create temp file: "
                     << llvm::toString(tempfile.takeError()) << "\n";
        return std::nullopt;
    }

    // TODO: Why tf is this in a sub-group??
    {
        llvm::raw_fd_ostream out(tempfile->FD, /*shouldClose=*/false);
        for (auto it = rewritten.begin(); it != rewritten.end(); ++it) out << *it;
    }

    std::string path = tempfile->TmpName;
    if (auto err = tempfile->keep()) {
        llvm::errs() << "failed to keep temp file: " << llvm::toString(std::move(err))
                     << "\n";
        return std::nullopt;
    }

    return path;
}

}  // namespace cli
}  // namespace gbox
