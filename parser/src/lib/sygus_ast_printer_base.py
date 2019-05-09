from io import StringIO
from abc import abstractmethod

from . import ast


class IndentScope(object):
    __slots__ = ('stream', 'old_indentation_level', 'new_indentation_level')

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
    __slots__ = ('buffer', 'current_indentation_level', 'index_in_current_line', 'spaces_per_indent')

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
    def __init__(self, name: str):
        super().__init__(name)
        self.stream = IndentedStream()

    def visit_sort_expression(self, sort_expression: ast.SortExpression):
        if sort_expression.sort_arguments is None or len(sort_expression.sort_arguments) == 0:
            self.stream.write(str(sort_expression.identifier))
        else:
            self.stream.write(f'({str(sort_expression.identifier)}')
            for param_sort in sort_expression.sort_arguments:
                self.stream.write(' ')
                param_sort.accept(self)
            self.stream.write(')')

    @abstractmethod
    def visit_enum_sort_expression(self, enum_sort_expression: ast.EnumSortExpression):
        raise NotImplemented

    def visit_identifier_term(self, identifier_term: ast.IdentifierTerm):
        self.stream.write(str(identifier_term.identifier))

    @abstractmethod
    def visit_literal_term(self, literal_term: ast.LiteralTerm):
        raise NotImplemented

    def visit_function_application_term(self, function_application_term: ast.FunctionApplicationTerm):
        self.stream.write(f'({str(function_application_term.function_identifier)}')
        for arg in function_application_term.arguments:
            self.stream.write(' ')
            arg.accept(self)
        self.stream.write(')')

    @abstractmethod
    def visit_quantified_term(self, quantified_term: ast.QuantifiedTerm):
        raise NotImplemented

    @abstractmethod
    def visit_let_term(self, let_term: ast.LetTerm):
        raise NotImplemented

    def visit_check_synth_command(self, check_synth_command: ast.CheckSynthCommand):
        self.stream.write('(check-synth)')

    def visit_constraint_command(self, constraint_command: ast.ConstraintCommand):
        self.stream.write('(constraint ')
        constraint_command.constraint.accept(self)
        self.stream.write(')')

    def visit_declare_var_command(self, declare_var_command: ast.DeclareVarCommand):
        self.stream.write('(declare-var ')
        self.stream.write(declare_var_command.symbol)
        declare_var_command.sort_expression.accept(self)
        self.stream.write(')')

    @abstractmethod
    def visit_declare_primed_var_command(self, declare_primed_var_command: ast.DeclarePrimedVarCommand):
        raise NotImplemented

    def visit_inv_constraint_command(self, inv_constraint_command: ast.InvConstraintCommand):
        self.stream.write('(inv-constraint ')
        self.stream.write(f'{inv_constraint_command.inv_fun_symbol} ')
        self.stream.write(f'{inv_constraint_command.pre_symbol} ')
        self.stream.write(f'{inv_constraint_command.trans_symbol} ')
        self.stream.write(f'{inv_constraint_command.post_symbol} ')
        self.stream.write(')')

    @abstractmethod
    def visit_set_feature_command(self, set_feature_command: ast.SetFeatureCommand):
        raise NotImplemented

    @abstractmethod
    def visit_set_option_command(self, set_option_command: ast.SetOptionCommand):
        raise NotImplemented

    @abstractmethod
    def visit_set_options_command(self, set_options_command: ast.SetOptionCommand):
        raise NotImplemented

    def visit_set_logic_command(self, set_logic_command: ast.SetLogicCommand):
        self.stream.write(f'(set-logic {set_logic_command.logic_name})')

    @abstractmethod
    def visit_synth_fun_command(self, synth_fun_command: ast.SynthFunCommand):
        raise NotImplemented

    @abstractmethod
    def visit_synth_inv_command(self, synth_inv_command: ast.SynthInvCommand):
        raise NotImplemented

    @abstractmethod
    def visit_grammar_term(self, grammar_term: ast.GrammarTerm):
        raise NotImplemented

    @abstractmethod
    def visit_grouped_rule_list(self, grouped_rule_list: ast.GroupedRuleList):
        raise NotImplemented

    @abstractmethod
    def visit_grammar(self, grammar: ast.Grammar):
        raise NotImplemented

    @abstractmethod
    def visit_declare_sort_command(self, declare_sort_command: ast.DeclareSortCommand):
        raise NotImplemented

    def visit_define_fun_command(self, define_fun_command: ast.DefineFunCommand):
        self.stream.write(f'(define-fun {str(define_fun_command.function_name)} (')
        first = True
        for param in define_fun_command.function_parameters:
            if not first:
                self.stream.write(' ')
            first = False
            self.stream.write(f'({param[0]} ')
            param[1].accept(self)
            self.stream.write(')')
        self.stream.write(') ')
        define_fun_command.function_body.accept(self)
        self.stream.write(')')

    @abstractmethod
    def visit_declare_fun_command(self, declare_fun_command: ast.DeclareFunCommand):
        raise NotImplemented

    @abstractmethod
    def visit_define_sort_command(self, define_sort_command: ast.DefineSortCommand):
        raise NotImplemented

    @abstractmethod
    def visit_datatype_constructor(self, datatype_constructor: ast.DatatypeConstructor):
        raise NotImplemented

    @abstractmethod
    def visit_datatype_constructor_list(self, datatype_constructor_list: ast.DatatypeConstructorList):
        raise NotImplemented

    @abstractmethod
    def visit_declare_datatypes_command(self, declare_datatypes_command: ast.DeclareDatatypesCommand):
        raise NotImplemented

    @abstractmethod
    def visit_declare_datatype_command(self, declare_datatypes_command: ast.DeclareDatatypeCommand):
        raise NotImplemented

    @abstractmethod
    def visit_program(self, program: ast.Program):
        raise NotImplemented
