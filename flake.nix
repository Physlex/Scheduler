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
    packages.${system}.llvm = pkgs.stdenv.mkDerivation {
      pname = "llvm-project";
      version = "21.1.8";

      src = pkgs.fetchFromGitHub {
        owner = "llvm";
        repo = "llvm-project";
        rev = "llvmorg-21.1.8";
        sha256 = "sha256-pgd8g9Yfvp7abjCCKSmIn1smAROjqtfZaJkaUkBSKW0";
      };

      buildPhase = ''
        echo "Build Phase"
        ls
      '';

      installPhase = ''
        echo "Install Phase"
        ls
        mkdir p $out/bin
      '';
    };

    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        self.packages.${system}.llvm
        gdb
        gtest
      ];

      nativeBuildInputs = with pkgs; [
        ninja
        cmake
      ];

      shellHook = ''
        echo "Nix development environment initialized."
        cmake -B build -S . -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/clang-toolchain.cmake
      '';
    };
  };
}
