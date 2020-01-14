import ply.yacc

from abc import abstractmethod
from io import StringIO

from .. import ast, exceptions, utilities
from ..symbol_table_builder import SymbolTableBuilder

from .lexer import SygusLexerBase


# noinspection PyMethodMayBeStatic
class SygusParserBase(object):
    tokens = SygusLexerBase.tokens

    def _get_position(self, line: int, pos: int) -> utilities.Location:
        line_start = self.input_string.rfind('\n', 0, pos) + 1
        end_col = (pos - line_start) + 1
        return utilities.Location(line, end_col)

    def p_program(self, p):
        """program : set_logic_command command_plus
                   | command_plus"""
        if len(p) == 3:
            p[0] = ast.Program([p[1]] + p[2])
        elif len(p) == 2:
            p[0] = ast.Program(p[1])
        else:
            raise NotImplementedError

        self._ast_root = p[0]

    def p_command_plus(self, p):
        """command_plus : command_plus command
                        | command"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_check_synth_command(self, p):
        """check_synth_command : TK_LPAREN TK_CHECK_SYNTH TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(3), p.lexpos(3))
        p[0] = ast.CheckSynthCommand(start_position, end_position)

    def p_constraint_command(self, p):
        """constraint_command : TK_LPAREN TK_CONSTRAINT term TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.ConstraintCommand(p[3], start_position, end_position)

    def p_var_decl_command(self, p):
        """var_decl_command : TK_LPAREN TK_DECLARE_VAR TK_SYMBOL sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(5), p.lexpos(5))
        p[0] = ast.DeclareVarCommand(p[3], p[4], start_position, end_position)

    def p_inv_constraint_command(self, p):
        """inv_constraint_command : TK_LPAREN TK_INV_CONSTRAINT TK_SYMBOL TK_SYMBOL TK_SYMBOL TK_SYMBOL TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        p[0] = ast.InvConstraintCommand(p[3], p[4], p[5], p[6], start_position, end_position)

    def p_set_logic_command(self, p):
        """set_logic_command : TK_LPAREN TK_SET_LOGIC TK_SYMBOL TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.SetLogicCommand(p[3], start_position, end_position)

    def _p_literal_common(self, p, kind):
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(str(p[1])))
        end_position = self._get_position(p.lineno(1), p.lexpos(1))
        p[0] = ast.Literal(kind, p[1], start_position, end_position)

    def p_literal_numeral(self, p):
        """literal : TK_NUMERAL"""
        self._p_literal_common(p, ast.LiteralKind.NUMERAL)

    def p_literal_bool_const(self, p):
        """literal : TK_BOOL_CONST"""
        self._p_literal_common(p, ast.LiteralKind.BOOLEAN)

    def p_literal_decimal(self, p):
        """literal : TK_DECIMAL"""
        self._p_literal_common(p, ast.LiteralKind.DECIMAL)

    def p_literal_hex_const(self, p):
        """literal : TK_HEX_CONST"""
        self._p_literal_common(p, ast.LiteralKind.HEXADECIMAL)

    def p_literal_bin_const(self, p):
        """literal : TK_BIN_CONST"""
        self._p_literal_common(p, ast.LiteralKind.BINARY)

    def p_literal_string(self, p):
        """literal : TK_STRING_LITERAL"""
        self._p_literal_common(p, ast.LiteralKind.STRING)

    def p_arg_list(self, p):
        """arg_list : TK_LPAREN sorted_symbol_star TK_RPAREN"""
        p[0] = p[2]

    def p_sorted_symbol_star(self, p):
        """sorted_symbol_star : sorted_symbol_star sorted_symbol
                              | """
        if len(p) == 1:
            p[0] = []
        else:
            p[0] = p[1] + [p[2]]

    def p_nonempty_arg_list(self, p):
        """nonempty_arg_list : TK_LPAREN sorted_symbol_plus TK_RPAREN"""
        p[0] = p[2]

    def p_sorted_symbol_plus(self, p):
        """sorted_symbol_plus : sorted_symbol_plus sorted_symbol
                              | sorted_symbol"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_sorted_symbol(self, p):
        """sorted_symbol : TK_LPAREN TK_SYMBOL sort_expr TK_RPAREN"""
        p[0] = (p[2], p[3])

    def p_error(self, p):
        if p:
            f = StringIO()
            f.write(f'Syntax error at token: {p.type}, {p.value}')
            location = self._get_position(p.lineno, p.lexpos)
            raise exceptions.ParseException(f.getvalue(), location, location)
        else:
            raise SyntaxError()

    def __init__(self):
        self.parser = ply.yacc.yacc(debug=False, module=self, start="program")
        self.input_string = None
        self.lexer = None
        self._ast_root = None

    def _parse(self, reset: bool = True):
        self.parser.parse(self.input_string, lexer=self.lexer.lexer)
        if not reset:
            return self._ast_root
        self.input_string = None
        result = self._ast_root
        self._ast_root = None
        return result

    @abstractmethod
    def parse(self, input_string):
        raise NotImplementedError
