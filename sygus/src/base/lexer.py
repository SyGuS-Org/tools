import ply.lex

from .. import utilities, exceptions


# noinspection PyPep8Naming,PyMethodMayBeStatic,PySingleQuotedDocstring
class SygusLexerBase(object):
    reserved = {
        'define-sort': 'TK_DEFINE_SORT',
        'define-fun': 'TK_DEFINE_FUN',
        'check-synth': 'TK_CHECK_SYNTH',
        'synth-fun': 'TK_SYNTH_FUN',
        'declare-var': 'TK_DECLARE_VAR',
        'inv-constraint': 'TK_INV_CONSTRAINT',
        'synth-inv': 'TK_SYNTH_INV',
        'set-logic': 'TK_SET_LOGIC',
        'constraint': 'TK_CONSTRAINT',
        'Constant': 'TK_CONSTANT',
        'Variable': 'TK_VARIABLE',
        'let': 'TK_LET',
        'true': 'TK_BOOL_CONST',
        'false': 'TK_BOOL_CONST',
        'exists': 'TK_EXISTS',
        'forall': 'TK_FORALL',
    }

    tokens = [
        'TK_LPAREN',
        'TK_RPAREN',
        'TK_NUMERAL',
        'TK_DECIMAL',
        'TK_HEX_CONST',
        'TK_BIN_CONST',
        'TK_STRING_LITERAL',
        'TK_SYMBOL',
    ]

    t_TK_LPAREN = r'\('
    t_TK_RPAREN = r'\)'

    _letter = r'[a-zA-Z_]'
    _zero = r'0'
    _nonzero = r'[1-9]'
    _digit = r'[0-9]'
    _hexdigit = f'{_digit}|[a-fA-F]'
    _bit = r'0|1'
    _numeral = f'(?:{_zero})|(?:{_nonzero}(?:{_digit})*)'
    _decimal = f'{_numeral}\\.(?:{_digit})+'
    _hexconst = f'[#]x(?:{_hexdigit})+'
    _binconst = f'[#]b(?:{_bit})+'
    _symbolcc = r'(?:[a-zA-Z_&!~<>=/%]|\+|-|\*|\||\?|\.|\$|\^)'
    _symbol = f'{_symbolcc}(?:(?:{_symbolcc})|(?:{_digit}))*'
    _stringconst = r'\"(?:[^\"]*(?:\"\")?)*\"'

    def _get_locations(self, t):
        end_line = t.lexer.lineno
        full_input = t.lexer.lexdata
        line_start = full_input.rfind('\n', 0, t.lexpos) + 1
        end_col = (t.lexpos - line_start) + 1

        end_location = utilities.Location(end_line, end_col)

        start_pos = t.lexpos - len(t.value)
        start_line = end_line - t.value.count('\n')
        line_start = full_input.rfind('\n', 0, start_pos) + 1
        start_col = (start_pos - line_start) + 1

        start_location = utilities.Location(start_line, start_col)

        return start_location, end_location

    t_ignore = ' \t\f'

    def t_newline(self, t):
        r'\n'
        t.lexer.lineno += 1

    # noinspection PyUnusedLocal
    def t_comment(self, t):
        r';.*'
        pass

    @ply.lex.TOKEN(_numeral)
    def t_TK_NUMERAL(self, t):
        t.value = int(t.value)
        return t

    @ply.lex.TOKEN(_decimal)
    def t_TK_DECIMAL(self, t):
        return t

    @ply.lex.TOKEN(_hexconst)
    def t_TK_HEX_CONST(self, t):
        return t

    @ply.lex.TOKEN(_binconst)
    def t_TK_BIN_CONST(self, t):
        return t

    @ply.lex.TOKEN(_stringconst)
    def t_TK_STRING_LITERAL(self, t):
        # strip away the quotes
        t.value = t.value[1:(len(t.value) - 1)]
        return t

    def t_error(self, t):
        start_location, end_location = self._get_locations(t)
        raise exceptions.ParseException(
            f'Could not figure out what to do with string: \"{t.value}\"',
            start_location, end_location)

    def __init__(self):
        self.lexer = ply.lex.lex(object=self, debug=False)

    def lex(self, input_string):
        self.lexer.input(input_string)
        while True:
            tok = self.lexer.token()
            if tok is None:
                break
            yield tok
