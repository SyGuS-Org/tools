#!/usr/bin/env python3

import argparse
import os
import traceback

from io import StringIO

from .lib.v1.parser import SygusV1Parser
from .lib.symbol_table_builder import SymbolTableBuilder


def _parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file_or_directory')
    return parser.parse_args()


def _process_one_file(input_path):
    print(f'Processing file: {input_path}...')
    with open(input_path, 'r') as f:
        contents = f.read()

    parser = SygusV1Parser()
    program = parser.parse(contents)

    if program is None:
        raise SyntaxError()

    SymbolTableBuilder.run(program)


def _main():
    args = _parse_args()
    failed_files = []
    if os.path.isdir(args.input_file_or_directory):
        file_count = 0
        for dir_path, _, file_names in os.walk(args.input_file_or_directory):
            for file_name in file_names:
                _, extension = os.path.splitext(file_name)
                if extension not in ['.sl', '.sygus']:
                    continue

                file_count += 1
                full_path = os.path.join(dir_path, file_name)

                # noinspection PyBroadException
                try:
                    _process_one_file(full_path)
                except Exception:
                    f = StringIO()
                    traceback.print_exc(file=f)
                    stack_trace_str = f.getvalue()
                    failed_files.append((full_path, stack_trace_str))

        if len(failed_files) > 0:
            print(f'Encountered {len(failed_files)} failures out of {file_count} files processed:')
            for path, stack_trace in failed_files:
                print(f'{path}:')
                print(stack_trace)
                print('-----------------------------------------')
        else:
            print(f'Processed {file_count} files without any failures!')
    else:
        _process_one_file(args.input_file_or_directory)


if __name__ == '__main__':
    _main()
