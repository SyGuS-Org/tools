#!/usr/bin/env bash

set -Eeuo pipefail

if (( ${BASH_VERSION%%.*} < 4 )); then echo "ERROR: [bash] version 4.0+ required!" ; exit -1 ; fi

ROOT="$(cd -P -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)/.."

if ! [ -x "$(command -v python3)" ]; then
    echo "Python 3 not found!" ; exit 1
fi

python3 -m pip install --user -r "$ROOT/requirements.txt"