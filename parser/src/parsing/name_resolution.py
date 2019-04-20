#!/usr/bin/python

import sys
from . import utilities
from .utilities import Identifier, Hashable
from typing import Union, List, Tuple, Dict
from enum import Enum
from abc import ABC, abstractmethod
from . import ast

if __name__ == '__main__':
    print('This file is intended to be used as a library module.')
    sys.exit(1)


class SymbolTableEntryKind(Enum):
    SORT_DESCRIPTOR = 1
    FUNCTION_DESCRIPTOR = 2
    SYMBOL_DESCRIPTOR = 3


class SymbolTableEntry(ABC):
    __slots__ = ('entry_kind', 'identifier')

    def __init__(self, kind: SymbolTableEntryKind, identifier: Union[Identifier, str]):
        self.entry_kind: SymbolTableEntryKind = kind
        self.identifier: Identifier = utilities.canonicalize_identifier(identifier)


class SortKind(Enum):
    UNINTERPRETED = 1
    PRIMITIVE = 2
    DATATYPE = 3
    FUNCTION = 4


class SortDescriptor(SymbolTableEntry, Hashable):
    __slots__ = ('sort_kind', 'num_parameters', 'instantiated_from', 'sort_arguments')

    def __init__(self, identifier: Union[Identifier, str]):
        super().__init__(SymbolTableEntryKind.SORT_DESCRIPTOR, identifier)

    @staticmethod
    def create_sort(identifier: Union[Identifier, str], num_parameters: int, sort_kind: SortKind) -> 'SortDescriptor':
        result = SortDescriptor(identifier)
        result.num_parameters: int = num_parameters
        result.sort_arguments = []
        result.instantiated_from = None
        result.sort_kind = sort_kind
        return result

    def _compute_equals(self, other: 'SortDescriptor') -> bool:
        if self.identifier != other.identifier:
            return False

        if len(self.sort_arguments) != len(other.sort_arguments):
            return False

        for i in range(len(self.sort_arguments)):
            self_type = type(self.sort_arguments[i])
            other_type = type(other.sort_arguments[i])

            if self_type != other_type:
                return False

            if self_type == SortDescriptor and self.sort_arguments[i] != other.sort_arguments[i]:
                return False

        return True

    def _compute_hash(self) -> int:
        return utilities.hash_sequence(hash(self.identifier), self.sort_arguments)

    def instantiate(self, args: List[Union['SortDescriptor', str]]):
        if self.num_parameters == 0 or len(self.sort_arguments) > 0:
            raise TypeError('Cannot instantiate a non-parametric sort.')
        if any(not(isinstance(x, str) or isinstance(x, SortDescriptor)) for x in args):
            raise TypeError('Sorts can only be instantiated with strings or other sorts.')

        result = SortDescriptor(self.identifier)
        result.sort_kind = self.sort_kind
        result.num_parameters = self.num_parameters
        result.sort_arguments = list(args)
        result.instantiated_from = self

        return result

    def substitute(self, **kwargs):
        result = SortDescriptor(self.identifier)
        result.sort_kind = self.sort_kind
        result.num_parameters = self.num_parameters
        result.sort_arguments = [kwargs[x] if x in kwargs else x for x in self.sort_arguments]
        result.instantiated_from = self.instantiated_from

    def is_concrete(self):
        return self.num_parameters == 0 or (len(self.sort_arguments) == self.num_parameters and
                                            all(isinstance(x, SortDescriptor) for x in self.sort_arguments))


class FunctionKind(Enum):
    UNINTERPRETED = 1
    SYNTH_FUN = 2
    SYNTH_INV = 3
    USER_DEFINED = 4
    DATATYPE_CONSTRUCTOR = 5
    DATATYPE_TESTER = 6


class FunctionDescriptor(SymbolTableEntry):
    __slots__ = ('function_kind', 'argument_sorts', 'argument_names',
                 'introduced_type_parameters', 'range_sort',
                 'function_body', 'synthesis_grammar')

    def __init__(self, identifier: Union[Identifier, str]):
        super().__init__(SymbolTableEntryKind.FUNCTION_DESCRIPTOR, identifier)

    @staticmethod
    def create_uninterpreted_function(identifier: Union[Identifier, str],
                                      arg_sorts: List[SortDescriptor],
                                      range_sort: SortDescriptor) -> 'FunctionDescriptor':
        result = FunctionDescriptor(identifier)
        result.function_kind = FunctionKind.UNINTERPRETED
        result.argument_sorts = list(arg_sorts)
        result.argument_names = None
        result.range_sort = range_sort
        result.introduced_type_parameters = None
        result.function_body = None
        result.synthesis_grammar = None
        return result

    @staticmethod
    def create_synthesis_objective(identifier: Union[Identifier, str],
                                   parameters: List[Tuple[str, SortDescriptor]],
                                   range_sort: SortDescriptor,
                                   function_kind: FunctionKind,
                                   synthesis_grammar: ast.Grammar) -> 'FunctionDescriptor':
        if function_kind != FunctionKind.SYNTH_FUN and function_kind != FunctionKind.SYNTH_INV:
            raise ValueError('function_kind must be FunctionKind.SYNTH_FUN or FunctionKind.SYNTH_INV '
                             'in call to FunctionDescriptor.create_synthesis_objective()')

        result = FunctionDescriptor(identifier)
        result.function_kind = function_kind
        result.argument_sorts = [x[1] for x in parameters]
        result.argument_names = [x[0] for x in parameters]
        result.range_sort = range_sort
        result.introduced_type_parameters = None
        result.function_body = None
        result.synthesis_grammar: ast.Grammar = synthesis_grammar
        return result

    @staticmethod
    def create_user_defined_function(identifier: Union[Identifier, str],
                                     parameters: List[Tuple[str, SortDescriptor]],
                                     range_sort: SortDescriptor,
                                     function_body: ast.Term):
        result = FunctionDescriptor(identifier)
        result.function_kind = FunctionKind.USER_DEFINED
        result.argument_sorts = [x[1] for x in parameters]
        result.argument_names = [x[0] for x in parameters]
        result.range_sort = range_sort
        result.function_body = function_body
        result.introduced_type_parameters = None
        result.synthesis_grammar = None
        return result

    @staticmethod
    def create_datatype_constructor(identifier: str,
                                    introduced_type_parameters: List[str],
                                    parameters: List[Tuple[str, Union[SortDescriptor, str]]],
                                    datatype_sort: SortDescriptor):
        result = FunctionDescriptor(identifier)
        result.function_kind = FunctionKind.DATATYPE_CONSTRUCTOR
        result.introduced_type_parameters = list(introduced_type_parameters)
        result.argument_names = [x[0] for x in parameters]
        result.argument_sorts: List[Union[SortDescriptor, str]] = []
        for parameter in parameters:
            if isinstance(parameter[1], str) and parameter[1] not in introduced_type_parameters:
                raise ValueError('Unresolved type name: ' + parameter[1])
            result.argument_sorts.append(parameter[1])
        result.range_sort = datatype_sort
        result.function_body = None
        result.synthesis_grammar = None
        return result

    @staticmethod
    def create_datatype_tester(constructor_name: str, datatype_sort_descriptor: SortDescriptor):
        tester_identifier = Identifier('is', constructor_name)
        result = FunctionDescriptor(tester_identifier)
        result.function_kind = FunctionKind.DATATYPE_TESTER
        result.introduced_type_parameters = None
        result.argument_names = None
        result.argument_sorts = [datatype_sort_descriptor]
        result.range_sort = SymbolTable.get_boolean_sort()
        result.function_body = None
        result.synthesis_grammar = None
        return result


class SymbolKind(Enum):
    UNIVERSAL_VARIABLE = 1
    PARAMETER = 2
    QUANTIFIED_VARIABLE = 3
    LET_BOUND_VARIABLE = 4
    GRAMMAR_NONTERMINAL = 5


class SymbolDescriptor(SymbolTableEntry):
    __slots__ = ('symbol_sort', 'symbol_kind')

    def __init__(self, symbol_kind: SymbolKind, symbol: str, symbol_sort: SortDescriptor):
        super().__init__(SymbolTableEntryKind.SYMBOL_DESCRIPTOR, symbol)
        self.symbol_sort: SortDescriptor = symbol_sort
        self.symbol_kind: SymbolKind = symbol_kind


class SymbolTableKey(Hashable):
    __slots__ = ('identifier', 'arg_sorts')

    def __init__(self, identifier: Union[str, Identifier], *arg_sorts: SortDescriptor):
        super().__init__()
        self.identifier = utilities.canonicalize_identifier(identifier)
        self.arg_sorts = list(arg_sorts)

    def _compute_hash(self) -> int:
        return utilities.hash_sequence(hash(self.identifier), self.arg_sorts)

    def _compute_equals(self, other: 'SymbolTableKey'):
        return (self.identifier == other.identifier and
                utilities.first_discrepancy(self.arg_sorts, other.arg_sorts) is None)


class SymbolTable(object):
    __slots__ = ('scope_stack',)

    def __init__(self):
        self.scope_stack = [{}]

    def push_scope(self, scope: Dict[SymbolTableKey, SymbolTableEntry] = None):
        self.scope_stack.append(scope if scope is not None else {})

    def pop_scope(self) -> Dict[SymbolTableKey, SymbolTableEntry]:
        return self.scope_stack.pop()

    @staticmethod
    def get_boolean_sort():
        raise NotImplemented

    def _lookup(self, identifier: Union[Identifier, str], *arg_sorts: SortDescriptor):
        key = SymbolTableKey(identifier, *arg_sorts)

        for i in reversed(range(len(self.scope_stack))):
            scope = self.scope_stack[i]
            if key in scope:
                return scope[key]

        return None

    def lookup_sort(self, identifier: Union[Identifier, str]) -> SortDescriptor:
        res = self._lookup(identifier)
        return res if isinstance(res, SortDescriptor) else None

    def lookup_function(self, identifier: Union[Identifier, str], *arg_sorts: SortDescriptor) -> FunctionDescriptor:
        res = self._lookup(identifier, *arg_sorts)
        return res if isinstance(res, FunctionDescriptor) else None

    def lookup_symbol(self, identifier: str) -> SymbolDescriptor:
        res = self._lookup(identifier)
        return res if isinstance(res, SymbolDescriptor) else None

    def lookup_or_resolve_sort(self, identifier: Union[Identifier, str]) -> SortDescriptor:
        res = self.lookup_sort(identifier)
        return res if res is not None else Resolver.resolve_sort(identifier)


class Resolver(ABC):
    __slots__ = ('name',)

    _resolvers = []

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(cls, **kwargs)
        cls._resolvers.append(cls)

    @classmethod
    def resolve_sort(cls, identifier: Union[Identifier, str]) -> Union[SortDescriptor, None]:
        identifier = utilities.canonicalize_identifier(identifier)
        for resolver in cls._resolvers:
            result = resolver.resolve_sort_impl(identifier)
            if result is not None:
                return result
        return None

    @classmethod
    def resolve_function(cls, identifier: Union[Identifier, str],
                         *arg_sorts: SortDescriptor) -> Union[FunctionDescriptor, None]:
        identifier = utilities.canonicalize_identifier(identifier)
        for resolver in cls._resolvers:
            result = resolver.resolve_function_impl(identifier, *arg_sorts)
            if result is not None:
                return result
        return None

    @abstractmethod
    def resolve_sort_impl(self, identifier: Identifier):
        raise NotImplemented

    @abstractmethod
    def resolve_function_impl(self, identifier: Identifier, *arg_sorts: SortDescriptor):
        raise NotImplemented


class CoreResolver(Resolver):
    _boolean_sort = SortDescriptor.create_sort('Bool', 0, SortKind.PRIMITIVE)

    @classmethod
    def get_boolean_sort(cls):
        return cls._boolean_sort

    def resolve_sort_impl(self, identifier: Identifier):
        pass

    def resolve_function_impl(self, identifier: Identifier, *arg_sorts: SortDescriptor):
        pass


class IntegerResolver(Resolver):
    _integer_sort = SortDescriptor.create_sort('Int', 0, SortKind.PRIMITIVE)

    @classmethod
    def get_integer_sort(cls):
        return cls._integer_sort

    def resolve_sort_impl(self, identifier: Identifier):
        pass

    def resolve_function_impl(self, identifier: Identifier, *arg_sorts: SortDescriptor):
        pass


class RealResolver(Resolver):
    _real_sort = SortDescriptor.create_sort('Real', 0, SortKind.PRIMITIVE)

    @classmethod
    def get_real_sort(cls):
        return cls._real_sort

    def resolve_sort_impl(self, identifier: Identifier):
        pass

    def resolve_function_impl(self, identifier: Identifier, *arg_sorts: SortDescriptor):
        pass


class ArrayResolver(Resolver):
    _array_sort = SortDescriptor.create_sort('Array', 2, SortKind.PRIMITIVE)

    @classmethod
    def get_array_sort(cls):
        return cls._array_sort

    def resolve_sort_impl(self, identifier: Identifier):
        pass

    def resolve_function_impl(self, identifier: Identifier, *arg_sorts: SortDescriptor):
        pass


class StringResolver(Resolver):
    _string_sort = SortDescriptor.create_sort('String', 0, SortKind.PRIMITIVE)

    @classmethod
    def get_string_sort(cls):
        return cls._string_sort

    def resolve_sort_impl(self, identifier: Identifier):
        pass

    def resolve_function_impl(self, identifier: Identifier, *arg_sorts: SortDescriptor):
        pass


class BitVectorResolver(Resolver):
    _bit_vector_sorts: Dict[int, SortDescriptor] = {}

    @classmethod
    def get_bit_vector_sort(cls, size: int):
        result = cls._bit_vector_sorts.get(size, None)

        if result is None:
            identifier = Identifier('BitVec', size)
            result = SortDescriptor.create_sort(identifier, 0, SortKind.PRIMITIVE)
            cls._bit_vector_sorts[size] = result

        return result

    def resolve_sort_impl(self, identifier: Identifier):
        pass

    def resolve_function_impl(self, identifier: Identifier, *arg_sorts: SortDescriptor):
        pass
