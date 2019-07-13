#!/usr/bin/env python3

from argparse import ArgumentDefaultsHelpFormatter, ArgumentParser, FileType

from ..src.base.sizer import SygusSizerBase


def main(args):
    if args.sygus_standard == '1':
        from ..src.v1.parser import SygusV1Parser
        parser = SygusV1Parser()
    elif args.sygus_standard == '2':
        from ..src.v2.parser import SygusV2Parser
        parser = SygusV2Parser()
    else:
        raise NotImplementedError

    functions = parser.parse(args.input_file.read())
    print(SygusSizerBase.run(functions))


if __name__ == '__main__':
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '-s', '--sygus-standard', default='2', choices=['1','2'],
        help='The SyGuS language standard used in the input file')

    parser.add_argument(
        'input_file', type=FileType('r'),
        help='Path to an input file (or stdin if "-")')

    main(parser.parse_args())
