# hello.nix
let
  pkgs = import <nixpkgs> { };
in
  pkgs.stdenv.mkDerivation {
    name = "hello-nix";

    src = ./.;

    buildPhase = ''
      $CC hello.c -o hello_nix
    '';

	installPhase = ''
		mkdir -p $out/bin
		cp hello_nix $out/bin/hello_nix
	'';
  }

