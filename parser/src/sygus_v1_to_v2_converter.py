#!/usr/bin/python

import argparse
import sys
import os
from io import StringIO
import traceback

from lib.sygus_v1_parser import SygusV1Parser
from lib.symbol_table_builder import SymbolTableBuilder
from lib.sygus_v2_ast_printer import SygusV2ASTPrinter
from lib.exceptions import ParseException


def _parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file_or_dir', help='The path to the input file or directory.')
    parser.add_argument('-o', '--output', help='The path to the output file or directory. If omitted, output will be generated on stdout.')
    return parser.parse_args()


def _convert_one_file(input_path, output_path):
    with open(input_path, 'r') as f:
        contents = f.read()

    parser = SygusV1Parser()
    program = parser.parse(contents)

    if program is None:
        raise ParseException('Failed to parse V1 program.', None, None)

    SymbolTableBuilder.run(program)
    result = SygusV2ASTPrinter.run(program)

    if output_path is not None:
        with open(output_path, 'w') as f:
            f.write(result)
    else:
        print(result)
        print('\n\n')


def _convert_file(args):
    _convert_one_file(args.input_file_or_dir, args.output)


def _convert_directory(args):
    input_dir = args.input_file_or_dir
    output_dir = args.output

    converted_successfully = []
    let_term_files = []
    failed_conversion = {}

    if output_dir is None:
        print('Cannot output to stdout when a directory is provided as input!')
        sys.exit(1)

    for (dir_path, dir_names, file_names) in os.walk(input_dir):
        for file_name in file_names:

            _, extension = os.path.splitext(file_name)
            if extension not in ['.sl', '.sygus']:
                continue

            full_input_path = os.path.join(dir_path, file_name)
            full_output_dir = os.path.join(output_dir, dir_path)
            full_output_path = os.path.join(full_output_dir, file_name)
            os.makedirs(full_output_dir, exist_ok=True)

            # noinspection PyBroadException
            try:
                print('Converting file: ' + full_input_path)
                _convert_one_file(full_input_path, full_output_path)
                converted_successfully.append(full_input_path)
            except Exception:
                f = StringIO()
                traceback.print_exc(file=f)
                stack_trace_str = f.getvalue()
                if 'Let terms in grammars are not supported.' in stack_trace_str:
                    let_term_files.append(full_input_path)
                else:
                    failed_conversion[full_input_path] = stack_trace_str

    print('---------------------------------')
    print(f'Converted {len(converted_successfully)} files successfully.')
    print(f'{len(failed_conversion)} + {len(let_term_files)} files failed conversion.')
    print(f'{len(let_term_files)} contained "let" terms in grammars.')
    print(f'True failure count: {len(failed_conversion)}')
    print('---------------------------------')
    print('\n\n')
    print('The following files had errors while converting:')
    for file_path, info in failed_conversion.items():
        print(f'{file_path}:\n{info}\n\n')


def _main():
    args = _parse_args()

    if os.path.isdir(args.input_file_or_dir):
        _convert_directory(args)
    else:
        _convert_file(args)


if __name__ == '__main__':
    _main()
