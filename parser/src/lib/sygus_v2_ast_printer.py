from lib import ast
from . import sygus_ast_printer_base


class SygusV2ASTPrinter(sygus_ast_printer_base.SygusASTPrinterBase):
    def visit_literal_term(self, literal_term: ast.LiteralTerm):
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

        if synth_fun_command.synthesis_grammar is not None:
            self.stream.write(' ')
            synth_fun_command.synthesis_grammar.accept(self)

        self.stream.write(')')

    def visit_synth_inv_command(self, synth_inv_command: ast.SynthInvCommand):
        pass

    def visit_grammar_term(self, grammar_term: ast.GrammarTerm):
        pass

    def visit_grouped_rule_list(self, grouped_rule_list: ast.GroupedRuleList):
        pass

    def visit_grammar(self, grammar: ast.Grammar):
        pass

    def visit_declare_sort_command(self, declare_sort_command: ast.DeclareSortCommand):
        pass

    def visit_declare_fun_command(self, declare_fun_command: ast.DeclareFunCommand):
        pass

    def visit_define_sort_command(self, define_sort_command: ast.DefineSortCommand):
        pass

    def visit_datatype_constructor(self, datatype_constructor: ast.DatatypeConstructor):
        pass

    def visit_datatype_constructor_list(self, datatype_constructor_list: ast.DatatypeConstructorList):
        pass

    def visit_declare_datatypes_command(self, declare_datatypes_command: ast.DeclareDatatypesCommand):
        pass

    def visit_declare_datatype_command(self, declare_datatypes_command: ast.DeclareDatatypeCommand):
        pass

    def visit_program(self, program: ast.Program):
        pass

    def __init__(self):
        super().__init__('SygusV2ASTPrinter')
