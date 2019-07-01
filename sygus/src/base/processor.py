from .. import ast

class SygusProcessorBase(ast.ASTVisitor):
    def visit_sort_expression(self, sort_expression: ast.SortExpression):
        pass

    def visit_enum_sort_expression(self, enum_sort_expression: ast.EnumSortExpression):
        pass

    def visit_identifier_term(self, identifier_term: ast.IdentifierTerm):
        pass

    def visit_literal_term(self, literal_term: ast.LiteralTerm):
        pass

    def visit_function_application_term(self, function_application_term: ast.FunctionApplicationTerm):
        for argument in function_application_term.arguments:
            argument.accept(self)

    def visit_quantified_term(self, quantified_term: ast.QuantifiedTerm):
        for quantified_variable in quantified_term.quantified_variables:
            quantified_variable[1].accept(self)
        quantified_term.term_body.accept(self)

    def visit_let_term(self, let_term: ast.LetTerm):
        for variable_binding in let_term.variable_bindings:
            variable_binding[1].accept(self)
        let_term.let_body.accept(self)
        if let_term.type_annotations is not None:
            for type_annotation in let_term.type_annotations.values():
                type_annotation.accept(self)

    def visit_check_synth_command(self, check_synth_command: ast.CheckSynthCommand):
        pass

    def visit_constraint_command(self, constraint_command: ast.ConstraintCommand):
        constraint_command.constraint.accept(self)

    def visit_declare_var_command(self, declare_var_command: ast.DeclareVarCommand):
        declare_var_command.sort_expression.accept(self)

    def visit_declare_primed_var_command(self, declare_primed_var_command: ast.DeclarePrimedVarCommand):
        declare_primed_var_command.sort_expression.accept(self)

    def visit_inv_constraint_command(self, inv_constraint_command: ast.InvConstraintCommand):
        pass

    def visit_set_feature_command(self, set_feature_command: ast.SetFeatureCommand):
        pass

    def visit_set_option_command(self, set_option_command: ast.SetOptionCommand):
        set_option_command.option_value.accept(self)

    def visit_set_options_command(self, set_options_command: ast.SetOptionsCommand):
        pass

    def visit_set_logic_command(self, set_logic_command: ast.SetLogicCommand):
        pass

    def visit_synth_fun_command(self, synth_fun_command: ast.SynthFunCommand):
        for parameter_and_sort in synth_fun_command.parameters_and_sorts:
            parameter_and_sort[1].accept(self)
        synth_fun_command.range_sort_expression.accept(self)
        if synth_fun_command.synthesis_grammar is not None:
            synth_fun_command.synthesis_grammar.accept(self)

    def visit_synth_inv_command(self, synth_inv_command: ast.SynthInvCommand):
        for parameter_and_sort in synth_inv_command.parameters_and_sorts:
            parameter_and_sort[1].accept(self)
        if synth_inv_command.synthesis_grammar is not None:
            synth_inv_command.synthesis_grammar.accept(self)

    def visit_grammar_term(self, grammar_term: ast.GrammarTerm):
        if grammar_term.sort_expression is not None:
            grammar_term.sort_expression.accept(self)

    def visit_grouped_rule_list(self, grouped_rule_list: ast.GroupedRuleList):
        grouped_rule_list.head_symbol_sort_expression.accept(self)
        for expansion_rule in grouped_rule_list.expansion_rules:
            expansion_rule.accept(self)

    def visit_grammar(self, grammar: ast.Grammar):
        for non_terminal in grammar.nonterminals:
            non_terminal[1].accept(self)
        for grouped_rule_list in grammar.grouped_rule_lists.values():
            grouped_rule_list.accept(self)

    def visit_declare_sort_command(self, declare_sort_command: ast.DeclareSortCommand):
        pass

    def visit_define_fun_command(self, define_fun_command: ast.DefineFunCommand):
        for function_parameter in define_fun_command.function_parameters:
            function_parameter[1].accept(self)
        define_fun_command.function_range_sort.accept(self)
        define_fun_command.function_body.accept(self)

    def visit_declare_fun_command(self, declare_fun_command: ast.DeclareFunCommand):
        for sort_expression in declare_fun_command.parameter_sorts:
            sort_expression.accept(self)
        declare_fun_command.function_range_sort.accept(self)

    def visit_define_sort_command(self, define_sort_command: ast.DefineSortCommand):
        define_sort_command.sort_expression.accept(self)

    def visit_datatype_constructor(self, datatype_constructor: ast.DatatypeConstructor):
        for constructor_parameter in datatype_constructor.constructor_paramters:
            constructor_parameter[1].accept(self)

    def visit_datatype_constructor_list(self, datatype_constructor_list: ast.DatatypeConstructorList):
        for datatype_constructor in datatype_constructor_list.datatype_constructors:
            datatype_constructor.accept(self)

    def visit_declare_datatypes_command(self, declare_datatypes_command: ast.DeclareDatatypesCommand):
        for datatype_constructor_list in declare_datatypes_command.datatype_constructor_lists:
            datatype_constructor_list.accept(self)

    def visit_declare_datatype_command(self, declare_datatype_command: ast.DeclareDatatypeCommand):
        declare_datatype_command.datatype_constructor_list.accept(self)

    def visit_program(self, program: ast.Program):
        for command in program.commands:
            command.accept(self)
