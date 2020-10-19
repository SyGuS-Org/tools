#!/usr/bin/env python3

import pyparsing as pp
import re

from argparse import ArgumentDefaultsHelpFormatter, ArgumentParser, FileType
from os import linesep


sane_idx, sane_table = 0, dict()
sane_pattern = re.compile(r'__symbol[0-9]+')

def sanitize_and_serialize(node):
    global sane_idx
    if type(node) is not list:
        if type(node) is not str:
            return node
        if (node.startswith('|') and node.endswith('|')) or sane_pattern.match(node):
            if node not in sane_table:
                sane_table[node] = f'__symbol{sane_idx}'
                sane_idx += 1
            return sane_table[node]
        return node

    return f'({" ".join(sanitize_and_serialize(e) for e in node)})'

def main(args):
    i_expr = pp.QuotedString(quoteChar='"') | pp.QuotedString(quoteChar='|', unquoteResults=False)
    s_expr = pp.nestedExpr(opener='(', closer=')', ignoreExpr=i_expr)
    s_expr.ignore(';' + pp.restOfLine)

    parser = pp.ZeroOrMore(pp.Suppress('(exit)') | s_expr)
    ast = parser.parseFile(args.input_file, parseAll=True).asList()
    
    for statement in ast:
        if statement[0] == 'set-logic':
            assert(statement[1] == 'HORN')
            statement[1] = 'CHC_LIA'
        elif statement[0] == 'assert':
            statement[0] = 'constraint'
        elif statement[0] == 'check-sat':
            statement[0] = 'check-synth'
        elif statement[0] == 'declare-fun':
            statement[0] = 'synth-fun'
            statement[2] = [[f'x_{i}', t]
                            for i,t in enumerate(statement[2])]

    serialized_ast = (sanitize_and_serialize(statement) for statement in ast)
    print(f'{linesep}{linesep}'.join(serialized_ast))

if __name__ == '__main__':
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        'input_file', type=FileType('r'),
        help='Path to an input file (or stdin if "-")')

    main(parser.parse_args())
