#!/usr/bin/python

import sys
from .utilities import Identifier, Location
from . import utilities
from abc import abstractmethod, ABC
from typing import Union, List, Tuple, Dict
from enum import Enum

if __name__ == '__main__':
    print('This file is intended to be used as a library module.')
    sys.exit(1)


class ASTVisitor(object):
    __slots__ = ('name',)

    def __init__(self, name: str):
        self.name: str = name

    @abstractmethod
    def visit_sort_expression(self, sort_expression: 'SortExpression'):
        raise NotImplemented

    @abstractmethod
    def visit_identifier_term(self, identifier_term: 'IdentifierTerm'):
        raise NotImplemented

    @abstractmethod
    def visit_literal_term(self, literal_term: 'LiteralTerm'):
        raise NotImplemented

    @abstractmethod
    def visit_function_application_term(self, function_application_term: 'FunctionApplicationTerm'):
        raise NotImplemented

    @abstractmethod
    def visit_quantified_term(self, quantified_term: 'QuantifiedTerm'):
        raise NotImplemented

    @abstractmethod
    def visit_let_term(self, let_term: 'LetTerm'):
        raise NotImplemented

    @abstractmethod
    def visit_check_synth_command(self, check_synth_command: 'CheckSynthCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_constraint_command(self, constraint_command: 'ConstraintCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_declare_var_command(self, declare_var_command: 'DeclareVarCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_inv_constraint_command(self, inv_constraint_command: 'InvConstraintCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_set_feature_command(self, set_feature_command: 'SetFeatureCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_set_option_command(self, set_option_command: 'SetOptionCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_set_logic_command(self, set_logic_command: 'SetLogicCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_synth_fun_command(self, synth_fun_command: 'SynthFunCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_synth_inv_command(self, synth_inv_command: 'SynthInvCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_grammar_term(self, grammar_term: 'GrammarTerm'):
        raise NotImplemented

    @abstractmethod
    def visit_grouped_rule_list(self, grouped_rule_list: 'GroupedRuleList'):
        raise NotImplemented

    @abstractmethod
    def visit_grammar(self, grammar: 'Grammar'):
        raise NotImplemented

    @abstractmethod
    def visit_declare_sort_command(self, declare_sort_command: 'DeclareSortCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_define_fun_command(self, define_fun_command: 'DefineFunCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_define_sort_command(self, define_sort_command: 'DefineSortCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_datatype_constructor(self, datatype_constructor: 'DatatypeConstructor'):
        raise NotImplemented

    @abstractmethod
    def visit_datatype_constructor_list(self, datatype_constructor_list: 'DatatypeConstructorList'):
        raise NotImplemented

    @abstractmethod
    def visit_declare_datatypes_command(self, declare_datatypes_command: 'DeclareDatatypesCommand'):
        raise NotImplemented

    @abstractmethod
    def visit_program(self, program: 'Program'):
        raise NotImplemented


class AST(object):
    __slots__ = ('location',)

    def __init__(self, location: Union[Location, None]):
        self.location: Union[Location, None] = location

    @abstractmethod
    def accept(self, visitor: ASTVisitor):
        raise NotImplemented


class SortExpression(AST):
    __slots__ = ('identifier', 'sort_arguments')

    def __init__(self, location: Union[Location, None],
                 identifier: Union[str, Identifier],
                 sort_arguments: Union[List['SortExpression'], None] = None):
        super().__init__(location)
        self.identifier = utilities.canonicalize_identifier(identifier)
        self.sort_arguments = list(sort_arguments) if sort_arguments is not None else []

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_sort_expression(self)


class LiteralKind(Enum):
    NUMERAL = 1
    DECIMAL = 2
    BOOLEAN = 3
    HEXADECIMAL = 4
    BINARY = 5
    STRING = 6


class Literal(AST):
    __slots__ = ('literal_kind', 'literal_value')

    def __init__(self, location: Union[Location, None],
                 literal_kind: LiteralKind,
                 literal_value: str):
        super().__init__(location)
        self.literal_kind = literal_kind
        self.literal_value = literal_value

    def accept(self, visitor: ASTVisitor):
        pass


class Term(AST, ABC):
    __slots__ = ('sort_descriptor', 'symbol_table_scope')

    def __init__(self, location: Location):
        super().__init__(location)


class IdentifierTerm(Term):
    __slots__ = ('identifier',)

    def __init__(self, location: Location, identifier: Union[Identifier, str]):
        super().__init__(location)
        self.identifier = utilities.canonicalize_identifier(identifier)

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_identifier_term(self)


class LiteralTerm(Term):
    __slots__ = ('literal',)

    def __init__(self, location: Location, literal: Literal):
        super().__init__(location)
        self.literal = literal

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_literal_term(self)


class FunctionApplicationTerm(Term):
    __slots__ = ('function_identifier', 'arguments')

    def __init__(self, location: Location, identifier: Union[Identifier, str],
                 arguments: List[Term]):
        super().__init__(location)
        self.function_identifier = utilities.canonicalize_identifier(identifier)
        self.arguments = list(arguments)

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_function_application_term(self)


class QuantifiedTerm(Term):
    __slots__ = ('quantified_variables', 'term_body')

    def __init__(self, location: Location, quantified_variables: List[Tuple[str, SortExpression]], term_body: Term):
        super().__init__(location)
        self.quantified_variables = list(quantified_variables)
        self.term_body = term_body

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_quantified_term(self)


class LetTerm(Term):
    __slots__ = ('variable_bindings', 'let_body')

    def __init__(self, location: Location, bindings: List[Tuple[str, Term]], let_body: Term):
        super().__init__(location)
        self.variable_bindings = list(bindings)
        self.let_body = let_body

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_let_term(self)


class CommandKind(Enum):
    CHECK_SYNTH = 1
    CONSTRAINT = 2
    DECLARE_VAR = 3
    INV_CONSTRAINT = 4
    SET_FEATURE = 5
    SET_OPTION = 6
    SET_LOGIC = 7
    SYNTH_FUN = 8
    SYNTH_INV = 9
    DECLARE_SORT = 10
    DEFINE_FUN = 11
    DEFINE_SORT = 12
    DECLARE_DATATYPES = 13
    DECLARE_DATATYPE = 14


class Command(AST, ABC):
    __slots__ = ('command_kind',)

    def __init__(self, location: Location, command_kind: CommandKind):
        super().__init__(location)
        self.command_kind: CommandKind = command_kind


class CheckSynthCommand(Command):
    def __init__(self, location: Location):
        super().__init__(location, CommandKind.CHECK_SYNTH)

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_check_synth_command(self)


class ConstraintCommand(Command):
    __slots__ = ('constraint',)

    def __init__(self, location: Location, constraint: Term):
        super().__init__(location, CommandKind.CONSTRAINT)
        self.constraint: Term = constraint

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_constraint_command(self)


class DeclareVarCommand(Command):
    __slots__ = ('symbol', 'sort_expression')

    def __init__(self, location: Location, symbol: str, sort_expression: SortExpression):
        super().__init__(location, CommandKind.DECLARE_VAR)
        self.symbol: str = symbol
        self.sort_expression: SortExpression = sort_expression

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_declare_var_command(self)


class InvConstraintCommand(Command):
    __slots__ = ('inv_fun_symbol', 'pre_symbol', 'trans_symbol', 'post_symbol')

    def __init__(self, location: Location, inv_fun_symbol: str,
                 pre_symbol: str, trans_symbol: str, post_symbol: str):
        super().__init__(location, CommandKind.INV_CONSTRAINT)
        self.inv_fun_symbol = inv_fun_symbol
        self.pre_symbol = pre_symbol
        self.trans_symbol = trans_symbol
        self.post_symbol = post_symbol

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_inv_constraint_command(self)


class SetFeatureCommand(Command):
    __slots__ = ('feature_name', 'feature_value')

    def __init__(self, location: Location, feature_name: str, feature_value: bool):
        super().__init__(location, CommandKind.SET_FEATURE)
        self.feature_name: str = feature_name
        self.feature_value: bool = feature_value

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_set_feature_command(self)


class SetOptionCommand(Command):
    __slots__ = ('option_name', 'option_value')

    def __init__(self, location: Location, option_name: str, option_value: Literal):
        super().__init__(location, CommandKind.SET_OPTION)
        self.option_name: str = option_name
        self.option_value: Literal = option_value

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_set_option_command(self)


class SetLogicCommand(Command):
    __slots__ = ('logic_name',)

    def __init__(self, location: Location, logic_name: str):
        super().__init__(location, CommandKind.SET_LOGIC)
        self.logic_name: str = logic_name

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_set_logic_command(self)


class GrammarTermKind(Enum):
    CONSTANT = 1
    VARIABLE = 2
    BINDER_FREE = 3


class GrammarTerm(Term):
    __slots__ = ('grammar_term_kind', 'sort_expression', 'binder_free_term')

    def __init__(self, location: Location):
        super().__init__(location)

    @staticmethod
    def create_constant_term(location: Location, sort_expression: SortExpression) -> 'GrammarTerm':
        result = GrammarTerm(location)
        result.sort_expression = sort_expression
        result.binder_free_term = None
        result.grammar_term_kind: GrammarTermKind = GrammarTermKind.CONSTANT
        return result

    @staticmethod
    def create_variable_term(location: Location, sort_expression: SortExpression):
        result = GrammarTerm(location)
        result.sort_expression = sort_expression
        result.binder_free_term = None
        result.grammar_term_kind: GrammarTermKind = GrammarTermKind.VARIABLE
        return result

    @staticmethod
    def create_binder_free_grammar_term(location: Location, binder_free_term: Term):
        result = GrammarTerm(location)
        result.sort_expression = None
        result.binder_free_term = binder_free_term
        result.grammar_term_kind: GrammarTermKind = GrammarTermKind.BINDER_FREE
        return result

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_grammar_term(self)


class GroupedRuleList(AST):
    __slots__ = ('head_symbol', 'head_symbol_sort_expression', 'expansion_rules')

    def __init__(self, location: Location, head_symbol: str,
                 head_symbol_sort_expression: SortExpression,
                 expansion_rules: List[GrammarTerm]):
        super().__init__(location)
        self.head_symbol = head_symbol
        self.head_symbol_sort_expression = head_symbol_sort_expression
        self.expansion_rules = list(expansion_rules)

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_grouped_rule_list(self)


class Grammar(AST):
    __slots__ = ('nonterminals', 'grouped_rule_lists', 'symbol_table_scope')

    def __init__(self, location: Location,
                 nonterminals: List[Tuple[str, SortExpression]],
                 grouped_rule_lists: List[GroupedRuleList]):
        super().__init__(location)
        self.nonterminals = list(nonterminals)
        self.grouped_rule_lists: Dict[str, GroupedRuleList] = {}
        for grouped_rule_list in grouped_rule_lists:
            head = grouped_rule_list.head_symbol
            if head in self.grouped_rule_lists:
                raise KeyError('The symbol: %s has more than one group of rules associated with it.\n'
                               'At: %s' % (head, str(grouped_rule_list.location)))
            self.grouped_rule_lists[head] = grouped_rule_list

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_grammar(self)


class SynthFunCommand(Command):
    __slots__ = ('function_symbol', 'argument_sort_expressions', 'range_sort_expression',
                 'synthesis_grammar', 'symbol_table_scope')

    def __init__(self, location: Location, function_symbol: str,
                 argument_sort_expressions: List[SortExpression],
                 range_sort_expression: SortExpression,
                 synthesis_grammar: Grammar):
        super().__init__(location, CommandKind.SYNTH_FUN)
        self.function_symbol = function_symbol
        self.argument_sort_expressions: List[SortExpression] = list(argument_sort_expressions)
        self.range_sort_expression: SortExpression = range_sort_expression
        self.synthesis_grammar = synthesis_grammar
        self.symbol_table_scope = None

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_synth_fun_command(self)


class SynthInvCommand(Command):
    __slots__ = ('function_symbol', 'argument_sort_expressions', 'synthesis_grammar',
                 'symbol_table_scope')

    def __init__(self, location: Location,
                 function_symbol: str,
                 argument_sort_expressions: List[SortExpression],
                 synthesis_grammar: Grammar):
        super().__init__(location, CommandKind.SYNTH_INV)
        self.function_symbol = function_symbol
        self.argument_sort_expressions = list(argument_sort_expressions)
        self.synthesis_grammar = synthesis_grammar
        self.symbol_table_scope = None

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_synth_inv_command(self)


class DeclareSortCommand(Command):
    __slots__ = ('sort_name', 'sort_arity')

    def __init__(self, location: Location,
                 sort_name: str, sort_arity: int):
        super().__init__(location, CommandKind.DECLARE_SORT)
        self.sort_name: str = sort_name
        self.sort_arity: int = sort_arity

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_declare_sort_command(self)


class DefineFunCommand(Command):
    __slots__ = ('function_name', 'function_parameters', 'function_range_sort', 'function_body')

    def __init__(self, location: Location, function_name: str,
                 function_parameters: List[Tuple[str, SortExpression]],
                 function_range_sort: SortExpression,
                 function_body: Term):
        super().__init__(location, CommandKind.DEFINE_FUN)
        self.function_name: str = function_name
        self.function_parameters: List[Tuple[str, SortExpression]] = list(function_parameters)
        self.function_range_sort: SortExpression = function_range_sort
        self.function_body: Term = function_body

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_define_fun_command(self)


class DefineSortCommand(Command):
    __slots__ = ('sort_name', 'sort_expression')

    def __init__(self, location: Location, sort_name: str, sort_expression: SortExpression):
        super().__init__(location, CommandKind.DEFINE_SORT)
        self.sort_name: str = sort_name
        self.sort_expression: SortExpression = sort_expression

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_define_sort_command(self)


class DatatypeConstructor(AST):
    __slots__ = ('constructor_name', 'constructor_parameters')

    def __init__(self, location: Location, constructor_name: str,
                 constructor_parameters: List[Tuple[str, SortExpression]]):
        super().__init__(location)
        self.constructor_name: str = constructor_name
        self.constructor_parameters: List[Tuple[str, SortExpression]] = list(constructor_parameters)

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_datatype_constructor(self)


class DatatypeConstructorList(AST):
    __slots__ = ('introduced_sort_parameters', 'constructors')

    def __init__(self, location: Location, introduced_parameters: List[str],
                 constructors: List[DatatypeConstructor]):
        super().__init__(location)
        self.introduced_sort_parameters = list(introduced_parameters)
        self.constructors = list(constructors)

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_datatype_constructor_list(self)


class DeclareDatatypesCommand(Command):
    __slots__ = ('sort_names_and_arities', 'datatype_constructor_lists')

    def __init__(self, location: Location, sort_names_and_arities: List[Tuple[str, int]],
                 datatype_constructor_lists: List[DatatypeConstructorList]):
        super().__init__(location, CommandKind.DECLARE_DATATYPES)
        self.sort_names_and_arities = list(sort_names_and_arities)
        self.datatype_constructor_lists = list(datatype_constructor_lists)

        if len(self.sort_names_and_arities) != len(self.datatype_constructor_lists):
            raise ValueError('Must have same number of datatype constructor lists as introduced sorts.')

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_declare_datatypes_command(self)


class Program(AST):
    __slots__ = ('commands',)

    def __init__(self, location: Location, commands: List[Command]):
        super().__init__(location)
        self.commands = list(commands)

    def accept(self, visitor: ASTVisitor):
        return visitor.visit_program(self)
