# Contributing
Thank you for considering contributing to the project.

## Branching model
The repository has two long-lived branches:

- **`master`** - the released library. Every commit on it is a release and
  carries a `vX.Y.Z` tag. Nothing is committed here directly.
- **`develop`** - the next release. Everything is merged here first, and it is
  the branch you start from and target.

Work happens on short-lived branches taken from `develop` and merged back into
it:

```
master   ──●────────────────────────────●──   (v1.6.3)          (v1.7.0)
            \                          /
develop      ●────●────●────●────●────●
                   \       /    \    /
fix/issue-123       ●─────●      \  /
feature/encoder                   ●●
```

Name the branch after what it does: `fix/issue-123`, `feature/rotary-encoder`,
`docs/getters-example`, `chore/release-automation`.

A release is cut when `develop` has accumulated enough changes - see
[Releasing](#releasing). Individual pull requests **do not** touch the version
number.

## Bug fix
1. Check for open [issues][issues] or create a new one.
2. Fork the repository.
3. Branch from `develop`: `git switch -c fix/issue-123 develop`.
4. Make your changes.
   - use consistent style
   - use descriptive commit messages
   - add an entry under `## [Unreleased]` in [`doc/changelog.md`][changelog]
5. Open a pull request **against `develop`**.

## Feature implementation
1. Check the [Discussions - Ideas][ideas-disc] or create a new discussion.
2. Fork the repository.
3. Branch from `develop`: `git switch -c feature/my-feature develop`.
4. Make your changes.
   - use consistent style
   - use descriptive commit messages
   - add Doxygen documentation
   - if applicable add an example
   - add an entry under `## [Unreleased]` in [`doc/changelog.md`][changelog]
5. Open a pull request **against `develop`**.

## Documentation improvement
1. Fork the repository.
2. Branch from `develop`.
3. Make your changes.
4. Open a pull request **against `develop`**.

## Releasing
*For maintainers.*

The version number appears in five places that cannot reference each other:

| Where | What |
| --- | --- |
| `library.properties` | `version=` - what Arduino's Library Manager reads |
| `README.md` | the download badge and the archive link |
| `doc/Doxygen/Doxyfile` | `PROJECT_NUMBER` |
| `src/LiquidMenu.h` | the `@version` doc tag |
| `src/LiquidMenu.h` | the `LIQUIDMENU_VERSION` string constant |

Keeping those in step by hand is error prone - they have drifted apart before -
so it is automated. Do not edit them directly; use [`tools/version.sh`][version-sh]:

```sh
tools/version.sh get      # print the current version
tools/version.sh check    # verify every file agrees (this runs in CI)
tools/version.sh notes    # print the current version's changelog section
```

To publish a release, run the **Release** workflow from the *Actions* tab and
choose `patch`, `minor` or `major` (or `exact` with a specific `x.y.z`). Tick
`dry_run` first if you want to see the diff and the release notes without
anything being pushed. The workflow, all from `develop`:

1. writes the new version into all five places,
2. turns `## [Unreleased]` in the changelog into `## [x.y.z] - <date>` and
   opens a fresh empty `[Unreleased]`,
3. commits that to `develop`,
4. merges `develop` into `master` and tags it `vx.y.z`,
5. creates the GitHub Release, using the changelog section as the notes.

It refuses to run if the `[Unreleased]` section is empty, or if `develop` is
missing commits that are on `master`.

Which part to bump follows [semantic versioning][semver]: `patch` for bug
fixes, `minor` for backwards-compatible additions, `major` for changes that
break existing sketches.

[issues]: https://github.com/VasilKalchev/LiquidMenu/issues
[ideas-disc]: https://github.com/VasilKalchev/LiquidMenu/discussions/categories/ideas
[changelog]: ../doc/changelog.md
[version-sh]: ../tools/version.sh
[semver]: https://semver.org/
