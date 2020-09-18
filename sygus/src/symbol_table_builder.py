from typing import Union

from . import ast, resolution, utilities

from .exceptions import ResolutionException, UnsupportedFeatureException
from .resolution import SymbolTable, SortDescriptor, SortKind, FunctionDescriptor


class SymbolTableBuilder(ast.ASTVisitor):
    # V1 only!
    def visit_enum_sort_expression(self, enum_sort_expression: ast.EnumSortExpression):
        return SortDescriptor.create_enumerated_sort(enum_sort_expression.enum_name,
                                                     enum_sort_expression.enum_constructors)

    # V1 only!
    def visit_declare_primed_var_command(self, declare_primed_var_command: ast.DeclarePrimedVarCommand):
        symbol = declare_primed_var_command.symbol
        sort_descriptor = declare_primed_var_command.sort_expression.accept(self)
        primed_symbol = declare_primed_var_command.symbol + '!'
        symbol_desc = resolution.SymbolDescriptor(resolution.SymbolKind.UNIVERSAL_VARIABLE,
                                                  symbol, sort_descriptor)
        primed_desc = resolution.SymbolDescriptor(resolution.SymbolKind.UNIVERSAL_VARIABLE,
                                                  primed_symbol, sort_descriptor)
        self.symbol_table.add_symbol(symbol_desc)
        self.symbol_table.add_symbol(primed_desc)

    # V1 only!
    def visit_set_options_command(self, set_options_command: ast.SetOptionsCommand):
        pass

    # V1 only!
    def visit_declare_fun_command(self, declare_fun_command: ast.DeclareFunCommand):
        func_descriptor = \
            resolution.FunctionDescriptor.create_uninterpreted_function(
                declare_fun_command.function_name,
                [param.accept(self) for param in declare_fun_command.parameter_sorts],
                declare_fun_command.function_range_sort.accept(self))
        self.symbol_table.add_function(func_descriptor)

    def visit_sort_expression(self, sort_expression: ast.SortExpression) -> SortDescriptor:
        sort_descriptor = self.symbol_table.lookup_or_resolve_sort(sort_expression.identifier)
        if sort_descriptor is None:
            raise ResolutionException('Could not resolve identifier: ' + str(sort_expression.identifier),
                                      sort_expression.identifier.start_location,
                                      sort_expression.identifier.end_location)

        if len(sort_expression.sort_arguments) != sort_descriptor.num_parameters:
            raise ResolutionException(f'Sort "{sort_descriptor.identifier}" requires ' +
                                      f'{sort_descriptor.num_parameters} arguments, but was instantiated ' +
                                      f'with {len(sort_expression.sort_arguments)}.',
                                      sort_expression.identifier.start_location,
                                      sort_expression.identifier.end_location)

        if sort_descriptor.num_parameters == 0:
            return sort_descriptor

        sort_argument_descriptors = [x.accept(self) for x in sort_expression.sort_arguments]
        return sort_descriptor.instantiate(sort_argument_descriptors)

    def visit_identifier_term(self, identifier_term: ast.IdentifierTerm):
        symbol_desc = self.symbol_table.lookup_symbol(identifier_term.identifier)
        if symbol_desc is None:
            # try resolving it as a constant function
            func_desc = self.symbol_table.lookup_function(identifier_term.identifier)

            if func_desc is None or len(func_desc.argument_sorts) > 0:
                raise ResolutionException(f'Could not resolve identifier: {identifier_term.identifier}.',
                                          identifier_term.start_location, identifier_term.end_location)
            identifier_term.sort_descriptor = func_desc.range_sort
        else:
            identifier_term.sort_descriptor = symbol_desc.symbol_sort

    def visit_literal_term(self, literal_term: ast.LiteralTerm):
        literal = literal_term.literal
        kind = literal.literal_kind
        value = literal.literal_value

        if kind == ast.LiteralKind.NUMERAL:
            literal_term.sort_descriptor = resolution.IntegerResolver.get_integer_sort()
        elif kind == ast.LiteralKind.DECIMAL:
            literal_term.sort_descriptor = resolution.RealResolver.get_real_sort()
        elif kind == ast.LiteralKind.BOOLEAN:
            literal_term.sort_descriptor = resolution.CoreResolver.get_boolean_sort()
        elif kind == ast.LiteralKind.HEXADECIMAL:
            size = (len(value) - 2) * 4
            literal_term.sort_descriptor = resolution.BitVectorResolver.get_bit_vector_sort(size)
        elif kind == ast.LiteralKind.BINARY:
            size = len(value) - 2
            literal_term.sort_descriptor = resolution.BitVectorResolver.get_bit_vector_sort(size)
        elif kind == ast.LiteralKind.STRING:
            literal_term.sort_descriptor = resolution.StringResolver.get_string_sort()
        else:
            raise NotImplementedError

    def visit_function_application_term(self, function_application_term: ast.FunctionApplicationTerm):
        # first recurse on the children
        for arg in function_application_term.arguments:
            arg.accept(self)

        arg_sorts = [x.sort_descriptor for x in function_application_term.arguments]
        identifier = function_application_term.function_identifier
        func_desc = self.symbol_table.lookup_or_resolve_function(identifier, *arg_sorts)

        if func_desc is None:
            raise ResolutionException(f'Could not resolve function application {identifier}.',
                                      function_application_term.start_location, function_application_term.end_location)

        range_sort = func_desc.can_apply(arg_sorts)
        if range_sort is None:
            raise TypeError(f'Identifier {identifier} cannot be applied in this context.\n' +
                            f'At {function_application_term.start_location} -- ' +
                            f'{function_application_term.end_location}')

        function_application_term.sort_descriptor = range_sort

    def visit_quantified_term(self, quantified_term: ast.QuantifiedTerm):
        quantified_vars = quantified_term.quantified_variables
        quantified_var_names = [x[0] for x in quantified_vars]
        quantified_var_sorts = [x[1].accept(self) for x in quantified_vars]

        self.symbol_table.push_scope()
        for i in range(len(quantified_var_names)):
            symbol_desc = resolution.SymbolDescriptor(resolution.SymbolKind.QUANTIFIED_VARIABLE,
                                                      quantified_var_names[i], quantified_var_sorts[i])
            self.symbol_table.add_symbol(symbol_desc)

        quantified_term.term_body.accept(self)
        scope = self.symbol_table.pop_scope()
        quantified_term.symbol_table_scope = scope

        if quantified_term.term_body.sort_descriptor != resolution.CoreResolver.get_boolean_sort():
            raise TypeError('Quantified terms must be Boolean sorted.\n' +
                            f'At: {quantified_term.start_location} -- {quantified_term.end_location}')

        quantified_term.sort_descriptor = quantified_term.term_body.sort_descriptor

    def visit_let_term(self, let_term: ast.LetTerm):
        bound_var_sorts = {}
        bound_var_descs = []
        for binding in let_term.variable_bindings:
            bound_var_name = binding[0]
            bound_var_term = binding[1]
            bound_var_term.accept(self)
            bound_var_sort = bound_var_term.sort_descriptor
            bound_var_sorts[bound_var_name] = bound_var_sort
            bound_var_descs.append(resolution.SymbolDescriptor(resolution.SymbolKind.LET_BOUND_VARIABLE,
                                                               bound_var_name, bound_var_sort))

        self.symbol_table.push_scope()
        for bound_var_desc in bound_var_descs:
            self.symbol_table.add_symbol(bound_var_desc)

        let_term.let_body.accept(self)
        let_term.sort_descriptor = let_term.let_body.sort_descriptor
        let_term.symbol_table_scope = self.symbol_table.pop_scope()

    def visit_check_synth_command(self, check_synth_command: ast.CheckSynthCommand):
        pass

    def visit_constraint_command(self, constraint_command: ast.ConstraintCommand):
        constraint_command.constraint.accept(self)
        if constraint_command.constraint.sort_descriptor != resolution.CoreResolver.get_boolean_sort():
            raise TypeError('Constraints must be Boolean sorted.\n' +
                            f'At: {constraint_command.start_location} -- {constraint_command.end_location}')

    def visit_declare_var_command(self, declare_var_command: ast.DeclareVarCommand):
        if self.symbol_table.lookup_symbol(declare_var_command.symbol) is not None:
            raise ValueError(f'Redeclaration of symbol: {declare_var_command.symbol}.\n' +
                             f'At: {declare_var_command.start_location} -- {declare_var_command.end_location}')
        symbol_desc = resolution.SymbolDescriptor(resolution.SymbolKind.UNIVERSAL_VARIABLE,
                                                  declare_var_command.symbol,
                                                  declare_var_command.sort_expression.accept(self))
        self.symbol_table.add_symbol(symbol_desc)

    def visit_inv_constraint_command(self, inv_constraint_command: ast.InvConstraintCommand):
        inv_fun_id = utilities.canonicalize_identifier(inv_constraint_command.inv_fun_symbol)
        pre_id = utilities.canonicalize_identifier(inv_constraint_command.pre_symbol)
        trans_id = utilities.canonicalize_identifier(inv_constraint_command.trans_symbol)
        post_id = utilities.canonicalize_identifier(inv_constraint_command.post_symbol)

        inv_fd = self.symbol_table.inv_functions.get(inv_fun_id, None)
        pre_fd = self.symbol_table.user_defined_functions.get(pre_id, None)
        trans_fd = self.symbol_table.user_defined_functions.get(trans_id, None)
        post_fd = self.symbol_table.user_defined_functions.get(post_id, None)

        if inv_fd is None:
            raise ResolutionException(f'Could not resolve identifier: {inv_fun_id}.',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)
        if pre_fd is None:
            raise ResolutionException(f'Could not resolve identifier: {pre_id}.',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)

        if trans_fd is None:
            raise ResolutionException(f'Could not resolve identifier: {trans_id}.',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)

        if post_fd is None:
            raise ResolutionException(f'Could not resolve identifier: {post_id}.',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)

        if inv_fd.function_kind != resolution.FunctionKind.SYNTH_INV:
            raise ResolutionException(f'{inv_fun_id} does not refer to an invariant to be synthesized.',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)

        if (pre_fd.function_kind != resolution.FunctionKind.USER_DEFINED or
                pre_fd.range_sort != resolution.CoreResolver.get_boolean_sort()):
            raise ResolutionException(f'{pre_id} does not refer to a user-defined Boolean-valued function',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)

        if (trans_fd.function_kind != resolution.FunctionKind.USER_DEFINED or
                trans_fd.range_sort != resolution.CoreResolver.get_boolean_sort()):
            raise ResolutionException(f'{trans_id} does not refer to a user-defined Boolean-valued function',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)

        if (post_fd.function_kind != resolution.FunctionKind.USER_DEFINED or
                post_fd.range_sort != resolution.CoreResolver.get_boolean_sort()):
            raise ResolutionException(f'{post_id} does not refer to a user-defined Boolean-valued function',
                                      inv_constraint_command.start_location, inv_constraint_command.end_location)

        # The argument sorts must match
        inv_arg_sorts = inv_fd.argument_sorts
        if utilities.first_discrepancy(inv_arg_sorts, pre_fd.argument_sorts) is not None:
            raise TypeError(f'The signatures of {inv_fun_id} and {pre_id} must match.\n' +
                            f'At: {inv_constraint_command.start_location} -- {inv_constraint_command.end_location}')

        if utilities.first_discrepancy(inv_arg_sorts, post_fd.argument_sorts) is not None:
            raise TypeError(f'The signatures of {inv_fun_id} and {post_id} must match.\n' +
                            f'At: {inv_constraint_command.start_location} -- {inv_constraint_command.end_location}')

        doubled_inv_arg_sorts = inv_arg_sorts + inv_arg_sorts
        if utilities.first_discrepancy(doubled_inv_arg_sorts, trans_fd.argument_sorts) is not None:
            raise TypeError(f'{trans_id} must take the same arguments as {inv_fun_id}, but twice.\n' +
                            f'At: {inv_constraint_command.start_location} -- {inv_constraint_command.end_location}')

    def visit_set_feature_command(self, set_feature_command: ast.SetFeatureCommand):
        if set_feature_command.feature_value:
            self.symbol_table.enable_feature(set_feature_command.feature_name)
        else:
            self.symbol_table.disable_feature(set_feature_command.feature_name)

    def visit_set_info_command(self, set_info_command: ast.SetInfoCommand):
        self.symbol_table.set_info(set_info_command.info_name,
                                   set_info_command.info_value)

    def visit_set_option_command(self, set_option_command: ast.SetOptionCommand):
        self.symbol_table.set_option(set_option_command.option_name,
                                     set_option_command.option_value)

    def visit_set_logic_command(self, set_logic_command: ast.SetLogicCommand):
        if self.symbol_table.logic_name != '':
            raise ValueError('At most one set-logic command is allowed.')
        self.symbol_table.set_logic(set_logic_command.logic_name)

    def visit_synth_fun_command(self, synth_fun_command: ast.SynthFunCommand):
        param_sort_descs = [(x[0], x[1].accept(self)) for x in synth_fun_command.parameters_and_sorts]
        range_sort_desc = synth_fun_command.range_sort_expression.accept(self)
        identifier = synth_fun_command.function_symbol

        func_desc = FunctionDescriptor.create_synthesis_objective(identifier, param_sort_descs,
                                                                  range_sort_desc, resolution.FunctionKind.SYNTH_FUN,
                                                                  synth_fun_command.synthesis_grammar)
        self.symbol_table.add_function(func_desc)

        if synth_fun_command.synthesis_grammar is None:
            return

        # check that the grammar sort matches the function range sort
        self.symbol_table.push_scope()
        for param_name, param_sort in param_sort_descs:
            sym_desc = resolution.SymbolDescriptor(resolution.SymbolKind.PARAMETER, param_name, param_sort)
            self.symbol_table.add_symbol(sym_desc)

        synth_fun_command.synthesis_grammar.accept(self)

        synth_fun_command.symbol_table_scope = self.symbol_table.pop_scope()

        self.symbol_table.push_scope(synth_fun_command.synthesis_grammar.symbol_table_scope)
        head_desc = self.symbol_table.lookup_symbol(synth_fun_command.synthesis_grammar.nonterminals[0][0])

        if head_desc.symbol_sort != range_sort_desc:
            raise TypeError('Grammar sort does not match sort of function to be synthesized.\n' +
                            f'At: {synth_fun_command.start_location} -- {synth_fun_command.end_location}')

    def visit_synth_inv_command(self, synth_inv_command: ast.SynthInvCommand):
        param_sort_descs = [(x[0], x[1].accept(self)) for x in synth_inv_command.parameters_and_sorts]
        range_sort_desc = resolution.CoreResolver.get_boolean_sort()
        identifier = synth_inv_command.function_symbol
        func_desc = FunctionDescriptor.create_synthesis_objective(identifier, param_sort_descs,
                                                                  range_sort_desc, resolution.FunctionKind.SYNTH_INV,
                                                                  synth_inv_command.synthesis_grammar)
        self.symbol_table.add_function(func_desc)

        if synth_inv_command.synthesis_grammar is None:
            return

        # check that the grammar sort matches the function range sort
        synth_inv_command.synthesis_grammar.accept(self)
        self.symbol_table.push_scope(synth_inv_command.synthesis_grammar.symbol_table_scope)
        head_desc = self.symbol_table.lookup_symbol(synth_inv_command.synthesis_grammar.nonterminals[0][0])

        if head_desc.symbol_sort != range_sort_desc:
            raise TypeError('Grammar sort does not match sort of function to be synthesized.\n' +
                            f'At: {synth_inv_command.start_location} -- {synth_inv_command.end_location}')

    def visit_grammar_term(self, grammar_term: ast.GrammarTerm):
        if grammar_term.grammar_term_kind == ast.GrammarTermKind.CONSTANT:
            grammar_term.sort_descriptor = grammar_term.sort_expression.accept(self)
        elif grammar_term.grammar_term_kind == ast.GrammarTermKind.VARIABLE:
            grammar_term.sort_descriptor = grammar_term.sort_expression.accept(self)
        else:
            grammar_term.binder_free_term.accept(self)
            grammar_term.sort_descriptor = grammar_term.binder_free_term.sort_descriptor

    def visit_grouped_rule_list(self, grouped_rule_list: ast.GroupedRuleList):
        grouped_rule_list.head_symbol_sort_descriptor = grouped_rule_list.head_symbol_sort_expression.accept(self)
        expected_type = grouped_rule_list.head_symbol_sort_descriptor

        for expansion in grouped_rule_list.expansion_rules:
            expansion.accept(self)
            if expansion.sort_descriptor != grouped_rule_list.head_symbol_sort_descriptor:
                raise TypeError(f'Expected a term of type: {expected_type.identifier} ' +
                                f'but was {expansion.sort_descriptor.identifier}.\n' +
                                f'At {expansion.start_location} -- {expansion.end_location}')

    def visit_grammar(self, grammar: ast.Grammar):
        nonterminal_symbols = {}
        for nonterminal in grammar.nonterminals:
            nt_desc = resolution.SymbolDescriptor(resolution.SymbolKind.GRAMMAR_NONTERMINAL,
                                                  nonterminal[0], nonterminal[1].accept(self))
            nonterminal_symbols[nonterminal[0]] = nt_desc

        self.symbol_table.push_scope()
        for nonterminal_symbol in nonterminal_symbols.values():
            self.symbol_table.add_symbol(nonterminal_symbol)

        for _, expansion in grammar.grouped_rule_lists.items():
            expansion.accept(self)
            if expansion.head_symbol_sort_descriptor != nonterminal_symbols[expansion.head_symbol].symbol_sort:
                raise TypeError('Actual type of expansion does not match declared type.\n' +
                                f'At: {expansion.start_location} -- {expansion.end_location}')

        grammar.symbol_table_scope = self.symbol_table.pop_scope()

    def visit_declare_sort_command(self, declare_sort_command: ast.DeclareSortCommand):
        sort_desc = SortDescriptor.create_sort(declare_sort_command.sort_name, declare_sort_command.sort_arity,
                                               SortKind.UNINTERPRETED)
        self.symbol_table.add_sort(sort_desc)

    def visit_define_fun_command(self, define_fun_command: ast.DefineFunCommand):
        arg_names = [x[0] for x in define_fun_command.function_parameters]
        arg_sorts = [x[1].accept(self) for x in define_fun_command.function_parameters]
        range_sort = define_fun_command.function_range_sort.accept(self)
        func_name = define_fun_command.function_name

        self.symbol_table.push_scope()
        for i in range(len(arg_names)):
            arg_desc = resolution.SymbolDescriptor(resolution.SymbolKind.PARAMETER, arg_names[i], arg_sorts[i])
            self.symbol_table.add_symbol(arg_desc)

        func_body = define_fun_command.function_body
        func_body.accept(self)

        scope = self.symbol_table.pop_scope()
        func_body.symbol_table_scope = scope

        if func_body.sort_descriptor != range_sort:
            raise TypeError('Function body does is not of the specified sort.\n' +
                            f'At: {func_body.start_location} -- {func_body.end_location}')

        func_desc = FunctionDescriptor.create_user_defined_function(func_name, list(zip(arg_names, arg_sorts)),
                                                                    range_sort, func_body)
        self.symbol_table.add_function(func_desc)

    def visit_define_sort_command(self, define_sort_command: ast.DefineSortCommand):
        alias_name = define_sort_command.sort_name
        introduced_placeholders = [SortDescriptor.create_placeholder(x) for x in define_sort_command.sort_parameters]
        alias_target = define_sort_command.sort_expression
        self.symbol_table.push_scope()
        for placeholder in introduced_placeholders:
            self.symbol_table.add_sort(placeholder)

        alias_target_descriptor = alias_target.accept(self)
        self.symbol_table.pop_scope()
        sort_desc = SortDescriptor.create_alias(alias_name, introduced_placeholders, alias_target_descriptor)
        self.symbol_table.add_sort(sort_desc)

    def visit_datatype_constructor(self, datatype_constructor: ast.DatatypeConstructor):
        # Should not be reachable
        raise NotImplementedError

    def visit_datatype_constructor_list(self, datatype_constructor_list: ast.DatatypeConstructorList):
        if len(datatype_constructor_list.introduced_sort_placeholders) != 0:
            raise UnsupportedFeatureException('Parametric datatypes are not (yet) supported.',
                                              datatype_constructor_list.start_location,
                                              datatype_constructor_list.end_location)

        constructor_range_type = self.current_datatype_sort

        for constructor in datatype_constructor_list.constructors:
            name = constructor.constructor_name
            params = constructor.constructor_parameters
            param_names = [x[0] for x in params]
            param_sort_descs = [x[1].accept(self) for x in params]
            resolved_params = list(zip(param_names, param_sort_descs))

            constructor, tester, selectors = \
                FunctionDescriptor.create_datatype_constructor_and_helpers(name, resolved_params,
                                                                           constructor_range_type)
            self.symbol_table.add_function(constructor)
            self.symbol_table.add_function(tester)
            for selector in selectors:
                self.symbol_table.add_function(selector)

    def visit_declare_datatypes_command(self, declare_datatypes_command: ast.DeclareDatatypesCommand):
        sort_names_and_arities = declare_datatypes_command.sort_names_and_arities
        if any(x[1] != 0 for x in sort_names_and_arities):
            raise UnsupportedFeatureException('Parametric datatypes are not (yet) supported',
                                              declare_datatypes_command.start_location,
                                              declare_datatypes_command.end_location)

        sort_descriptors = [SortDescriptor.create_sort(x[0], 0, SortKind.DATATYPE) for x in sort_names_and_arities]

        for sort_descriptor in sort_descriptors:
            self.symbol_table.add_sort(sort_descriptor)

        for i in range(len(declare_datatypes_command.datatype_constructor_lists)):
            self.current_datatype_sort = sort_descriptors[i]
            declare_datatypes_command.datatype_constructor_lists[i].accept(self)

    def visit_declare_datatype_command(self, declare_datatype_command: ast.DeclareDatatypeCommand):
        if declare_datatype_command.sort_arity != 0:
            raise UnsupportedFeatureException('Parametric datatypes are not (yet) supported',
                                              declare_datatype_command.start_location,
                                              declare_datatype_command.end_location)

        sort_desc = SortDescriptor.create_sort(declare_datatype_command.sort_name,
                                               declare_datatype_command.sort_arity,
                                               SortKind.DATATYPE)
        self.symbol_table.add_sort(sort_desc)
        self.current_datatype_sort = sort_desc
        declare_datatype_command.datatype_constructor_list.accept(self)

    def visit_program(self, program: ast.Program):
        for command in program.commands:
            command.accept(self)

    def __init__(self):
        super().__init__("SymbolTableBuilder")
        self.symbol_table: SymbolTable = SymbolTable()
        self.current_datatype_sort: Union[SortDescriptor, None] = None

    @staticmethod
    def run(program: ast.Program):
        builder = SymbolTableBuilder()
        program.accept(builder)
        return builder.symbol_table
