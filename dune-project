(lang dune 3.8)

(using melange 0.1)

(generate_opam_files true)

(name melange-testing-library)

(source
 (github melange-community/melange-testing-library))

(authors "David Sancho Moreno <dsnxmoreno@gmail.com>")

(maintainers "David Sancho Moreno <dsnxmoreno@gmail.com>")

(homepage "https://github.com/melange-community/melange-testing-library")

(bug_reports "https://github.com/melange-community/melange-testing-library")

(license "MIT")

(package
 (name melange-testing-library)
 (synopsis "Melange bindings for testing-library (dom-testing-library and react-testing-library)")
 (depends
  ocaml
  (melange
   (>= "5.0.0"))
  reason-react
  (melange-jest :with-test)
  (opam-check-npm-deps :with-test) ; todo: use with-dev-setup once opam 2.2 is out
  (bisect_ppx
   (and
    :with-test
    (>= "2.5.0")))))
