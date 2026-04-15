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
      cmakeFlags = [
        "-DCMAKE_TOOLCHAIN_FILE=$PWD/cmake/clang-toolchain.cmake"
        "-DCMAKE_C_COMPILER=${pkgs.llvmPackages_latest.clang}/bin/clang"
        "-DCMAKE_CXX_COMPILER=${pkgs.llvmPackages_latest.clang}/bin/clang++"
        "-DCMAKE_BUILD_TYPE=Debug"
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"
      ];
      cmakeEnv = ''
        export LLVM_DIR=${pkgs.llvmPackages_latest.llvm.dev}/lib/cmake/llvm
        export Clang_DIR=${pkgs.llvmPackages_latest.libclang.dev}/lib/cmake/clang
        export LD_LIBRARY_PATH=${pkgs.stdenv.cc.cc.lib}/lib/
        export LIBCLANG_PATH=${pkgs.llvmPackages_latest.libclang.lib}/lib
      '';
    in {
      packages.duck = pkgs.rustPlatform.buildRustPackage {
        pname = "duck";
        version = "unstable";

        src = pkgs.fetchFromGitHub {
          owner = "rdmsr";
          repo = "duck";
          rev = "ff95938795dfc9e55f891eb27de68a4bf63b122a";
          hash = "sha256-aBCm69V//ZtYmmE3GAvUqErSnVIp59te6bJOlibICSM==";
        };

        cargoHash = "sha256-XzqhofYePhrusi5FPx9qWh8gBCsNUb3QAvi78SKNoJs=";

        LIBCLANG_PATH = "${pkgs.llvmPackages_latest.libclang.lib}/lib";
        buildInputs = [ pkgs.llvmPackages_latest.libclang ];
        nativeBuildInputs = [ pkgs.pkg-config ];
      };

      packages.default = pkgs.stdenv.mkDerivation {
        pname = "gbox";
        version = "0.1.0";
        src = ./.;

        nativeBuildInputs = with pkgs; [
          ninja
          cmake
          gtest
          llvmPackages_latest.clang
          self.packages.${system}.duck
        ];

        buildInputs = with pkgs; [
          llvmPackages_latest.llvm
          llvmPackages_latest.libclang.dev
          llvmPackages_latest.libclang.lib
          llvmPackages_latest.llvm.dev
        ];

        configurePhase = ''
          ${cmakeEnv}
          cmake -B build -S . -G Ninja ${builtins.concatStringsSep " " cmakeFlags}
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
        packages = self.packages.${system}.default.buildInputs
          ++ self.packages.${system}.default.nativeBuildInputs
          ++ [ pkgs.pre-commit pkgs.uv pkgs.gdb pkgs.nixd ];

        shellHook = ''
          ${cmakeEnv}

          cat > ./.clangd <<EOF
          CompileFlags:
            CompilationDatabase: build
            Add:
              - -I${pkgs.llvmPackages_latest.llvm.dev}/include
              - -I${pkgs.llvmPackages_latest.libclang.dev}/include
          Index:
            Background: Build
          EOF

          configure() {
            cmake -B build -S . -G Ninja ${builtins.concatStringsSep " " cmakeFlags}
          }

          echo "Nix development environment initialized."
        '';
      };
    });
}
