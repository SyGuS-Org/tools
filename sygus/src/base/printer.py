from abc import abstractmethod
from io import StringIO

from .. import ast
from ..resolution import CoreResolver, SymbolTable


class IndentScope(object):
    __slots__ = ['stream', 'old_indentation_level', 'new_indentation_level']

    def __init__(self, stream: 'IndentedStream', new_indentation_level):
        self.stream = stream
        self.old_indentation_level = stream.current_indentation_level
        self.new_indentation_level = new_indentation_level

    def __enter__(self):
        self.old_indentation_level = self.stream.current_indentation_level
        self.stream.current_indentation_level = self.new_indentation_level

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stream.current_indentation_level = self.old_indentation_level


class IndentedStream(object):
    __slots__ = ['buffer', 'current_indentation_level', 'index_in_current_line', 'spaces_per_indent']

    def __init__(self, spaces_per_indent=4):
        self.current_indentation_level = 0
        self.buffer = StringIO()
        self.index_in_current_line = 0
        self.spaces_per_indent = spaces_per_indent

    def _write_char(self, c: str):
        if self.index_in_current_line == 0:
            self.buffer.write(' ' * self.current_indentation_level)

        self.buffer.write(c)

        if c == '\n':
            self.index_in_current_line = 0
        else:
            self.index_in_current_line += 1

    def _write_indented(self, contents: str):
        for c in contents:
            self._write_char(c)

    def write(self, contents):
        self._write_indented(contents)

    def write_line(self, contents):
        self._write_indented(contents)
        self._write_char('\n')

    def write_raw(self, contents):
        self.buffer.write(contents)

    def push_scope(self):
        return IndentScope(self, self.current_indentation_level + self.spaces_per_indent)

    def push_scope_at_current_indent(self):
        return IndentScope(self, self.index_in_current_line)

    def push_scope_at_custom_indent(self, custom_indent_position: int):
        return IndentScope(self, custom_indent_position)

    def get_value(self):
        return self.buffer.getvalue()


class SygusASTPrinterBase(ast.ASTVisitor):
    __slots__ = ['options', 'stream', 'symbol_table']

    def visit_sort_expression(self, sort_expression: ast.SortExpression):
        if sort_expression.sort_arguments is None or len(sort_expression.sort_arguments) == 0:
            self.stream.write(str(sort_expression.identifier))
        else:
            self.stream.write(f'({sort_expression.identifier}')
            for param_sort in sort_expression.sort_arguments:
                self.stream.write(' ')
                param_sort.accept(self)
            self.stream.write(')')

    def visit_identifier_term(self, identifier_term: ast.IdentifierTerm):
        self.stream.write(str(identifier_term.identifier))

    def visit_literal_term(self, literal_term: ast.LiteralTerm):
        if literal_term.literal.literal_kind == ast.LiteralKind.STRING:
            self.stream.write(f'"{literal_term.literal.literal_value}"')
        else:
            self.stream.write(str(literal_term.literal.literal_value))

    def _write_default_function_application_term(self, function_application_term: ast.FunctionApplicationTerm):
        self.stream.write(f'({function_application_term.function_identifier}')
        for arg in function_application_term.arguments:
            self.stream.write(' ')
            arg.accept(self)
        self.stream.write(')')

    def _break_self_chained_function_application(self, identifier, arguments):
        final_term = ast.FunctionApplicationTerm(identifier, arguments[0:2], None, None)
        final_term.sort_descriptor = arguments[0].sort_descriptor
        for i in range(2, len(arguments)):
            final_term = ast.FunctionApplicationTerm(identifier, [final_term, arguments[i]], None, None)
            final_term.sort_descriptor = arguments[0].sort_descriptor
        self._write_default_function_application_term(final_term)

    def _break_bool_chained_function_application(self, identifier, arguments):
        chained_calls = []
        for i in range(len(arguments) - 1):
            subcall = ast.FunctionApplicationTerm(identifier, arguments[i:i+2], None, None)
            subcall.sort_descriptor = CoreResolver.get_boolean_sort()
            chained_calls.append(subcall)
        final_term = ast.FunctionApplicationTerm('and', chained_calls, None, None)
        final_term.sort_descriptor = CoreResolver.get_boolean_sort()
        self.visit_function_application_term(final_term)

    def _break_chained_function_application_term(self, function_application_term: ast.FunctionApplicationTerm):
        arguments = function_application_term.arguments
        identifier = function_application_term.function_identifier
        if function_application_term.sort_descriptor == arguments[0].sort_descriptor:
            self._break_self_chained_function_application(identifier, arguments)
        elif function_application_term.sort_descriptor == CoreResolver.get_boolean_sort():
            self._break_bool_chained_function_application(identifier, arguments)
        else:
            raise NotImplementedError

    def visit_function_application_term(self, function_application_term: ast.FunctionApplicationTerm):
        if not self.options.get('binarize', False):
            self._write_default_function_application_term(function_application_term)
        else:
            arg_sorts = [x.sort_descriptor for x in function_application_term.arguments]
            identifier = function_application_term.function_identifier
            func_desc = self.symbol_table.lookup_or_resolve_function(identifier, *arg_sorts)
            if func_desc.is_chainable and len(function_application_term.arguments) > 2:
                self._break_chained_function_application_term(function_application_term)
            else:
                self._write_default_function_application_term(function_application_term)

    def _write_params_and_sorts(self, parameters_and_sorts):
        first = True
        for param in parameters_and_sorts:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({param[0]} ')
            param[1].accept(self)
            self.stream.write(')')

    def visit_quantified_term(self, quantified_term: ast.QuantifiedTerm):
        if quantified_term.quantifier_kind == ast.QuantifierKind.EXISTS:
            self.stream.write('(exists (')
        else:
            self.stream.write('(forall (')
        self._write_params_and_sorts(quantified_term.quantified_variables)
        self.stream.write(') ')
        quantified_term.term_body.accept(self)
        self.stream.write(')')

    def visit_check_synth_command(self, check_synth_command: ast.CheckSynthCommand):
        self.stream.write('\n(check-synth)')

    def visit_constraint_command(self, constraint_command: ast.ConstraintCommand):
        self.stream.write('(constraint ')
        constraint_command.constraint.accept(self)
        self.stream.write(')')

    def visit_declare_var_command(self, declare_var_command: ast.DeclareVarCommand):
        self.stream.write('(declare-var ')
        self.stream.write(f'{declare_var_command.symbol} ')
        declare_var_command.sort_expression.accept(self)
        self.stream.write(')')

    def visit_inv_constraint_command(self, inv_constraint_command: ast.InvConstraintCommand):
        self.stream.write('\n(inv-constraint ')
        self.stream.write(f'{inv_constraint_command.inv_fun_symbol} ')
        self.stream.write(f'{inv_constraint_command.pre_symbol} ')
        self.stream.write(f'{inv_constraint_command.trans_symbol} ')
        self.stream.write(f'{inv_constraint_command.post_symbol}')
        self.stream.write(')')

    def visit_set_logic_command(self, set_logic_command: ast.SetLogicCommand):
        self.stream.write(f'(set-logic {set_logic_command.logic_name})\n')

    def visit_synth_fun_command(self, synth_fun_command: ast.SynthFunCommand):
        self.stream.write(f'(synth-fun {str(synth_fun_command.function_symbol)} (')
        self._write_params_and_sorts(synth_fun_command.parameters_and_sorts)
        self.stream.write(') ')
        synth_fun_command.range_sort_expression.accept(self)

        with self.stream.push_scope() as _:
            if synth_fun_command.synthesis_grammar is not None:
                self.stream.write('\n')
                synth_fun_command.synthesis_grammar.accept(self)

        self.stream.write(')\n')

    def visit_synth_inv_command(self, synth_inv_command: ast.SynthInvCommand):
        self.stream.write(f'(synth-inv {str(synth_inv_command.function_symbol)} (')
        self._write_params_and_sorts(synth_inv_command.parameters_and_sorts)
        self.stream.write(')')

        if synth_inv_command.synthesis_grammar is not None:
            self.stream.write('\n')
            synth_inv_command.synthesis_grammar.accept(self)

        self.stream.write(')\n')

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
            raise NotImplementedError

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

    def visit_define_fun_command(self, define_fun_command: ast.DefineFunCommand):
        self.stream.write(f'(define-fun {str(define_fun_command.function_name)} (')
        self._write_params_and_sorts(define_fun_command.function_parameters)
        self.stream.write(') ')
        define_fun_command.function_range_sort.accept(self)
        with self.stream.push_scope() as _:
            self.stream.write('\n')
            define_fun_command.function_body.accept(self)
        self.stream.write(')')

    def visit_program(self, program: ast.Program):
        for command in program.commands:
            command.accept(self)
            self.stream.write('\n')

    def __init__(self, name: str, symbol_table: SymbolTable, options = {}):
        super().__init__(name)
        self.options = options
        self.stream = IndentedStream()
        self.symbol_table = symbol_table
