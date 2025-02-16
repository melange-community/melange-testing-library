{
  description = "melange-testing-library Nix Flake";

  inputs.nixpkgs.url = "github:nix-ocaml/nix-overlays";

  outputs = { self, nixpkgs }:
    let
      forAllSystems = f: nixpkgs.lib.genAttrs nixpkgs.lib.systems.flakeExposed (system:
        let
          pkgs = nixpkgs.legacyPackages.${system}.extend (self: super: {
            ocamlPackages = super.ocaml-ng.ocamlPackages_5_1.overrideScope (oself: osuper:
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
          });
        in
        f pkgs);
    in
    {
      packages = forAllSystems (pkgs:
        let
          inherit (pkgs) nodejs_latest;

          melange-testing-library = with pkgs.ocamlPackages; buildDunePackage {
            pname = "melange-testing-library";
            version = "dev";

            src =
              let fs = pkgs.lib.fileset; in
              fs.toSource {
                root = ./.;
                fileset = fs.unions [
                  ./dune-project
                  ./dune
                  ./dom
                  ./react
                  ./melange-testing-library.opam
                ];
              };


            nativeBuildInputs = with pkgs.ocamlPackages; [ melange reason ];
            nativeCheckInputs = [ nodejs_latest ];
            checkInputs = [ bisect_ppx melange-jest melange-webapi ];
            doCheck = false;
            propagatedBuildInputs = with pkgs.ocamlPackages; [ melange reason-react ];
          };
        in
        {
          inherit melange-testing-library;
          default = melange-testing-library;
        });

      devShells = forAllSystems (pkgs:
        let
          mkShell = { buildInputs ? [ ] }: pkgs.mkShell {
            inputsFrom = [ self.packages.${pkgs.system}.melange-testing-library ];
            nativeBuildInputs = with pkgs; [
              yarn
              nodejs_latest
            ] ++ (with pkgs.ocamlPackages; [
              ocamlformat
              merlin
              reason
            ]);
            inherit buildInputs;
          };
        in
        {
          default = mkShell { };
          release = mkShell {
            buildInputs = with pkgs; [ cacert curl ocamlPackages.dune-release git ];
          };
        })
      ;
    };
}
