import ply.yacc

from io import StringIO

from .. import ast, exceptions, utilities
from ..base.parser import SygusParserBase

from .lexer import SygusV2Lexer


# noinspection PyMethodMayBeStatic
class SygusV2Parser(SygusParserBase):
    tokens = SygusV2Lexer.tokens

    def p_command(self, p):
        """command : check_synth_command
                   | constraint_command
                   | var_decl_command
                   | inv_constraint_command
                   | set_feature_command
                   | synth_fun_command
                   | synth_inv_command
                   | sort_decl_command
                   | fun_def_command
                   | sort_def_command
                   | set_option_command
                   | declare_datatype_command
                   | declare_datatypes_command"""
        p[0] = p[1]

    def p_set_feature_command(self, p):
        """set_feature_command : TK_LPAREN TK_SET_FEATURE TK_COLON TK_SYMBOL TK_BOOL_CONST TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(6), p.lexpos(6))
        p[0] = ast.SetFeatureCommand(p[4], p[5], start_position, end_position)

    def p_synth_fun_command(self, p):
        """synth_fun_command : TK_LPAREN TK_SYNTH_FUN TK_SYMBOL arg_list sort_expr opt_grammar_def TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        p[0] = ast.SynthFunCommand(p[3], p[4], p[5], p[6], start_position, end_position)

    def p_synth_inv_command(self, p):
        """synth_inv_command : TK_LPAREN TK_SYNTH_INV TK_SYMBOL arg_list opt_grammar_def TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(6), p.lexpos(6))
        p[0] = ast.SynthInvCommand(p[3], p[4], p[5], start_position, end_position)

    def p_sort_decl_command(self, p):
        """sort_decl_command : TK_LPAREN TK_DECLARE_SORT TK_SYMBOL TK_NUMERAL TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(5), p.lexpos(5))
        p[0] = ast.DeclareSortCommand(p[3], p[4], start_position, end_position)

    def p_fun_def_command(self, p):
        """fun_def_command : TK_LPAREN TK_DEFINE_FUN TK_SYMBOL arg_list sort_expr term TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        p[0] = ast.DefineFunCommand(p[3], p[4], p[5], p[6], start_position, end_position)

    def p_sort_def_command(self, p):
        """sort_def_command : TK_LPAREN TK_DEFINE_SORT TK_SYMBOL symbol_star sort_expr TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(6), p.lexpos(6))
        p[0] = ast.DefineSortCommand(p[3], p[4], p[5], start_position, end_position)

    def p_set_option_command(self, p):
        """set_option_command : TK_LPAREN TK_SET_OPTION TK_COLON TK_SYMBOL literal TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(6), p.lexpos(6))
        p[0] = ast.SetOptionCommand(p[4], p[5], start_position, end_position)

    def p_declare_datatype_command(self, p):
        """declare_datatype_command : TK_LPAREN TK_DECLARE_DATATYPE TK_SYMBOL datatype_constructor_list TK_RPAREN
                                    | TK_LPAREN TK_DECLARE_DATATYPE TK_LPAREN TK_SYMBOL TK_NUMERAL TK_RPAREN datatype_constructor_list TK_RPAREN"""
        if len(p) == 6:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(5), p.lexpos(5))
            p[0] = ast.DeclareDatatypeCommand(p[3], 0, p[4], start_position, end_position)
        else:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(8), p.lexpos(8))
            p[0] = ast.DeclareDatatypeCommand(p[4], p[5], p[7], start_position, end_position)

    def p_declare_datatypes_command(self, p):
        """declare_datatypes_command : TK_LPAREN TK_DECLARE_DATATYPES TK_LPAREN sort_name_and_arity_plus TK_RPAREN TK_LPAREN datatype_constructor_list_plus TK_RPAREN TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(8), p.lexpos(8))
        p[0] = ast.DeclareDatatypesCommand(p[4], p[7], start_position, end_position)

    def p_term(self, p):
        """term : identifier
                | literal
                | TK_LPAREN identifier term_star TK_RPAREN
                | TK_LPAREN TK_EXISTS nonempty_arg_list term TK_RPAREN
                | TK_LPAREN TK_FORALL nonempty_arg_list term TK_RPAREN
                | TK_LPAREN TK_LET TK_LPAREN binding_plus TK_RPAREN term TK_RPAREN"""
        if len(p) == 2 and isinstance(p[1], utilities.Identifier):
            p[0] = ast.IdentifierTerm(p[1], p[1].start_location, p[1].end_location)
        elif len(p) == 2 and isinstance(p[1], ast.Literal):
            p[0] = ast.LiteralTerm(p[1], p[1].start_location, p[1].end_location)
        elif len(p) == 5:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(4), p.lexpos(4))
            p[0] = ast.FunctionApplicationTerm(p[2], p[3], start_position, end_position)
        elif len(p) == 6 and (p[2] == 'forall' or p[2] == 'exists'):
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(5), p.lexpos(5))
            quantifier_kind = ast.QuantifierKind.FORALL if p[2] == 'forall' else ast.QuantifierKind.EXISTS
            p[0] = ast.QuantifiedTerm(quantifier_kind, p[3], p[4], start_position, end_position)
        else:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(7), p.lexpos(7))
            p[0] = ast.LetTerm(p[4], p[6], start_position, end_position)

    def p_sort_expr(self, p):
        """sort_expr : identifier
                     | TK_LPAREN identifier sort_expr_plus TK_RPAREN"""
        if len(p) == 2:
            p[0] = ast.SortExpression(p[1], [], p[1].start_location, p[1].end_location)
        else:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(4), p.lexpos(4))
            p[0] = ast.SortExpression(p[2], p[3], start_position, end_position)

    def p_term_plus(self, p):
        """term_plus : term_plus term
                     | term"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_term_star(self, p):
        """term_star : term_plus
                     | """
        if len(p) == 1:
            p[0] = []
        else:
            p[0] = p[1]

    def p_sort_expr_plus(self, p):
        """sort_expr_plus : sort_expr_plus sort_expr
                          | sort_expr"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_opt_grammar_def(self, p):
        """opt_grammar_def : grammar_def
                           | """
        if len(p) == 2:
            p[0] = p[1]
        else:
            p[0] = None

    def p_grammar_def(self, p):
        """grammar_def : nonempty_arg_list TK_LPAREN grouped_rule_list_plus TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.Grammar(p[1], p[3], start_position, end_position)

    def p_grouped_rule_list_plus(self, p):
        """grouped_rule_list_plus : grouped_rule_list_plus grouped_rule_list
                                  | grouped_rule_list"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_grouped_rule_list(self, p):
        """grouped_rule_list : TK_LPAREN TK_SYMBOL sort_expr TK_LPAREN grammar_term_plus TK_RPAREN TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        p[0] = ast.GroupedRuleList(p[2], p[3], p[5], start_position, end_position)

    def p_grammar_term_plus(self, p):
        """grammar_term_plus : grammar_term_plus grammar_term
                             | grammar_term"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_grammar_term(self, p):
        """grammar_term : TK_LPAREN TK_CONSTANT sort_expr TK_RPAREN
                        | TK_LPAREN TK_VARIABLE sort_expr TK_RPAREN
                        | binder_free_term"""
        if len(p) == 5:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(4), p.lexpos(4))
            if p[2] == 'Constant':
                p[0] = ast.GrammarTerm.create_constant_term(p[3], start_position, end_position)
            else:
                p[0] = ast.GrammarTerm.create_variable_term(p[3], start_position, end_position)
        else:
            p[0] = ast.GrammarTerm.create_binder_free_grammar_term(p[1], p[1].start_location, p[1].end_location)

    def p_binder_free_term(self, p):
        """binder_free_term : identifier
                            | literal
                            | TK_LPAREN identifier binder_free_term_plus TK_RPAREN"""
        if len(p) == 2 and isinstance(p[1], utilities.Identifier):
            p[0] = ast.IdentifierTerm(p[1], p[1].start_location, p[1].end_location)
        elif len(p) == 2 and isinstance(p[1], ast.Literal):
            p[0] = ast.LiteralTerm(p[1], p[1].start_location, p[1].end_location)
        else:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - 1)
            end_position = self._get_position(p.lineno(4), p.lexpos(4))
            p[0] = ast.FunctionApplicationTerm(p[2], p[3], start_position, end_position)

    def p_binder_free_term_plus(self, p):
        """binder_free_term_plus : binder_free_term_plus binder_free_term
                                 | binder_free_term"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_symbol_star(self, p):
        """symbol_star : symbol_plus
                       | TK_LPAREN TK_RPAREN"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = []

    def p_symbol_plus(self, p):
        """symbol_plus : TK_LPAREN symbol_plus TK_SYMBOL TK_RPAREN
                       | TK_LPAREN TK_SYMBOL TK_RPAREN"""
        if len(p) == 4:
            p[0] = [p[2]]
        else:
            p[0] = p[2] + [p[3]]

    def p_binding_plus(self, p):
        """binding_plus : binding_plus binding
                        | binding"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_binding(self, p):
        """binding : TK_LPAREN TK_SYMBOL term TK_RPAREN"""
        p[0] = (p[2], p[3])

    def p_identifier(self, p):
        """identifier : TK_SYMBOL
                      | TK_LPAREN TK_UNDERSCORE TK_SYMBOL index_plus TK_RPAREN"""
        if len(p) == 2:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
            end_position = self._get_position(p.lineno(1), p.lexpos(1))
            p[0] = ast.Identifier(p[1])
            p[0].start_location = start_position
            p[0].end_location = end_position
        else:
            start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
            end_position = self._get_position(p.lineno(5), p.lexpos(5))
            p[0] = ast.Identifier(p[3], *p[4])
            p[0].start_location = start_position
            p[0].end_location = end_position

    def p_index_plus(self, p):
        """index_plus : index_plus index
                      | index"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_index(self, p):
        """index : TK_NUMERAL
                 | TK_SYMBOL"""
        p[0] = p[1]

    def p_sort_name_and_arity_plus(self, p):
        """sort_name_and_arity_plus : sort_name_and_arity_plus sort_name_and_arity
                                    | sort_name_and_arity"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_sort_name_and_arity(self, p):
        """sort_name_and_arity : TK_LPAREN TK_SYMBOL TK_NUMERAL TK_RPAREN"""
        p[0] = (p[2], p[3])

    def p_datatype_constructor_list_plus(self, p):
        """datatype_constructor_list_plus : datatype_constructor_list_plus datatype_constructor_list
                                          | datatype_constructor_list"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_datatype_constructor_list(self, p):
        """datatype_constructor_list : parameterized_datatype_constructor_list
                                     | simple_datatype_constructor_list"""
        p[0] = p[1]

    def p_parameterized_datatype_constructor_list(self, p):
        """parameterized_datatype_constructor_list : TK_LPAREN TK_PAR symbol_plus TK_LPAREN constructor_definition_plus TK_RPAREN TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(7), p.lexpos(7))
        p[0] = ast.DatatypeConstructorList(p[3], p[5], start_position, end_position)

    def p_simple_datatype_constructor_list(self, p):
        """simple_datatype_constructor_list : TK_LPAREN constructor_definition_plus TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(3), p.lexpos(3))
        p[0] = ast.DatatypeConstructorList([], p[2], start_position, end_position)

    def p_constructor_definition_plus(self, p):
        """constructor_definition_plus : constructor_definition_plus constructor_definition
                                       | constructor_definition"""
        if len(p) == 2:
            p[0] = [p[1]]
        else:
            p[0] = p[1] + [p[2]]

    def p_constructor_definition(self, p):
        """constructor_definition : TK_LPAREN TK_SYMBOL sorted_symbol_star TK_RPAREN"""
        start_position = self._get_position(p.lineno(1), p.lexpos(1) - len(p[1]))
        end_position = self._get_position(p.lineno(4), p.lexpos(4))
        p[0] = ast.DatatypeConstructor(p[2], p[3], start_position, end_position)

    def __init__(self):
        super().__init__()

    def parse(self, input_string):
        self.input_string = input_string
        self.lexer = SygusV2Lexer()
        return self._parse()
