from lib import ast
from . import sygus_ast_printer_base


class SygusV2ASTPrinter(sygus_ast_printer_base.SygusASTPrinterBase):
    def visit_literal_term(self, literal_term: ast.LiteralTerm):
        if literal_term.literal.literal_kind == ast.LiteralKind.NUMERAL and literal_term.literal.literal_value < 0:
            self.stream.write(f'(- {-literal_term.literal.literal_value})')
        elif literal_term.literal.literal_kind == ast.LiteralKind.STRING:
            self.stream.write(f'"{literal_term.literal.literal_value}"')
        else:
            self.stream.write(str(literal_term.literal.literal_value))

    def visit_enum_sort_expression(self, enum_sort_expression: ast.EnumSortExpression):
        raise NotImplemented

    def visit_quantified_term(self, quantified_term: ast.QuantifiedTerm):
        if quantified_term.quantifier_kind == ast.QuantifierKind.EXISTS:
            self.stream.write('(exists (')
        else:
            self.stream.write('(forall (')
        first = True
        for binding in quantified_term.quantified_variables:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({binding[0]} ')
            binding[1].accept(self)
            self.stream.write(')')
        self.stream.write(') ')
        quantified_term.term_body.accept(self)
        self.stream.write(')')

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

    def visit_declare_primed_var_command(self, declare_primed_var_command: ast.DeclarePrimedVarCommand):
        pass

    def visit_set_feature_command(self, set_feature_command: ast.SetFeatureCommand):
        self.stream.write('(set-feature : ')
        self.stream.write(set_feature_command.feature_name)
        self.stream.write(f' {str(set_feature_command.feature_value).lower()})')

    def visit_set_option_command(self, set_option_command: ast.SetOptionCommand):
        self.stream.write('(set-option : ')
        self.stream.write(set_option_command.option_name)
        self.stream.write(f' {str(set_option_command.option_value.literal_value)})')

    def visit_set_options_command(self, set_options_command: ast.SetOptionCommand):
        raise NotImplemented

    def visit_synth_fun_command(self, synth_fun_command: ast.SynthFunCommand):
        self.stream.write(f'(synth-fun {str(synth_fun_command.function_symbol)} (')
        first = True
        for param in synth_fun_command.parameters_and_sorts:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({param[0]} ')
            param[1].accept(self)
            self.stream.write(')')
        self.stream.write(') ')
        synth_fun_command.range_sort_expression.accept(self)

        with self.stream.push_scope() as _:
            if synth_fun_command.synthesis_grammar is not None:
                self.stream.write('\n')
                synth_fun_command.synthesis_grammar.accept(self)

        self.stream.write(')')

    def visit_synth_inv_command(self, synth_inv_command: ast.SynthInvCommand):
        self.stream.write(f'(synth-inv {str(synth_inv_command.function_symbol)} (')
        first = True
        for param in synth_inv_command.parameters_and_sorts:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({param[0]} ')
            param[1].accept(self)
            self.stream.write(')')
        self.stream.write(') ')

        if synth_inv_command.synthesis_grammar is not None:
            self.stream.write(' ')
            synth_inv_command.synthesis_grammar.accept(self)

        self.stream.write(')')

    def visit_grammar_term(self, grammar_term: ast.GrammarTerm):
        if grammar_term.grammar_term_kind == ast.GrammarTermKind.CONSTANT:
            self.stream.write('(Constant ')
            grammar_term.sort_expression.accept(self)
            self.stream.write(')')
        elif grammar_term.grammar_term_kind == ast.GrammarTermKind.VARIABLE:
            self.stream.write('(Variable ')
            grammar_term.sort_expression.accept(self)
            self.stream.write(')')
        elif grammar_term.grammar_term_kind == ast.GrammarTermKind.BINDER_FREE:
            grammar_term.binder_free_term.accept(self)
        else:
            raise NotImplemented

    def visit_grouped_rule_list(self, grouped_rule_list: ast.GroupedRuleList):
        self.stream.write(f'({grouped_rule_list.head_symbol} ')
        grouped_rule_list.head_symbol_sort_expression.accept(self)
        self.stream.write(' (')
        first = True
        for expansion in grouped_rule_list.expansion_rules:
            if not first:
                self.stream.write(' ')
            first = False
            expansion.accept(self)
        self.stream.write('))')

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

    def visit_declare_sort_command(self, declare_sort_command: ast.DeclareSortCommand):
        raise NotImplemented

    def visit_declare_fun_command(self, declare_fun_command: ast.DeclareFunCommand):
        raise NotImplemented

    def visit_define_sort_command(self, define_sort_command: ast.DefineSortCommand):
        raise NotImplemented

    def visit_datatype_constructor(self, datatype_constructor: ast.DatatypeConstructor):
        raise NotImplemented

    def visit_datatype_constructor_list(self, datatype_constructor_list: ast.DatatypeConstructorList):
        raise NotImplemented

    def visit_declare_datatypes_command(self, declare_datatypes_command: ast.DeclareDatatypesCommand):
        raise NotImplemented

    def visit_declare_datatype_command(self, declare_datatypes_command: ast.DeclareDatatypeCommand):
        raise NotImplemented

    def visit_program(self, program: ast.Program):
        for command in program.commands:
            command.accept(self)
            self.stream.write('\n')

    def __init__(self):
        super().__init__('SygusV2ASTPrinter')

    @staticmethod
    def run(program: ast.Program) -> str:
        writer = SygusV2ASTPrinter()
        program.accept(writer)
        return writer.stream.get_value()
