/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SRC_SYNTHLIB2PARSER_TAB_H_INCLUDED
# define YY_YY_SRC_SYNTHLIB2PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_DEFINE_SORT = 258,
     TK_DEFINE_FUN = 259,
     TK_DECLARE_FUN = 260,
     TK_SET_OPTIONS = 261,
     TK_CHECK_SYNTH = 262,
     TK_SYNTH_FUN = 263,
     TK_SYNTH_INV = 264,
     TK_DECLARE_VAR = 265,
     TK_DECLARE_PRIMED_VAR = 266,
     TK_LPAREN = 267,
     TK_RPAREN = 268,
     TK_SET_LOGIC = 269,
     TK_BV = 270,
     TK_INT = 271,
     TK_BOOL = 272,
     TK_ENUM = 273,
     TK_STRING = 274,
     TK_CONSTRAINT = 275,
     TK_INV_CONSTRAINT = 276,
     TK_CONSTANT = 277,
     TK_VARIABLE = 278,
     TK_LOCAL_VARIABLE = 279,
     TK_INPUT_VARIABLE = 280,
     TK_ERROR = 281,
     TK_DOUBLECOLON = 282,
     TK_LET = 283,
     TK_ARRAY = 284,
     TK_REAL = 285,
     TK_INT_LITERAL = 286,
     TK_REAL_LITERAL = 287,
     TK_BOOL_LITERAL = 288,
     TK_ENUM_LITERAL = 289,
     TK_BV_LITERAL = 290,
     TK_STRING_LITERAL = 291,
     TK_SYMBOL = 292,
     TK_QUOTED_LITERAL = 293
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 36 "./src/SynthLib2Parser.y"

    // yyunion fields
    string* LexerString;
    pair<string, string>* EnumConstant;
    const SortExpr* TheSortExpr;
    Literal* TheLiteral;
    vector<string>* SymbolVector;
    ArgSortPair* SymbolSortPair;
    vector<const SortExpr*>* SortVector;
    vector<ArgSortPair*>* SymbolSortVector;
    pair<string, string>* SymbolSymbolPair;
    vector<pair<string, string> >* SymbolSymbolVector;
    Term* TheTerm;
    vector<Term*>* TermVector;
    LetBindingTerm* STPair;
    vector<LetBindingTerm*>* STPairVector;
    vector<NTDef*>* NTDefVector;
    NTDef* TheNTDef;
    GTerm* TheGTerm;
    vector<GTerm*>* GTermVector;
    LetBindingGTerm* SGPair;
    vector<LetBindingGTerm*>* SGPairVector;
    Program* TheProgram;
    ASTCmd* TheCmd;
    vector<ASTCmd*>* CmdList;


/* Line 2058 of yacc.c  */
#line 123 "./src/SynthLib2Parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_SRC_SYNTHLIB2PARSER_TAB_H_INCLUDED  */
