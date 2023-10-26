module.exports = {
    rootDir: "./_build/default/",
    testMatch: ["**/*_test.js"],
    // ignoring these, are they are just temporary copies from runtime_deps, the final ones are in
    // the path with melange.emit target, e.g. _build/default/dom/test/test/dom/test/__snapshots__
    modulePathIgnorePatterns: ["react/test/__snapshots__", "dom/test/__snapshots__"]
};
  