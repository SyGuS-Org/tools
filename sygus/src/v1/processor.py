from .. import ast
from ..resolution import SymbolTable
from ..base.processor import SygusProcessorBase

class SygusV1Processor(SygusProcessorBase):
    __slots__ = ['new_decls', 'symbol_table']

    def visit_quantified_term(self, quantified_term: ast.QuantifiedTerm):
        raise NotImplementedError

    def visit_set_feature_command(self, set_feature_command: ast.SetFeatureCommand):
        raise NotImplementedError

    def visit_set_option_command(self, set_option_command: ast.SetOptionCommand):
        raise NotImplementedError

    def visit_declare_sort_command(self, declare_sort_command: ast.DeclareSortCommand):
        raise NotImplementedError

    def visit_datatype_constructor(self, datatype_constructor: ast.DatatypeConstructor):
        raise NotImplementedError

    def visit_datatype_constructor_list(self, datatype_constructor_list: ast.DatatypeConstructorList):
        raise NotImplementedError

    def visit_declare_datatypes_command(self, declare_datatypes_command: ast.DeclareDatatypesCommand):
        raise NotImplementedError

    def visit_declare_datatype_command(self, declare_datatypes_command: ast.DeclareDatatypeCommand):
        raise NotImplementedError

    def visit_let_term(self, let_term: ast.LetTerm):
        let_term.type_annotations = dict()
        for binding in let_term.variable_bindings:
            binding_sort = binding[1].sort_descriptor
            sort_expression = ast.SortExpression(binding_sort.identifier, binding_sort.sort_arguments, None, None)
            let_term.type_annotations[binding[0]] = sort_expression
        SygusProcessorBase.visit_let_term(self, let_term)

    def visit_grammar(self, grammar: ast.Grammar):
        if grammar.nonterminals[0][0] != 'Start':
            grammar_term = ast.GrammarTerm.create_binder_free_grammar_term( ast.IdentifierTerm(grammar.nonterminals[0][0], None, None), None, None)
            grammar.grouped_rule_lists['Start'] = ast.GroupedRuleList('Start', grammar.nonterminals[0][1], [ grammar_term ], None, None)
            grammar.nonterminals = [ ('Start', grammar.nonterminals[0][1]) ] + grammar.nonterminals
        SygusProcessorBase.visit_grammar(self, grammar)

    def visit_inv_constraint_command(self, inv_constraint_command: ast.InvConstraintCommand):
        SygusProcessorBase.visit_inv_constraint_command(self, inv_constraint_command)
        trans_function = self.symbol_table.lookup_function(inv_constraint_command.trans_symbol)
        for argument_name, argument_sort in zip(trans_function.argument_names, trans_function.argument_sorts):
            if argument_name[-1] == '!':
                base_argument_name = argument_name[:-1]
                sort_expression = ast.SortExpression(argument_sort.identifier, argument_sort.sort_arguments, None, None)
                if self.symbol_table.lookup_symbol(base_argument_name) is None \
                    and self.symbol_table.lookup_symbol(base_argument_name) is None:
                    self.new_decls.extend([
                        ast.DeclarePrimedVarCommand(base_argument_name, sort_expression, None, None)
                    ])
                elif self.symbol_table.lookup_symbol(base_argument_name) is None:
                    self.new_decls.extend([
                        ast.DeclareVarCommand(base_argument_name, sort_expression, None, None)
                    ])
                elif self.symbol_table.lookup_symbol(argument_name) is None:
                    self.new_decls.append(
                        ast.DeclareVarCommand(argument_name, sort_expression, None, None)
                    )

    def visit_program(self, program: ast.Program):
        SygusProcessorBase.visit_program(self, program)
        for decl in self.new_decls:
            decl.accept(self)
        program.commands = [ program.commands[0] ] + self.new_decls + program.commands[1:]

    def __init__(self, symbol_table: SymbolTable):
        super().__init__('SygusV1Processor')
        self.new_decls = []
        self.symbol_table = symbol_table

    @staticmethod
    def run(program: ast.Program, symbol_table: SymbolTable) -> None:
        processor = SygusV1Processor(symbol_table)
        program.accept(processor)
