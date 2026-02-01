{
  description = "Nix package management for the gbox framework";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";  # adjust if needed
    pkgs = import nixpkgs { inherit system; };
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        llvmPackages.llvm
        llvmPackages.clang
        llvmPackages.libclang.dev
        llvmPackages.libclang.lib
        llvmPackages.llvm.dev
        gdb
        gtest
      ];

      nativeBuildInputs = with pkgs; [
        ninja
        cmake
      ];

      shellHook = ''
        export LLVM_DIR=${pkgs.llvmPackages.llvm.dev}/lib/cmake/llvm
        export Clang_DIR=${pkgs.llvmPackages.libclang.dev}/lib/cmake/clang
        echo "Nix development environment initialized."
        cmake -B build -S . -G Ninja \
          -DCMAKE_TOOLCHAIN_FILE=cmake/clang-toolchain.cmake \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=On
      '';
    };
  };
}
