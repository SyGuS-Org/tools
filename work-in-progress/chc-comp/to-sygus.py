#!/usr/bin/env python3

import pyparsing as pp
import re
import sys

from argparse import ArgumentDefaultsHelpFormatter, ArgumentParser, FileType


sane_idx, sane_table = 0, dict()
sane_pattern = re.compile(r'__symbol[0-9]+')
pp.ParserElement.enablePackrat()


def sanitize_and_serialize(args, node):
    global sane_idx

    if type(node) is not list:
        if not args.sanitize_names:
            return node
        if type(node) is not str:
            return node
        if (node.startswith('|') and node.endswith('|')) or sane_pattern.match(node):
            if node not in sane_table:
                sane_table[node] = f'__symbol{sane_idx}'
                sane_idx += 1
            return sane_table[node]
        return node

    return f'({" ".join(sanitize_and_serialize(args, e) for e in node)})'

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

    sys.stdout.writelines(sanitize_and_serialize(args, statement) + '\n'
                          for statement in ast)

if __name__ == '__main__':
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '-s', '--sanitize-names', action='store_true',
        help='Replace SMTLib quoted symbols with simple symbols')
    parser.add_argument(
        'input_file', type=FileType('r'),
        help='Path to an input file (or stdin if "-")')

    main(parser.parse_args())
