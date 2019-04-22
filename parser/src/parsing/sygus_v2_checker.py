#!/usr/bin/python

import argparse
from sygus_v2_parser import SygusV2Parser
from sygus_v2_lexer import SygusV2Lexer
from visitors import SymbolTableBuilder


def _parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file')
    return parser.parse_args()


def _main():
    args = _parse_args()

    with open(args.input_file, 'r') as f:
        contents = f.read()

    # lexer = SygusV2Lexer()
    # for tok in lexer.lex(contents):
    #     print(tok)

    parser = SygusV2Parser()
    program = parser.parse(contents)
    SymbolTableBuilder.run(program)


if __name__ == '__main__':
    _main()
