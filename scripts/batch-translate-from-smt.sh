#!/usr/bin/env bash

set -Euo pipefail

if (( ${BASH_VERSION%%.*} < 4 )); then echo "ERROR: [bash] version 4.0+ required!" ; exit -1 ; fi

USER_DIR="$(pwd)"
ROOT_DIR="$(cd -P -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)/.."

SYGUS_EXTENSION="sl"
SMT_EXTENSION="smt2"

IS_VERBOSE=""
DO_COLLECT_ERRORS=""
OUTPUT_DIR=""
INTERNAL_OPTIONS=""

usage() {
  if [ -n "$1" ]; then echo -e "\nERROR: $1" >&2 ; fi
  echo -en "
Usage: $0 [flags] -o <dir> <dir>

Parameters:
    [--output-dir, -o]          The output directory for converted files

Flags:
    [--verbose, -v]             Increase output verbosity (list PASSsed files too)
" 1>&2 ; exit 1
}

for opt in "$@"; do
  shift
  case "$opt" in
    "--output-dir")         set -- "$@" "-o" ;;
    "--verbose")            set -- "$@" "-v" ;;

    "--")                   set -- "$@" "--" ;;
    "--"*)                  usage "Unrecognized option: $opt." ;;
    *)                      set -- "$@" "$opt"
  esac
done

OPTIND=1
while getopts ':e:I:o:s:t:v' OPTION ; do
  case "$OPTION" in
    "o" ) OUTPUT_DIR="$OPTARG"
          ;;
    "v" ) IS_VERBOSE="yes"
          ;;
      * ) usage "Unrecognized option: -$OPTARG." ;;
  esac
done
shift $(($OPTIND -1))

[ -n "$OUTPUT_DIR" ] || usage "Missing required parameter: --output-dir | -o <dir>."
OUTPUT_DIR="$(realpath $USER_DIR/$OUTPUT_DIR)"
mkdir -p "$OUTPUT_DIR" || usage "Could not locate or create output directory: $OUTPUT_DIR."

[ "$#" -eq "1" ] || usage "This script accepts exactly 1 input directory ($# provided)."
[ -d "$1" ] || usage "Could not locate source directory: $1."
SRC_DIR="$(realpath $USER_DIR/$1)"

cd "$ROOT_DIR"
for FILE in $(find "$SRC_DIR" -name *".$SMT_EXTENSION" | sort) ; do
    FILE_REL_PATH="${FILE#$SRC_DIR/}"
    OUTPUT_REL_PATH=${FILE_REL_PATH%.*}.$SYGUS_EXTENSION
    mkdir -p "$OUTPUT_DIR/$(dirname $OUTPUT_REL_PATH$)"

    python3 -m work-in-progress.chc-comp.to-sygus  \
            "$FILE"                                                     \
            2> "$OUTPUT_DIR/$OUTPUT_REL_PATH.err" > "$OUTPUT_DIR/$OUTPUT_REL_PATH"

    if [ "$?" -eq "0" ]; then
        rm -rf "$OUTPUT_DIR/$OUTPUT_REL_PATH.err"
        [ "$IS_VERBOSE" == "yes" ] && echo "  [PASS]  $OUTPUT_REL_PATH"
    else
        rm -rf "$OUTPUT_DIR/$OUTPUT_REL_PATH"
        echo "  [FAIL]  $OUTPUT_REL_PATH"
    fi
done
