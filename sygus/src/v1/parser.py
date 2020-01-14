import ply.yacc

from io import StringIO

from .. import ast,  exceptions, utilities
from ..base.parser import SygusParserBase

from .lexer import SygusV1Lexer


# noinspection PyMethodMayBeStatic
class SygusV1Parser(SygusParserBase):
    tokens = SygusV1Lexer.tokens

    def p_command(self, p):
        """command : fun_def_command
                   | fun_decl_command
                   | synth_fun_command
                   | synth_inv_command
                   | check_synth_command
                   | constraint_command
                   | inv_constraint_command
                   | sort_def_command
                   | set_opts_command
                   | var_decl_command
                   | primed_var_decl_command"""
        p[0] = p[1]

    def p_fun_def_command(self, p):
        """fun_def_command : TK_LPAREN TK_DEFINE_FUN symbol arg_list sort_expr term TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        p[0] = ast.DefineFunCommand(p[3], p[4], p[5], p[6], start_position, end_position)

    def p_fun_decl_command(self, p):
        """fun_decl_command : TK_LPAREN TK_DECLARE_FUN symbol TK_LPAREN sort_star TK_RPAREN sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(8), p.lexpos(8))
        p[0] = ast.DeclareFunCommand(p[3], p[5], p[7], start_position, end_position)

    def p_synth_fun_command(self, p):
        """synth_fun_command : TK_LPAREN TK_SYNTH_FUN symbol arg_list sort_expr TK_LPAREN nonterminal_def_plus TK_RPAREN TK_RPAREN
                             | TK_LPAREN TK_SYNTH_FUN symbol arg_list sort_expr TK_RPAREN"""
        if len(p) == 10:
            grammar_start_position = self._get_position(p.lineno(6), p.lexpos(6) - 1)
            grammar_end_position = self._get_position(p.lineno(8), p.lexpos(8))
            grouped_rule_lists = p[7]
            nonterminals = [(l.head_symbol, l.head_symbol_sort_expression) for l in grouped_rule_lists]
            grammar = ast.Grammar(nonterminals, grouped_rule_lists, grammar_start_position, grammar_end_position)
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(9), p.lexpos(9))
        else:
            grammar = None
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(6), p.lexpos(6))

        p[0] = ast.SynthFunCommand(p[3], p[4], p[5], grammar, start_position, end_position)

    def p_synth_inv_command(self, p):
        """synth_inv_command : TK_LPAREN TK_SYNTH_INV symbol arg_list TK_LPAREN nonterminal_def_plus TK_RPAREN TK_RPAREN
                            | TK_LPAREN TK_SYNTH_INV symbol arg_list TK_RPAREN"""
        if len(p) == 9:
            grammar_start_position = self._get_position(p.lineno(5), p.lexpos(5) - 1)
            grammar_end_position = self._get_position(p.lineno(7), p.lexpos(7))
            grouped_rule_lists = p[6]
            nonterminals = [(l.head_symbol, l.head_symbol_sort_expression) for l in grouped_rule_lists]
            grammar = ast.Grammar(nonterminals, grouped_rule_lists, grammar_start_position, grammar_end_position)
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(8), p.lexpos(8))
        else:
            grammar = None
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(5), p.lexpos(5))

        p[0] = ast.SynthInvCommand(p[3], p[4], grammar, start_position, end_position)

    def p_sort_def_command(self, p):
        """sort_def_command : TK_LPAREN TK_DEFINE_SORT symbol sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(5), p.lexpos(5))
        p[0] = ast.DefineSortCommand(p[3], [], p[4], start_position, end_position)

    def p_set_opts_command(self, p):
        """set_opts_command : TK_LPAREN TK_SET_OPTIONS option_list TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.SetOptionsCommand(p[3], start_position, end_position)

    def p_primed_var_decl_command(self, p):
        """primed_var_decl_command : TK_LPAREN TK_DECLARE_PRIMED_VAR symbol sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(5), p.lexpos(5))
        p[0] = ast.DeclarePrimedVarCommand(p[3], p[4], start_position, end_position)

    def p_sort_expr_bit_vector(self, p):
        """sort_expr : TK_LPAREN TK_BV TK_NUMERAL TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.SortExpression(ast.Identifier('BitVec', p[3]), [], start_position, end_position)

    def p_sort_expr_int(self, p):
        """sort_expr : TK_INT"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(1), p.lexpos(1))
        p[0] = ast.SortExpression('Int', [], start_position, end_position)

    def p_sort_expr_bool(self, p):
        """sort_expr : TK_BOOL"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(1), p.lexpos(1))
        p[0] = ast.SortExpression('Bool', [], start_position, end_position)

    def p_sort_expr_real(self, p):
        """sort_expr : TK_REAL"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(1), p.lexpos(1))
        p[0] = ast.SortExpression('Real', [], start_position, end_position)

    def p_sort_expr_string(self, p):
        """sort_expr : TK_STRING"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(1), p.lexpos(1))
        p[0] = ast.SortExpression('String', [], start_position, end_position)

    def p_sort_expr_enum(self, p):
        """sort_expr : TK_LPAREN TK_ENUM enum_constructor_list TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.EnumSortExpression(p[3], start_position, end_position)

    def p_sort_expr_array(self, p):
        """sort_expr : TK_LPAREN TK_ARRAY sort_expr sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(5), p.lexpos(5))
        p[0] = ast.SortExpression('Array', [p[3], p[4]], start_position, end_position)

    def p_sort_expr_symbol(self, p):
        """sort_expr : TK_SYMBOL"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(1), p.lexpos(1))
        p[0] = ast.SortExpression(p[1], [], start_position, end_position)

    def p_symbol(self, p):
        """symbol : TK_SYMBOL"""
        p[0] = p[1]

    def p_enum_constructor_list(self, p):
        """enum_constructor_list : TK_LPAREN symbol_plus TK_RPAREN"""
        p[0] = p[2]

    def p_symbol_plus_close_paren(self, p):
        """symbol_plus : symbol_plus symbol
                       | symbol"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_term(self, p):
        """term : literal
                | TK_SYMBOL
                | TK_LPAREN symbol term_star TK_RPAREN
                | TK_LPAREN TK_EXISTS nonempty_arg_list term TK_RPAREN
                | TK_LPAREN TK_FORALL nonempty_arg_list term TK_RPAREN
                | let_term"""
        if len(p) == 2 and isinstance(p[1], ast.Literal):
            p[0] = ast.LiteralTerm(p[1], p[1].start_location, p[1].end_location)
        elif len(p) == 2 and isinstance(p[1], str):
            start_location = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
            end_location = self._get_position(p.lineno(1), p.lexpos(1))
            p[0] = ast.IdentifierTerm(p[1], start_location, end_location)
        elif len(p) == 2:
            p[0] = p[1]
        elif len(p) == 6 and (p[2] == 'forall' or p[2] == 'exists'):
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(5), p.lexpos(5))
            quantifier_kind = ast.QuantifierKind.FORALL if p[2] == 'forall' else ast.QuantifierKind.EXISTS
            p[0] = ast.QuantifiedTerm(quantifier_kind, p[3], p[4], start_position, end_position)
        else:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(4), p.lexpos(4))
            p[0] = ast.FunctionApplicationTerm(p[2], p[3], start_position, end_position)

    def p_let_term(self, p):
        """let_term : TK_LPAREN TK_LET TK_LPAREN binding_plus TK_RPAREN term TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        bindings = [(t[0], t[2]) for t in p[4]]
        type_annotations = {t[0]: t[1] for t in p[4]}
        p[0] = ast.LetTerm(bindings, p[6], start_position, end_position, type_annotations)

    def p_binding_plus(self, p):
        """binding_plus : binding_plus binding
                        | binding"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_binding(self, p):
        """binding : TK_LPAREN symbol sort_expr term TK_RPAREN"""
        p[0] = (p[2], p[3], p[4])

    def p_term_star(self, p):
        """term_star : term_star term
                     | """
        if len(p) == 3:
            p[0] = p[1] + [p[2]]
        else:
            p[0] = []

    def p_literal_enum_const(self, p):
        """literal : enum_const"""
        self._p_literal_common(p, ast.LiteralKind.ENUMERATED)

    def p_enum_const(self, p):
        """enum_const : TK_SYMBOL TK_DOUBLE_COLON TK_SYMBOL"""
        p[0] = p[1] + '::' + p[3]

    def p_sort_star(self, p):
        """sort_star : sort_star sort_expr
                     | """
        if len(p) == 3:
            p[0] = p[1] + [p[2]]
        else:
            p[0] = []

    def p_option_list(self, p):
        """option_list : TK_LPAREN symbol_pair_plus TK_RPAREN"""
        p[0] = p[2]

    def p_symbol_pair_plus(self, p):
        """symbol_pair_plus : symbol_pair_plus symbol_pair
                            | symbol_pair"""
        if len(p) == 3:
            p[0] = p[1] + [p[2]]
        else:
            p[0] = [p[1]]

    def p_symbol_pair(self, p):
        """symbol_pair : TK_LPAREN symbol TK_STRING_LITERAL TK_RPAREN"""
        p[0] = (p[2], p[3])

    def p_nonterminal_def_plus(self, p):
        """nonterminal_def_plus : nonterminal_def_plus nonterminal_def
                                | nonterminal_def"""
        if len(p) == 3:
            p[0] = p[1] + [p[2]]
        else:
            p[0] = [p[1]]

    def p_nonterminal_def(self, p):
        """nonterminal_def : TK_LPAREN symbol sort_expr TK_LPAREN grammar_term_plus TK_RPAREN TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(str(p[1])))
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        p[0] = ast.GroupedRuleList(p[2], p[3], p[5], start_position, end_position)

    def p_grammar_term_plus(self, p):
        """grammar_term_plus : grammar_term_plus grammar_term
                             | grammar_term"""
        if len(p) == 3:
            p[0] = p[1] + [p[2]]
        else:
            p[0] = [p[1]]

    def p_grammar_term_symbol(self, p):
        """grammar_term : TK_SYMBOL"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(str(p[1])))
        end_position = self._get_position(p.lineno(1), p.lexpos(1))
        term = ast.IdentifierTerm(p[1], start_position, end_position)
        p[0] = ast.GrammarTerm.create_binder_free_grammar_term(term, term.start_location, term.end_location)

    def p_grammar_term_literal(self, p):
        """grammar_term : literal"""
        term = ast.LiteralTerm(p[1], p[1].start_location, p[1].end_location)
        p[0] = ast.GrammarTerm.create_binder_free_grammar_term(term, term.start_location, term.end_location)

    def p_grammar_term_function_application(self, p):
        """grammar_term : TK_LPAREN symbol grammar_term_star TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        term = ast.FunctionApplicationTerm(p[2], p[3], start_position, end_position)
        p[0] = ast.GrammarTerm.create_binder_free_grammar_term(term, term.start_location, term.end_location)

    def p_grammar_term_constant(self, p):
        """grammar_term : TK_LPAREN TK_CONSTANT sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.GrammarTerm.create_constant_term(p[3], start_position, end_position)

    def p_grammar_term_variable(self, p):
        """grammar_term : TK_LPAREN TK_VARIABLE sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.GrammarTerm.create_variable_term(p[3], start_position, end_position)

    def p_grammar_term_let(self, p):
        """grammar_term : let_grammar_term"""
        p[0] = p[1]

    def p_let_grammar_term(self, p):
        """let_grammar_term : TK_LPAREN TK_LET grammar_let_binding_plus TK_RPAREN grammar_term TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(6), p.lexpos(6))
        raise exceptions.ParseException('Let terms in grammars are not supported.', start_position, end_position)

    def p_grammar_let_binding_plus(self, p):
        """grammar_let_binding_plus : grammar_let_binding_plus grammar_let_binding
                                    | grammar_let_binding"""
        if len(p) == 3:
            p[0] = p[1] + [p[2]]
        else:
            p[0] = [p[1]]

    def p_grammar_let_binding(self, p):
        """grammar_let_binding : TK_LPAREN symbol sort_expr grammar_term TK_RPAREN"""
        p[0] = (p[2], p[3], p[4])

    def p_grammar_term_star(self, p):
        """grammar_term_star : grammar_term_star grammar_term
                             | """
        if len(p) == 3:
            p[0] = p[1] + [p[2]]
        else:
            p[0] = []

    def __init__(self):
        super().__init__()

    def parse(self, input_string):
        self.input_string = input_string
        self.lexer = SygusV1Lexer()
        return self._parse()
