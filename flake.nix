{
  description = "Nix package management for the gbox framework";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
    ...
  }:
  flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        pname = "gbox";
        version = "0.1.0";
        src = ./.;

        nativeBuildInputs = with pkgs; [
          ninja
          cmake
          gtest
          llvmPackages_latest.clang
        ];

        buildInputs = with pkgs; [
          llvmPackages_latest.llvm
          llvmPackages_latest.libclang.dev
          llvmPackages_latest.libclang.lib
          llvmPackages_latest.llvm.dev
        ];

        configurePhase = ''
          export LLVM_DIR=${pkgs.llvmPackages_latest.llvm.dev}/lib/cmake/llvm
          export Clang_DIR=${pkgs.llvmPackages_latest.libclang.dev}/lib/cmake/clang
          export LD_LIBRARY_PATH=${pkgs.stdenv.cc.cc.lib}/lib/

          cmake -B build -S . -G Ninja \
              -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake/clang-toolchain.cmake \
              -DCMAKE_C_COMPILER=${pkgs.llvmPackages_latest.clang}/bin/clang \
              -DCMAKE_CXX_COMPILER=${pkgs.llvmPackages_latest.clang}/bin/clang++ \
              -D CMAKE_BUILD_TYPE=Debug \
              -DCMAKE_EXPORT_COMPILE_COMMANDS=1
        '';

        buildPhase = ''
          ninja -C build
        '';

        installPhase = ''
          mkdir -p $out/bin $out/lib $out/include
          cp build/bin/gbclang $out/bin/
          cp build/lib/libgbox-runtime.a $out/lib/
          cp -r lib-runtime/inc $out/include
        '';
      };

      devShells.default = pkgs.mkShell {
        packages = self.packages.${system}.default.buildInputs ++ [ pkgs.pre-commit pkgs.uv pkgs.gdb ];

        shellHook = ''
          export LLVM_DIR=${pkgs.llvmPackages_latest.llvm.dev}/lib/cmake/llvm
          export Clang_DIR=${pkgs.llvmPackages_latest.libclang.dev}/lib/cmake/clang
          export LD_LIBRARY_PATH=${pkgs.stdenv.cc.cc.lib}/lib/
          echo "Nix development environment initialized."
        '';
      };
    });
}
