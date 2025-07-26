# wordCountRanking.nix

let
  pkgs = import <nixpkgs> {};
in
  pkgs.stdenv.mkDerivation {
    name = "word_count_rank";

    src = ./.;

    buildPhase = ''
      $CC -O -fsanitize=undefined -Wall word_count_rank.c -o word_count_rank
    '';

	installPhase = ''
		mkdir -p $out/bin
		cp word_count_rank $out/bin/word_count_rank
	'';
  }

