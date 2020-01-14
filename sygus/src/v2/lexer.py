import ply.lex

from ..base.lexer import SygusLexerBase


# noinspection PyPep8Naming
class SygusV2Lexer(SygusLexerBase):
    tokens = SygusLexerBase.tokens
    reserved = SygusLexerBase.reserved

    reserved['declare-datatype'] = 'TK_DECLARE_DATATYPE'
    reserved['declare-datatypes'] = 'TK_DECLARE_DATATYPES'
    reserved['declare-sort'] = 'TK_DECLARE_SORT'
    reserved['par'] = 'TK_PAR'
    reserved['set-feature'] = 'TK_SET_FEATURE'
    reserved['set-option'] = 'TK_SET_OPTION'

    tokens += list(set(reserved.values()))

    @ply.lex.TOKEN(SygusLexerBase._symbol)
    def t_TK_SYMBOL(self, t):
        if t.value == '_':
            t.type = 'TK_UNDERSCORE'
        else:
            t.type = self.reserved.get(t.value, 'TK_SYMBOL')
        return t

    tokens += [
        'TK_COLON',
        'TK_UNDERSCORE'
    ]

    t_TK_COLON = r':'

    def __init__(self):
        super().__init__()
