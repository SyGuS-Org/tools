#!/usr/bin/env bash

set -Euo pipefail

if (( ${BASH_VERSION%%.*} < 4 )); then echo "ERROR: [bash] version 4.0+ required!" ; exit -1 ; fi

USER_DIR="$(pwd)"
ROOT_DIR="$(cd -P -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)/.."

SYGUS_EXTENSION="sl"
SYGUS_STANDARD="2"

IS_VERBOSE=""
DO_COLLECT_ERRORS=""

usage() {
  if [ -n "$1" ]; then echo -e "\nERROR: $1" >&2 ; fi
  echo -en "
Usage: $0 [flags] [options] <dir> [<dir> [<dir> ...]]

Options:
    [--extension, -e]           The extension for SyGuS files           (default = $SYGUS_EXTENSION)
    [--standard, -s]            The SyGuS standard to check against     (default = $SYGUS_STANDARD)

Flags:
    [--collect-errors, -c]      Also print the exceptions raised for FAILed files
    [--verbose, -v]             Increase output verbosity (list PASSsed files too)
" 1>&2 ; exit 1
}

for opt in "$@"; do
  shift
  case "$opt" in
    "--collect-errors")     set -- "$@" "-c" ;;
    "--extension")          set -- "$@" "-e" ;;
    "--standard")           set -- "$@" "-s" ;;
    "--verbose")            set -- "$@" "-v" ;;

    "--")             set -- "$@" "--" ;;
    "--"*)            usage "Unrecognized option: $opt." ;;
    *)                set -- "$@" "$opt"
  esac
done

OPTIND=1
while getopts ':ce:s:v' OPTION ; do
  case "$OPTION" in
    "c" ) DO_COLLECT_ERRORS="yes"
          ;;
    "e" ) SYGUS_EXTENSION="$OPTARG"
          ;;
    "s" ) SYGUS_STANDARD="$OPTARG"
          ;;
    "v" ) IS_VERBOSE="yes"
          ;;
      * ) usage "Unrecognized option: -$OPTARG." ;;
  esac
done
shift $(($OPTIND -1))

[ "$#" -ge "1" ] || usage "No directories provided for checking."

cd "$ROOT_DIR"
for DIR in $@; do
    REAL_DIR="$(realpath $USER_DIR/$DIR)"
    if [ ! -d "$REAL_DIR" ]; then
        echo -e "\n! Skipped MISSING directory: \"$DIR\""
    else
        echo -e "\n> Checking within directory: \"$DIR\" ..."
        for FILE in $(find "$REAL_DIR" -name *".$SYGUS_EXTENSION" | sort) ; do
            STDERR=$(python3 -m sygus.bin.check -s "$SYGUS_STANDARD" "$FILE" 2>&1 > /dev/null)
            if [ "$?" -eq "0" ]; then
                [ "$IS_VERBOSE" == "yes" ] && echo "  [PASS]  ${FILE#$REAL_DIR/}"
            else
                echo "  [FAIL]  ${FILE#$REAL_DIR/}"
                [ "$DO_COLLECT_ERRORS" == "yes" ] && echo -e "$STDERR\n"
            fi
        done
    fi
    printf "%0100d\n" | tr 0 -
done
