from typing import Dict

from .. import ast
from ..base.printer import SygusASTPrinterBase
from ..resolution import SymbolTable


class SygusV2ASTPrinter(SygusASTPrinterBase):
    def visit_literal_term(self, literal_term: ast.LiteralTerm):
        if literal_term.literal.literal_kind == ast.LiteralKind.NUMERAL and literal_term.literal.literal_value < 0:
            self.stream.write(f'(- {-literal_term.literal.literal_value})')
        else:
            SygusASTPrinterBase.visit_literal_term(self, literal_term)

    def visit_let_term(self, let_term: ast.LetTerm):
        self.stream.write('(let (')
        first = True
        for binding in let_term.variable_bindings:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({binding[0]} ')
            binding[1].accept(self)
            self.stream.write(')')
        self.stream.write(') ')
        let_term.let_body.accept(self)
        self.stream.write(')')

    def visit_set_feature_command(self, set_feature_command: ast.SetFeatureCommand):
        self.stream.write('(set-feature :')
        self.stream.write(set_feature_command.feature_name)
        self.stream.write(f' {str(set_feature_command.feature_value).lower()})')

    def visit_set_option_command(self, set_option_command: ast.SetOptionCommand):
        self.stream.write('(set-option :')
        self.stream.write(set_option_command.option_name)
        self.stream.write(f' {str(set_option_command.option_value.literal_value)})')

    def visit_grammar(self, grammar: ast.Grammar):
        self.stream.write('(')
        first = True
        for nonterminal in grammar.nonterminals:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({nonterminal[0]} ')
            nonterminal[1].accept(self)
            self.stream.write(')')
        self.stream.write(')\n(')

        first = True
        for grouped_rule_list in grammar.grouped_rule_lists.values():
            if not first:
                self.stream.write('\n')
            first = False
            grouped_rule_list.accept(self)
        self.stream.write(')')

    def __init__(self, symbol_table: SymbolTable, convert_chains_to_binary: bool = False):
        super().__init__('SygusV2ASTPrinter', symbol_table, convert_chains_to_binary)

    @staticmethod
    def run(program: ast.Program, symbol_table: SymbolTable, options = {}) -> str:
        writer = SygusV2ASTPrinter(symbol_table, options)
        program.accept(writer)
        return writer.stream.get_value()
