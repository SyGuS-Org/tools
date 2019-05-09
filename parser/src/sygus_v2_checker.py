#!/usr/bin/python

import argparse
import sys

from lib.sygus_v2_parser import SygusV2Parser
from lib.symbol_table_builder import SymbolTableBuilder


def _parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file')
    return parser.parse_args()


def _main():
    args = _parse_args()

    with open(args.input_file, 'r') as f:
        contents = f.read()

    parser = SygusV2Parser()
    program = parser.parse(contents)

    if program is None:
        print('Failed to parse.')
        sys.exit(1)

    SymbolTableBuilder.run(program)


if __name__ == '__main__':
    _main()
