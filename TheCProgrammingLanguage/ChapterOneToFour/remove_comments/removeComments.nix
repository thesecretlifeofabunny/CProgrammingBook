# removeComments.nix
# yoinked from https://book.divnix.com/ch06-01-simple-c-program.html

let
  pkgs = import <nixpkgs> { };
in
  pkgs.stdenv.mkDerivation {
    name = "remove_comments";

    src = ./.;

    # Use $CC as it allows for stdenv to reference the correct C compiler
    buildPhase = ''
      $CC remove_comments.c -o remove_comments
    '';

	installPhase = ''
		mkdir -p $out/bin
		cp remove_comments $out/bin/remove_comments
	'';
  }

