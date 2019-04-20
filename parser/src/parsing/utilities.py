#!/usr/bin/python

import sys
from abc import abstractmethod
from typing import Union, List

if __name__ == '__main__':
    print('This file is intended to be used as a library module.')
    sys.exit(1)

sixty_four_bit_integer_mask = 0xFFFFFFFFFFFFFFFF
thirty_two_bit_integer_mask = 0xFFFFFFFF


def hash_sequence(sequence, hash_seed=0, hash_combiner=None):
    def _default_hash_combiner(x: int, y: int) -> int:
        return (x * 1018057) ^ y

    if hash_combiner is None:
        hash_combiner = _default_hash_combiner

    result = hash_seed
    if sequence is None:
        return result

    for element in sequence:
        hash_combiner(result, hash(element))

    return result & sixty_four_bit_integer_mask


def first_discrepancy(sequence_one, sequence_two) -> Union[int, None]:
    if len(sequence_one) != len(sequence_two):
        return min(len(sequence_one), len(sequence_two))

    for i in range(len(sequence_one)):
        if sequence_one[i] != sequence_two[i]:
            return i

    return None


class Hashable(object):
    __slots__ = ('_hash',)

    def __init__(self):
        self._hash = None

    @abstractmethod
    def _compute_hash(self) -> int:
        raise NotImplemented

    @abstractmethod
    def _compute_equals(self, other):
        raise NotImplemented

    def __hash__(self):
        if self._hash is None:
            self._hash = self._compute_hash() & sixty_four_bit_integer_mask
        return self._hash

    def __eq__(self, other):
        if hash(self) != hash(object):
            return False
        if type(other) != type(self):
            return False
        return self._compute_equals(other)


class Identifier(Hashable):
    __slots__ = ('symbol', 'indices', '_hash')

    def __init__(self, symbol: str, *indices: Union[str, int]):
        super().__init__()
        self.symbol = symbol
        if indices is not None and len(indices) > 0:
            self.indices = indices
        else:
            self.indices = None
        self._hash = None

        if self.indices is not None:
            index_type = type(self.indices[0])
            if (index_type != str and index_type != int) or (any(type(x) != index_type for x in self.indices)):
                raise TypeError('Indices must be either of string or integer type')

    def _compute_equals(self, other: 'Identifier'):
        return self.symbol == other.symbol and first_discrepancy(self.indices, other.indices) is None

    def _compute_hash(self) -> int:
        return hash_sequence(self.indices, hash(self.symbol))

    def is_indexed(self) -> bool:
        return self.indices is not None

    def __str__(self):
        if self.is_indexed():
            return '(_ %s %s)' % (self.symbol, ' '.join(str(x) for x in self.indices))
        else:
            return self.symbol


class Location(Hashable):
    __slots__ = ('line_number', 'column_number')

    def __init__(self, line_number, column_number):
        super().__init__()
        self.line_number = line_number
        self.column_number = column_number

    def _compute_hash(self) -> int:
        return (hash(self.line_number) * 1019093) ^ hash(self.column_number)

    def _compute_equals(self, other: 'Location'):
        return self.line_number == other.line_number and self.column_number == other.column_number

    def __str__(self):
        return '%d:%d' % (self.line_number, self.column_number)


def canonicalize_identifier(idenfier: Union[str, Identifier]):
    if isinstance(idenfier, Identifier):
        return idenfier
    elif isinstance(idenfier, str):
        return Identifier(idenfier)
    else:
        raise TypeError('Only str types can be coerced into Identifier instances.')