{
  description = "melange-testing-library Nix Flake";

  inputs.nix-filter.url = "github:numtide/nix-filter";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs = {
    url = "github:nix-ocaml/nix-overlays";
    inputs.flake-utils.follows = "flake-utils";
  };
  inputs.melange-src = {
    url = "github:melange-re/melange";
    inputs.nix-filter.follows = "nix-filter";
    inputs.flake-utils.follows = "flake-utils";
    inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, flake-utils, nix-filter, melange-src }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages."${system}".appendOverlays [
          (self: super: {
            ocamlPackages = super.ocaml-ng.ocamlPackages_5_1.overrideScope' (oself: osuper:
              with oself;
              {
                bisect_ppx = osuper.bisect_ppx.overrideAttrs (o: {
                  src = super.fetchFromGitHub {
                    owner = "aantron";
                    repo = "bisect_ppx";
                    rev = "a14c5b5cd4564d2992dd1b14238922029bc1b1d7";
                    hash = "sha256-zu4YZO9QZdhIa+0IF0KMnPDQJGNUz2+oaFjaWLY7dn0=";
                  };
                  nativeBuildInputs = o.nativeBuildInputs ++ [ melange ];
                  propagatedBuildInputs = o.propagatedBuildInputs ++ [ melange ];
                });
              });
          })
          melange-src.overlays.default
        ];
        inherit (pkgs) nodejs_latest lib stdenv darwin;

        melange-testing-library = with pkgs.ocamlPackages; buildDunePackage {
          pname = "melange-testing-library";
          version = "dev";

          src = ./.;
          nativeBuildInputs = with pkgs.ocamlPackages; [ melange ];
          checkInputs = [ bisect_ppx melange-jest melange-webapi ];
          doCheck = true;
          propagatedBuildInputs = with pkgs.ocamlPackages; [ melange ];
        };

        mkShell = { buildInputs ? [ ] }: pkgs.mkShell {
          inputsFrom = [ melange-testing-library ];
          nativeBuildInputs = with pkgs; [
            yarn
            nodejs_latest
          ] ++ (with pkgs.ocamlPackages; [
            ocamlformat
            merlin
            reason
            reason-react
          ]);
          inherit buildInputs;
        };
      in
      rec {
        packages.default = melange-testing-library;
        devShells = {
          default = mkShell { };
          release = mkShell {
            buildInputs = with pkgs; [ cacert curl ocamlPackages.dune-release git ];
          };
        };
      });
}
