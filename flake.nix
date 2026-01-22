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
        packages = with pkgs; [
          clang
          clang-tools
          ninja
          gdb
          gtest
        ];

        shellHook = ''
          echo "Nix development environment initialized."
          cmake -B build -S . -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/clang-toolchain.cmake
        '';
      };
    };
}
