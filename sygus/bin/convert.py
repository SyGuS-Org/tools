#!/usr/bin/env python3

from argparse import ArgumentDefaultsHelpFormatter, ArgumentParser, FileType

from ..src.symbol_table_builder import SymbolTableBuilder


def main(args):
    # Step 1: Parse the program
    if args.source_sygus_standard == '1':
        from ..src.v1.parser import SygusV1Parser
        parser = SygusV1Parser()
    elif args.source_sygus_standard == '2':
        from ..src.v2.parser import SygusV2Parser
        parser = SygusV2Parser()
    else:
        raise NotImplementedError

    program = parser.parse(args.input_file.read())
    symbol_table = SymbolTableBuilder.run(program)

    # Step 2: Postprocess the program, if needed
    if args.target_sygus_standard != args.source_sygus_standard:
        if args.target_sygus_standard == '1':
            from ..src.v1.processor import SygusV1Processor as processor
        elif args.target_sygus_standard == '2':
            from ..src.v2.processor import SygusV2Processor as processor
        else:
            raise NotImplementedError

        processor.run(program, symbol_table)
        symbol_table = SymbolTableBuilder.run(program)

    # Step 3: Print the converted program
    if args.target_sygus_standard == '1':
        from ..src.v1.printer import SygusV1ASTPrinter as printer
    elif args.target_sygus_standard == '2':
        from ..src.v2.printer import SygusV2ASTPrinter as printer
    else:
        raise NotImplementedError

    print(printer.run(program, symbol_table, vars(args)))


if __name__ == '__main__':
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '-b', '--binarize', action='store_true',
        help='Convert all chainable operators to binary operator applications')
    parser.add_argument(
        '-q', '--quiet', action='store_true',
        help='Suppress all messages and debugging output')
    parser.add_argument(
        '-u', '--no-unary-minus', action='store_true',
        help='Convert all (- x) terms to (- 0 x)')

    parser.add_argument(
        '-s', '--source-sygus-standard', default='1', choices=['1','2'],
        help='The SyGuS language standard used in the input file')
    parser.add_argument(
        '-t', '--target-sygus-standard', default='2', choices=['1','2'],
        help='The SyGuS language standard for the output file')

    parser.add_argument(
        'input_file', type=FileType('r'),
        help='Path to an input file (or stdin if "-")')

    main(parser.parse_args())
