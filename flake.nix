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
        pkgs.uv
        llvmPackages_latest.llvm
        llvmPackages_latest.libclang.dev
        llvmPackages_latest.libclang.lib
        llvmPackages_latest.clang
        llvmPackages_latest.llvm.dev
        gdb
        gtest
      ];

      nativeBuildInputs = with pkgs; [
        ninja
        cmake
      ];

      shellHook = ''
        export LLVM_DIR=${pkgs.llvmPackages_latest.llvm.dev}/lib/cmake/llvm
        export Clang_DIR=${pkgs.llvmPackages_latest.libclang.dev}/lib/cmake/clang
        export LD_LIBRARY_PATH=${pkgs.stdenv.cc.cc.lib}/lib/
        echo "Nix development environment initialized."
        ./tools/setup.sh

        # TODO: Replace this with installing the plugin from a github artifact.
        #       for now, this makes sure that the plugin is installed on first
        #       pass.
        cmake --build build
      '';
    };
  };
}
