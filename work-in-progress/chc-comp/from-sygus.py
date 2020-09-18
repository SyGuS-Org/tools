#!/usr/bin/env python3

import pyparsing as pp

from argparse import ArgumentDefaultsHelpFormatter, ArgumentParser, FileType
from os import linesep


def serialize(statement):
    if type(statement) is not list:
        return statement

    return f'({" ".join(serialize(e) for e in statement)})'

def main(args):
    sexp = pp.nestedExpr(opener='(', closer=')')
    sexp.ignore(';' + pp.restOfLine)

    parser = pp.ZeroOrMore(sexp)
    ast = parser.parseFile(args.input_file, parseAll=True).asList()
    
    for statement in ast:
        if statement[0] == 'set-logic':
            assert(statement[1].startswith('CHC_'))
            statement[1] = 'HORN'
        elif statement[0] == 'constraint':
            statement[0] == 'assert'
        elif statement[0] == 'check-synth':
            statement[0] == 'check-sat'
        elif statement[0] == 'synth-fun':
            statement[0] == 'declare-fun'
            statement[2] = [var_decl[1] for var_decl in statement[2]]


    serialized_ast = (serialize(statement) for statement in ast)
    print(f'{linesep}{linesep}'.join(serialized_ast))

if __name__ == '__main__':
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        'input_file', type=FileType('r'),
        help='Path to an input file (or stdin if "-")')

    main(parser.parse_args())
