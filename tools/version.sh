#!/usr/bin/env bash
#
# Single source of truth for the library's version number.
#
# The version is duplicated in several files that cannot reference each
# other (Arduino's library.properties, a Doxygen setting, a README badge,
# a C++ string constant). This script keeps them in sync.
#
# Usage:
#   tools/version.sh get             print the current version
#   tools/version.sh check           verify every file agrees (exit 1 if not)
#   tools/version.sh set <x.y.z>     write <x.y.z> everywhere
#   tools/version.sh bump <part>     set the next major|minor|patch version
#   tools/version.sh release <x.y.z> set the version and close the changelog
#   tools/version.sh notes [<x.y.z>] print a version's changelog section
#
# library.properties is authoritative for "get" and "check".

set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."

PROPERTIES="library.properties"
README="README.md"
DOXYFILE="doc/Doxygen/Doxyfile"
HEADER="src/LiquidMenu.h"
CHANGELOG="doc/changelog.md"

SEMVER='[0-9]+\.[0-9]+\.[0-9]+'

die() { printf 'error: %s\n' "$*" >&2; exit 1; }

# Reads the version out of each file. Every reader must print nothing (rather
# than fail) when it cannot find its pattern, so that `check` reports a clear
# mismatch instead of aborting.
read_properties() { sed -nE 's/^version=('"$SEMVER"')$/\1/p' "$PROPERTIES"; }
read_readme_url() { sed -nE 's/.*archive\/v('"$SEMVER"')\.zip.*/\1/p' "$README"; }
read_readme_badge() { sed -nE 's/.*badge\/download-('"$SEMVER"')-.*/\1/p' "$README"; }
read_doxyfile() { sed -nE 's/^PROJECT_NUMBER[[:space:]]*=[[:space:]]*('"$SEMVER"')[[:space:]]*$/\1/p' "$DOXYFILE"; }
read_header_doc() { sed -nE 's/^@version ('"$SEMVER"')$/\1/p' "$HEADER"; }
read_header_const() { sed -nE 's/^const char LIQUIDMENU_VERSION\[\] = "('"$SEMVER"')".*/\1/p' "$HEADER"; }

get() {
  local version
  version="$(read_properties)"
  [[ -n $version ]] || die "no 'version=x.y.z' line in $PROPERTIES"
  printf '%s\n' "$version"
}

check() {
  local expected status=0
  expected="$(get)"

  compare() { # <description> <file> <found>
    if [[ $3 != "$expected" ]]; then
      printf 'MISMATCH %-28s %s: %s\n' "$1" "$2" "${3:-<not found>}" >&2
      status=1
    fi
  }

  compare "download URL"      "$README"   "$(read_readme_url)"
  compare "download badge"    "$README"   "$(read_readme_badge)"
  compare "PROJECT_NUMBER"    "$DOXYFILE" "$(read_doxyfile)"
  compare "@version"          "$HEADER"   "$(read_header_doc)"
  compare "LIQUIDMENU_VERSION" "$HEADER"  "$(read_header_const)"

  if (( status != 0 )); then
    # shellcheck disable=SC2016 # the backticks are literal, not a subshell
    printf '\n%s says %s. Run `tools/version.sh set %s` to sync.\n' \
      "$PROPERTIES" "$expected" "$expected" >&2
    return 1
  fi
  printf 'all version strings agree: %s\n' "$expected"
}

set_version() {
  local version=$1
  [[ $version =~ ^$SEMVER$ ]] || die "'$version' is not a x.y.z version"

  sed -i -E 's/^version=.*$/version='"$version"'/' "$PROPERTIES"
  sed -i -E 's/archive\/v'"$SEMVER"'\.zip/archive\/v'"$version"'.zip/g' "$README"
  sed -i -E 's/badge\/download-'"$SEMVER"'-/badge\/download-'"$version"'-/g' "$README"
  sed -i -E 's/^(PROJECT_NUMBER[[:space:]]*=[[:space:]]*).*$/\1'"$version"'/' "$DOXYFILE"
  sed -i -E 's/^@version .*$/@version '"$version"'/' "$HEADER"
  sed -i -E 's/^(const char LIQUIDMENU_VERSION\[\] = ").*(";.*)$/\1'"$version"'\2/' "$HEADER"

  check
}

bump() {
  local part=$1 current major minor patch
  current="$(get)"
  IFS=. read -r major minor patch <<<"$current"
  case $part in
    major) major=$((major + 1)); minor=0; patch=0 ;;
    minor) minor=$((minor + 1)); patch=0 ;;
    patch) patch=$((patch + 1)) ;;
    *) die "bump takes major, minor or patch, not '$part'" ;;
  esac
  set_version "$major.$minor.$patch"
}

# Turns the "[Unreleased]" heading into a dated release heading and opens a
# fresh empty "[Unreleased]" section above it.
close_changelog() {
  local version=$1 date
  date="$(date -u +%Y-%m-%d)"

  grep -q '^## \[Unreleased\]$' "$CHANGELOG" \
    || die "no '## [Unreleased]' section in $CHANGELOG"
  ! grep -q "^## \[$version\]" "$CHANGELOG" \
    || die "$CHANGELOG already has a [$version] section"

  # Refuse to cut a release with an empty Unreleased section - it means
  # nothing was written down for the users.
  awk '/^## \[Unreleased\]$/{found=1; next} found && /^## \[/{exit} found && NF {entries++} END{exit !(entries>0)}' \
    "$CHANGELOG" || die "the [Unreleased] section is empty, nothing to release"

  sed -i "s/^## \[Unreleased\]$/## [Unreleased]\n\n## [$version] - $date/" "$CHANGELOG"
}

release() {
  local version=$1
  [[ $version =~ ^$SEMVER$ ]] || die "'$version' is not a x.y.z version"
  close_changelog "$version"
  set_version "$version"
}

# Prints one section of the changelog, for use as GitHub release notes.
notes() {
  local version=${1:-$(get)}
  awk -v heading="## [$version]" '
    index($0, heading) == 1 { found = 1; next }
    found && /^## \[/ { exit }
    found { print }
  ' "$CHANGELOG"
}

command=${1:-}
shift || true
case $command in
  get)     get ;;
  check)   check ;;
  set)     [[ $# -eq 1 ]] || die "set needs a x.y.z version"; set_version "$1" ;;
  bump)    [[ $# -eq 1 ]] || die "bump needs major, minor or patch"; bump "$1" ;;
  release) [[ $# -eq 1 ]] || die "release needs a x.y.z version"; release "$1" ;;
  notes)   notes "${1:-}" ;;
  *) sed -nE 's/^# ?//p' "${BASH_SOURCE[0]}" | sed -n '2,20p' >&2; exit 1 ;;
esac
