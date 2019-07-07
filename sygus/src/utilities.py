from abc import abstractmethod
from typing import Union

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


def are_all_elements_equal(sequence, expected=None):
    if expected is None:
        expected = sequence[0]
    return all(x == expected for x in sequence)


class Hashable(object):
    def __init__(self):
        self._hash = None

    @abstractmethod
    def _compute_hash(self) -> int:
        raise NotImplementedError

    @abstractmethod
    def _compute_equals(self, other):
        raise NotImplementedError

    def __hash__(self):
        if self._hash is None:
            self._hash = self._compute_hash() & sixty_four_bit_integer_mask
        return self._hash

    def __eq__(self, other):
        if hash(self) != hash(other):
            return False
        if type(other) != type(self):
            return False
        return self._compute_equals(other)


class Location(Hashable):
    __slots__ = ['line', 'col']

    def __init__(self, line, col):
        super().__init__()
        self.line = line
        self.col = col

    def _compute_hash(self) -> int:
        return hash_sequence([self.line, self.col])

    def _compute_equals(self, other: 'Location'):
        return self.line == other.line and self.col == other.col

    def __str__(self):
        return '%d:%d' % (self.line, self.col)


class Identifier(Hashable):
    __slots__ = ['symbol', 'indices', 'start_location', 'end_location']

    def __init__(self, symbol: str, *indices: Union[str, int]):
        super().__init__()
        self.symbol = symbol
        if indices is not None and len(indices) > 0:
            self.indices = indices
        else:
            self.indices = []
        self._hash = None

        if any(not isinstance(x, str) and not isinstance(x, int) for x in self.indices):
            raise TypeError('Indices must be either of string or integer type')

        self.start_location = None
        self.end_location = None

    def _compute_equals(self, other: 'Identifier'):
        return self.symbol == other.symbol and first_discrepancy(self.indices, other.indices) is None

    def _compute_hash(self) -> int:
        return hash_sequence(self.indices, hash(self.symbol))

    def is_indexed(self) -> bool:
        return self.indices is not None and len(self.indices) > 0

    def __str__(self):
        if self.is_indexed():
            return '(_ %s %s)' % (self.symbol, ' '.join(str(x) for x in self.indices))
        else:
            return self.symbol


def canonicalize_identifier(identifier: Union[str, Identifier]):
    if isinstance(identifier, Identifier):
        return identifier
    elif isinstance(identifier, str):
        return Identifier(identifier)
    else:
        raise TypeError('Only str types can be coerced into Identifier instances.')
