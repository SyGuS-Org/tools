from .. import ast
from ..base.printer import SygusASTPrinterBase
from ..resolution import SymbolTable


class SygusV1ASTPrinter(SygusASTPrinterBase):
    def visit_sort_expression(self, sort_expression: ast.SortExpression):
        if sort_expression.identifier.is_indexed and sort_expression.identifier.symbol == 'BitVec':
            self.stream.write(f'(BitVec {sort_expression.identifier.indices[0]})')
        else:
            SygusASTPrinterBase.visit_sort_expression(self, sort_expression)

    def visit_let_term(self, let_term: ast.LetTerm):
        self.stream.write('(let (')
        first = True
        for binding in let_term.variable_bindings:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({binding[0]} ')
            let_term.type_annotations[binding[0]].accept(self)
            self.stream.write(' ')
            binding[1].accept(self)
            self.stream.write(')')
        self.stream.write(') ')
        let_term.let_body.accept(self)
        self.stream.write(')')

    def visit_declare_primed_var_command(self, declare_primed_var_command: ast.DeclarePrimedVarCommand):
        self.stream.write('(declare-primed-var ')
        self.stream.write(f'{declare_primed_var_command.symbol} ')
        declare_primed_var_command.sort_expression.accept(self)
        self.stream.write(')')

    def visit_set_options_command(self, set_options_command: ast.SetOptionsCommand):
        self.stream.write('(set-options (')
        for option_name_and_value in set_options_command.option_names_and_values:
            self.stream.write(f'({option_name_and_value[0]} {option_name_and_value[1]})')
        self.stream.write('))')

    def visit_grammar(self, grammar: ast.Grammar):
        self.stream.write('(')
        first = True
        for grouped_rule_list in grammar.grouped_rule_lists.values():
            if not first:
                self.stream.write('\n')
            first = False
            grouped_rule_list.accept(self)
        self.stream.write(')')

    def __init__(self, symbol_table: SymbolTable, convert_chains_to_binary: bool = False):
        super().__init__('SygusV1ASTPrinter', symbol_table, convert_chains_to_binary)

    @staticmethod
    def run(program: ast.Program, symbol_table: SymbolTable, convert_chains_to_binary: bool = False) -> str:
        writer = SygusV1ASTPrinter(symbol_table, convert_chains_to_binary)
        program.accept(writer)
        return writer.stream.get_value()
