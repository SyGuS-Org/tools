/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "./src/SynthLib2Parser.y"

    #include <SynthLib2ParserIFace.hpp>
    #include <SymbolTable.hpp>
    #include <iostream>
    #include <string.h>
    #include <boost/lexical_cast.hpp>
    #include <LogicSymbols.hpp>
    
    using namespace std;
    using namespace SynthLib2Parser;

    namespace SynthLib2Bison {
        Program* TheProgram = NULL;
    }

    long yylinenum = 1;
    long yycolnum = 0;
    extern char* yytext;

    extern int yylex(void);
    int yyerror(char* s)
    {
        cerr << "Parse error: Last token read was: " << yytext
             << " at line: " << yylinenum << ", column: " 
             << yycolnum - strlen(yytext) << endl;
        cerr.flush();
        exit(1);
    }

    static inline SourceLocation GetCurrentLocation()
    {
        return SourceLocation(yylinenum, yycolnum);
    }

/* Line 371 of yacc.c  */
#line 103 "src/SynthLib2Parser.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "SynthLib2Parser.tab.h".  */
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
/* Line 387 of yacc.c  */
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


/* Line 387 of yacc.c  */
#line 212 "src/SynthLib2Parser.tab.cpp"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 240 "src/SynthLib2Parser.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  30
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   235

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  95
/* YYNRULES -- Number of states.  */
#define YYNSTATES  199

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    13,    18,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    51,    57,    63,    68,    70,    72,    74,    76,
      81,    87,    89,    91,    93,    95,    99,   101,   103,   107,
     110,   112,   117,   121,   124,   126,   131,   139,   148,   151,
     152,   156,   159,   160,   165,   170,   172,   174,   176,   184,
     187,   189,   195,   198,   199,   201,   203,   205,   207,   209,
     211,   214,   216,   224,   227,   229,   233,   238,   246,   256,
     263,   272,   278,   280,   282,   287,   292,   297,   302,   307,
     309,   317,   320,   322,   328,   331
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    41,    -1,    -1,    43,    44,    -1,    44,
      -1,    37,    -1,    12,    14,    42,    13,    -1,    44,    45,
      -1,    45,    -1,    62,    -1,    63,    -1,    80,    -1,    81,
      -1,    77,    -1,    78,    -1,    79,    -1,    48,    -1,    58,
      -1,    46,    -1,    47,    -1,    12,    10,    42,    49,    13,
      -1,    12,    11,    42,    49,    13,    -1,    12,     3,    42,
      49,    13,    -1,    12,    15,    50,    13,    -1,    16,    -1,
      17,    -1,    30,    -1,    19,    -1,    12,    18,    56,    13,
      -1,    12,    29,    49,    49,    13,    -1,    42,    -1,    31,
      -1,    33,    -1,    35,    -1,    42,    27,    42,    -1,    32,
      -1,    36,    -1,    12,    57,    13,    -1,    57,    42,    -1,
      42,    -1,    12,     6,    59,    13,    -1,    12,    60,    13,
      -1,    60,    61,    -1,    61,    -1,    12,    42,    38,    13,
      -1,    12,     4,    42,    65,    49,    68,    13,    -1,    12,
       5,    42,    12,    64,    13,    49,    13,    -1,    64,    49,
      -1,    -1,    12,    66,    13,    -1,    66,    67,    -1,    -1,
      12,    42,    49,    13,    -1,    12,    42,    72,    13,    -1,
      73,    -1,    42,    -1,    69,    -1,    12,    28,    12,    70,
      13,    68,    13,    -1,    70,    71,    -1,    71,    -1,    12,
      42,    49,    68,    13,    -1,    72,    68,    -1,    -1,    50,
      -1,    51,    -1,    52,    -1,    53,    -1,    54,    -1,    55,
      -1,    74,    75,    -1,    75,    -1,    12,    42,    49,    12,
      76,    13,    13,    -1,    76,    82,    -1,    82,    -1,    12,
       7,    13,    -1,    12,    20,    68,    13,    -1,    12,    21,
      42,    42,    42,    42,    13,    -1,    12,     8,    42,    65,
      49,    12,    74,    13,    13,    -1,    12,     8,    42,    65,
      49,    13,    -1,    12,     9,    42,    65,    12,    74,    13,
      13,    -1,    12,     9,    42,    65,    13,    -1,    42,    -1,
      73,    -1,    12,    42,    86,    13,    -1,    12,    22,    49,
      13,    -1,    12,    23,    49,    13,    -1,    12,    25,    49,
      13,    -1,    12,    24,    49,    13,    -1,    83,    -1,    12,
      28,    12,    84,    13,    82,    13,    -1,    84,    85,    -1,
      85,    -1,    12,    42,    49,    82,    13,    -1,    86,    82,
      -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   111,   111,   114,   119,   127,   133,   138,   147,   152,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   170,   177,   184,   195,   205,   209,   213,   217,   221,
     226,   231,   237,   242,   247,   252,   260,   265,   272,   277,
     283,   290,   296,   301,   307,   314,   321,   330,   338,   341,
     343,   348,   354,   359,   365,   371,   375,   380,   385,   391,
     396,   402,   409,   415,   419,   424,   429,   438,   450,   455,
     461,   466,   472,   480,   485,   491,   496,   501,   511,   523,
     531,   540,   548,   553,   557,   563,   567,   571,   575,   579,
     584,   590,   595,   601,   608,   614
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_DEFINE_SORT", "TK_DEFINE_FUN",
  "TK_DECLARE_FUN", "TK_SET_OPTIONS", "TK_CHECK_SYNTH", "TK_SYNTH_FUN",
  "TK_SYNTH_INV", "TK_DECLARE_VAR", "TK_DECLARE_PRIMED_VAR", "TK_LPAREN",
  "TK_RPAREN", "TK_SET_LOGIC", "TK_BV", "TK_INT", "TK_BOOL", "TK_ENUM",
  "TK_STRING", "TK_CONSTRAINT", "TK_INV_CONSTRAINT", "TK_CONSTANT",
  "TK_VARIABLE", "TK_LOCAL_VARIABLE", "TK_INPUT_VARIABLE", "TK_ERROR",
  "TK_DOUBLECOLON", "TK_LET", "TK_ARRAY", "TK_REAL", "TK_INT_LITERAL",
  "TK_REAL_LITERAL", "TK_BOOL_LITERAL", "TK_ENUM_LITERAL", "TK_BV_LITERAL",
  "TK_STRING_LITERAL", "TK_SYMBOL", "TK_QUOTED_LITERAL", "$accept",
  "start", "Prog", "Symbol", "SetLogicCmd", "CmdPlus", "Cmd", "VarDeclCmd",
  "PrimedVarDeclCmd", "SortDefCmd", "SortExpr", "IntConst", "BoolConst",
  "BVConst", "EnumConst", "RealConst", "StringConst", "ECList",
  "SymbolPlus", "SetOptsCmd", "OptList", "SymbolPairPlus", "SymbolPair",
  "FunDefCmd", "FunDeclCmd", "SortStar", "ArgList", "SymbolSortPairStar",
  "SymbolSortPair", "Term", "LetTerm", "LetBindingTermPlus",
  "LetBindingTerm", "TermStar", "Literal", "NTDefPlus", "NTDef",
  "GTermPlus", "CheckSynthCmd", "ConstraintCmd", "InvConstraintCmd",
  "SynthFunCmd", "SynthInvCmd", "GTerm", "LetGTerm", "LetBindingGTermPlus",
  "LetBindingGTerm", "GTermStar", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    40,    41,    41,    42,    43,    44,    44,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    46,    47,    48,    49,    49,    49,    49,    49,    49,
      49,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      57,    58,    59,    60,    60,    61,    62,    63,    64,    64,
      65,    66,    66,    67,    68,    68,    68,    68,    69,    70,
      70,    71,    72,    72,    73,    73,    73,    73,    73,    73,
      74,    74,    75,    76,    76,    77,    78,    79,    80,    80,
      81,    81,    82,    82,    82,    82,    82,    82,    82,    82,
      83,    84,    84,    85,    86,    86
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     4,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     5,     5,     4,     1,     1,     1,     1,     4,
       5,     1,     1,     1,     1,     3,     1,     1,     3,     2,
       1,     4,     3,     2,     1,     4,     7,     8,     2,     0,
       3,     2,     0,     4,     4,     1,     1,     1,     7,     2,
       1,     5,     2,     0,     1,     1,     1,     1,     1,     1,
       2,     1,     7,     2,     1,     3,     4,     7,     9,     6,
       8,     5,     1,     1,     4,     4,     4,     4,     4,     1,
       7,     2,     1,     5,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     2,     0,     5,     9,    19,    20,    17,
      18,    10,    11,    14,    15,    16,    12,    13,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     0,     4,     8,     6,     0,     0,     0,     0,     0,
      75,     0,     0,     0,     0,     0,     0,    32,    36,    33,
      34,    37,    56,    64,    65,    66,    67,    68,    69,     0,
      57,    55,     0,     0,    25,    26,    28,    27,    31,     0,
      52,     0,    49,     0,     0,    44,    41,     0,     0,     0,
       0,     7,     0,    63,     0,    76,     0,     0,     0,     0,
      23,     0,     0,     0,     0,    42,    43,     0,     0,    81,
      21,    22,     0,     0,    35,     0,     0,     0,     0,     0,
       0,    50,    51,     0,     0,    48,     0,     0,    79,     0,
       0,    71,     0,     0,    60,    54,    62,     0,    24,    40,
       0,    29,     0,     0,    46,     0,    45,     0,     0,     0,
      70,     0,     0,    59,    77,    38,    39,    30,     0,    47,
       0,     0,    80,     0,     0,    53,    78,     0,     0,    58,
       0,    82,    83,     0,    74,    89,    61,     0,     0,     0,
       0,     0,    95,     0,    73,     0,     0,     0,     0,     0,
       0,    72,    85,    86,    88,    87,     0,     0,    92,    84,
      94,     0,     0,    91,     0,     0,     0,    90,    93
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    68,     4,     5,     6,     7,     8,     9,
      69,    53,    54,    55,    56,    57,    58,   108,   130,    10,
      39,    74,    75,    11,    12,    93,    71,    91,   112,    59,
      60,   123,   124,   103,    61,   120,   121,   163,    13,    14,
      15,    16,    17,   164,   165,   187,   188,   180
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -147
static const yytype_int16 yypact[] =
{
      21,   189,    36,  -147,    31,    31,  -147,  -147,  -147,  -147,
    -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,    15,    15,
      15,    33,    50,    15,    15,    15,    15,    15,   115,    15,
    -147,   208,    31,  -147,  -147,   154,    42,    55,    61,    67,
    -147,    42,    42,   154,   154,    71,    -8,  -147,  -147,  -147,
    -147,  -147,    76,  -147,  -147,  -147,  -147,  -147,  -147,    73,
    -147,  -147,    15,     1,  -147,  -147,  -147,  -147,  -147,    75,
    -147,   154,  -147,    15,    10,  -147,  -147,   154,    12,    80,
      92,  -147,    94,  -147,    15,  -147,    15,    77,    95,   154,
    -147,    14,   115,   106,    79,  -147,  -147,    29,   104,  -147,
    -147,  -147,   108,    25,  -147,    15,   117,    15,   118,   154,
      15,  -147,  -147,   119,   154,  -147,   120,   104,  -147,    15,
      38,  -147,    15,    57,  -147,  -147,  -147,   121,  -147,  -147,
       2,  -147,   124,   154,  -147,   125,  -147,    59,   154,   131,
    -147,   154,   115,  -147,  -147,  -147,  -147,  -147,   136,  -147,
     140,   116,  -147,   115,   141,  -147,  -147,   128,   156,  -147,
     198,    76,  -147,    63,  -147,  -147,  -147,   154,   154,   154,
     154,   129,  -147,   159,  -147,   162,   164,   165,   167,   155,
      78,  -147,  -147,  -147,  -147,  -147,    15,    66,  -147,  -147,
    -147,   154,   128,  -147,   128,   168,   169,  -147,  -147
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -147,  -147,  -147,   -18,  -147,   179,     8,  -147,  -147,  -147,
     -12,    98,  -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,
    -147,  -147,   112,  -147,  -147,  -147,    41,  -147,  -147,   -89,
    -147,  -147,    64,  -147,  -145,    72,  -116,  -147,  -147,  -147,
    -147,  -147,  -147,  -146,  -147,  -147,     3,  -147
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      35,    36,    37,   113,   140,    41,    42,    43,    44,    45,
      52,    62,   162,    33,   126,   145,    87,   174,   162,    88,
      82,   140,    73,    95,    98,    99,   110,   111,    83,    34,
      89,    79,    80,     1,   190,   162,    30,    46,   125,    34,
      33,   117,   118,    31,    86,    38,   195,   162,   196,   162,
     119,   139,    34,   154,    70,    94,    47,    48,    49,    92,
      50,    51,    34,    40,   158,    97,   104,    72,   105,   122,
     142,   119,   150,    73,    52,   160,   173,   109,   186,   192,
      76,   115,    77,    78,    81,    52,    85,   127,    90,   129,
     160,   189,   133,   100,    47,    48,    49,   132,    50,    51,
      34,   138,   135,    84,   141,   101,   102,   107,    47,    47,
      48,    49,   146,    50,    51,    34,   119,   116,    63,   114,
     122,   148,    64,    65,    52,    66,   151,    46,   157,   153,
     128,   131,   134,   136,   144,    52,    67,   147,   149,   161,
     160,   179,   172,    34,   152,   161,    47,    48,    49,   155,
      50,    51,    34,   156,   159,   175,   176,   177,   178,    47,
      48,    49,   161,    50,    51,    34,    63,   186,   191,   166,
      64,    65,   181,    66,   161,   182,   161,   183,   184,   194,
     185,   197,   198,    32,    67,   106,    96,   143,     0,   137,
     193,    34,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,    28,
      29,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     167,   168,   169,   170,     0,     0,   171,     0,    28,    29,
       0,     0,     0,     0,     0,    34
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-147)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      18,    19,    20,    92,   120,    23,    24,    25,    26,    27,
      28,    29,   157,     5,   103,    13,    15,   163,   163,    18,
      28,   137,    12,    13,    12,    13,    12,    13,    46,    37,
      29,    43,    44,    12,   180,   180,     0,    12,    13,    37,
      32,    12,    13,    12,    62,    12,   192,   192,   194,   194,
      12,    13,    37,   142,    12,    73,    31,    32,    33,    71,
      35,    36,    37,    13,   153,    77,    84,    12,    86,    12,
      13,    12,    13,    12,    92,    12,    13,    89,    12,    13,
      13,    93,    41,    42,    13,   103,    13,   105,    13,   107,
      12,    13,   110,    13,    31,    32,    33,   109,    35,    36,
      37,   119,   114,    27,   122,    13,    12,    12,    31,    31,
      32,    33,   130,    35,    36,    37,    12,    38,    12,    13,
      12,   133,    16,    17,   142,    19,   138,    12,    12,   141,
      13,    13,    13,    13,    13,   153,    30,    13,    13,   157,
      12,    12,   160,    37,    13,   163,    31,    32,    33,    13,
      35,    36,    37,    13,    13,   167,   168,   169,   170,    31,
      32,    33,   180,    35,    36,    37,    12,    12,   186,    13,
      16,    17,    13,    19,   192,    13,   194,    13,    13,   191,
      13,    13,    13,     4,    30,    87,    74,   123,    -1,   117,
     187,    37,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    20,
      21,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      22,    23,    24,    25,    -1,    -1,    28,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    37
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    40,    41,    43,    44,    45,    46,    47,    48,
      58,    62,    63,    77,    78,    79,    80,    81,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    14,    20,    21,
       0,    12,    44,    45,    37,    42,    42,    42,    12,    59,
      13,    42,    42,    42,    42,    42,    12,    31,    32,    33,
      35,    36,    42,    50,    51,    52,    53,    54,    55,    68,
      69,    73,    42,    12,    16,    17,    19,    30,    42,    49,
      12,    65,    12,    12,    60,    61,    13,    65,    65,    49,
      49,    13,    28,    42,    27,    13,    42,    15,    18,    29,
      13,    66,    49,    64,    42,    13,    61,    49,    12,    13,
      13,    13,    12,    72,    42,    42,    50,    12,    56,    49,
      12,    13,    67,    68,    13,    49,    38,    12,    13,    12,
      74,    75,    12,    70,    71,    13,    68,    42,    13,    42,
      57,    13,    49,    42,    13,    49,    13,    74,    42,    13,
      75,    42,    13,    71,    13,    13,    42,    13,    49,    13,
      13,    49,    13,    49,    68,    13,    13,    12,    68,    13,
      12,    42,    73,    76,    82,    83,    13,    22,    23,    24,
      25,    28,    42,    13,    82,    49,    49,    49,    49,    12,
      86,    13,    13,    13,    13,    13,    12,    84,    85,    13,
      82,    42,    13,    85,    49,    82,    82,    13,    13
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 112 "./src/SynthLib2Parser.y"
    { SynthLib2Bison::TheProgram = (yyvsp[(1) - (1)].TheProgram); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 114 "./src/SynthLib2Parser.y"
    {   
          vector<ASTCmd*> Dummy;
          SynthLib2Bison::TheProgram = new Program(Dummy);
      }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 120 "./src/SynthLib2Parser.y"
    { 
         vector<ASTCmd*> AllCmds;
         AllCmds.push_back((yyvsp[(1) - (2)].TheCmd));
         AllCmds.insert(AllCmds.end(), (yyvsp[(2) - (2)].CmdList)->begin(), (yyvsp[(2) - (2)].CmdList)->end());
         delete (yyvsp[(2) - (2)].CmdList);
         (yyval.TheProgram) = new Program(AllCmds);
     }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 128 "./src/SynthLib2Parser.y"
    {
         (yyval.TheProgram) = new Program(*(yyvsp[(1) - (1)].CmdList));
         delete (yyvsp[(1) - (1)].CmdList);
     }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 134 "./src/SynthLib2Parser.y"
    {
           (yyval.LexerString) = (yyvsp[(1) - (1)].LexerString);
       }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 139 "./src/SynthLib2Parser.y"
    {
                if(*(yyvsp[(3) - (4)].LexerString) != "LIA" && *(yyvsp[(3) - (4)].LexerString) != "Reals" && *(yyvsp[(3) - (4)].LexerString) != "Arrays" && *(yyvsp[(3) - (4)].LexerString) != "BV" && *(yyvsp[(3) - (4)].LexerString) != "S" && *(yyvsp[(3) - (4)].LexerString) != "SLIA") {
                    throw SynthLib2ParserException("Invalid logic in set logic command");
                }
                (yyval.TheCmd) = new SetLogicCmd(GetCurrentLocation(), *(yyvsp[(3) - (4)].LexerString));
                delete (yyvsp[(3) - (4)].LexerString);
            }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 148 "./src/SynthLib2Parser.y"
    {
            (yyval.CmdList) = (yyvsp[(1) - (2)].CmdList);
            (yyval.CmdList)->push_back((yyvsp[(2) - (2)].TheCmd));
        }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 153 "./src/SynthLib2Parser.y"
    {
            (yyval.CmdList) = new vector<ASTCmd*>();
            (yyval.CmdList)->push_back((yyvsp[(1) - (1)].TheCmd));
        }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 171 "./src/SynthLib2Parser.y"
    {
               (yyval.TheCmd) = new VarDeclCmd(GetCurrentLocation(), 
                                   *(yyvsp[(3) - (5)].LexerString), (yyvsp[(4) - (5)].TheSortExpr));
               delete (yyvsp[(3) - (5)].LexerString);
           }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 178 "./src/SynthLib2Parser.y"
    {
               (yyval.TheCmd) = new PrimedVarDeclCmd(GetCurrentLocation(), 
                                   *(yyvsp[(3) - (5)].LexerString), (yyvsp[(4) - (5)].TheSortExpr));
               delete (yyvsp[(3) - (5)].LexerString);
           }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 185 "./src/SynthLib2Parser.y"
    {
               (yyval.TheCmd) = new SortDefCmd(GetCurrentLocation(),
                                   *(yyvsp[(3) - (5)].LexerString), (yyvsp[(4) - (5)].TheSortExpr));
               if ((yyvsp[(4) - (5)].TheSortExpr)->GetKind() == SORTKIND_ENUM) {
                   auto SortExprAsEnum = dynamic_cast<const EnumSortExpr*>((yyvsp[(4) - (5)].TheSortExpr));
                   SortExprAsEnum->SetEnumSortName(*(yyvsp[(3) - (5)].LexerString));
               }
               delete (yyvsp[(3) - (5)].LexerString);
           }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 196 "./src/SynthLib2Parser.y"
    {
             if (boost::lexical_cast<u32>(*(yyvsp[(3) - (4)].LexerString)) == 0) {
                 throw SynthLib2ParserException("Zero-length bitvectors not supported.\n" +
                                                GetCurrentLocation().ToString());
             }
             (yyval.TheSortExpr) = new BVSortExpr(GetCurrentLocation(),
                                 boost::lexical_cast<u32>(*(yyvsp[(3) - (4)].LexerString)));
             delete (yyvsp[(3) - (4)].LexerString);
         }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 206 "./src/SynthLib2Parser.y"
    {
             (yyval.TheSortExpr) = new IntSortExpr(GetCurrentLocation());
         }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 210 "./src/SynthLib2Parser.y"
    {
             (yyval.TheSortExpr) = new BoolSortExpr(GetCurrentLocation());
         }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 214 "./src/SynthLib2Parser.y"
    {
             (yyval.TheSortExpr) = new RealSortExpr(GetCurrentLocation());
         }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 218 "./src/SynthLib2Parser.y"
    {
             (yyval.TheSortExpr) = new StringSortExpr(GetCurrentLocation());
         }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 222 "./src/SynthLib2Parser.y"
    {
             (yyval.TheSortExpr) = new EnumSortExpr(GetCurrentLocation(), *(yyvsp[(3) - (4)].SymbolVector));
             delete (yyvsp[(3) - (4)].SymbolVector);
         }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 227 "./src/SynthLib2Parser.y"
    {
             (yyval.TheSortExpr) = new ArraySortExpr(GetCurrentLocation(),
                                    (yyvsp[(3) - (5)].TheSortExpr), (yyvsp[(4) - (5)].TheSortExpr));
         }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 232 "./src/SynthLib2Parser.y"
    {
             (yyval.TheSortExpr) = new NamedSortExpr(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString));
             delete (yyvsp[(1) - (1)].LexerString);
         }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 238 "./src/SynthLib2Parser.y"
    {
             (yyval.LexerString) = (yyvsp[(1) - (1)].LexerString);
         }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 243 "./src/SynthLib2Parser.y"
    {
              (yyval.LexerString) = (yyvsp[(1) - (1)].LexerString);
          }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 248 "./src/SynthLib2Parser.y"
    {
            (yyval.LexerString) = (yyvsp[(1) - (1)].LexerString);
        }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 253 "./src/SynthLib2Parser.y"
    {
              auto ConCat = new string(*(yyvsp[(1) - (3)].LexerString) + "::" + *(yyvsp[(3) - (3)].LexerString));
              delete (yyvsp[(1) - (3)].LexerString);
              delete (yyvsp[(3) - (3)].LexerString);
              (yyval.LexerString) = ConCat;
          }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 261 "./src/SynthLib2Parser.y"
    {
              (yyval.LexerString) = (yyvsp[(1) - (1)].LexerString);
          }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 266 "./src/SynthLib2Parser.y"
    {   
              auto StringConst = new string(*(yyvsp[(1) - (1)].LexerString));
	      delete (yyvsp[(1) - (1)].LexerString);
              (yyval.LexerString) = StringConst;
          }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 273 "./src/SynthLib2Parser.y"
    {
           (yyval.SymbolVector) = (yyvsp[(2) - (3)].SymbolVector);
       }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 278 "./src/SynthLib2Parser.y"
    {
               (yyval.SymbolVector) = (yyvsp[(1) - (2)].SymbolVector);
               (yyval.SymbolVector)->push_back(*(yyvsp[(2) - (2)].LexerString));
               delete (yyvsp[(2) - (2)].LexerString);
           }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 284 "./src/SynthLib2Parser.y"
    {
               (yyval.SymbolVector) = new vector<string>();
               (yyval.SymbolVector)->push_back(*(yyvsp[(1) - (1)].LexerString));
               delete (yyvsp[(1) - (1)].LexerString);
           }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 291 "./src/SynthLib2Parser.y"
    {
               (yyval.TheCmd) = new SetOptsCmd(GetCurrentLocation(), *(yyvsp[(3) - (4)].SymbolSymbolVector));
               delete (yyvsp[(3) - (4)].SymbolSymbolVector);
           }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 297 "./src/SynthLib2Parser.y"
    {
            (yyval.SymbolSymbolVector) = (yyvsp[(2) - (3)].SymbolSymbolVector);
        }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 302 "./src/SynthLib2Parser.y"
    {
                   (yyval.SymbolSymbolVector) = (yyvsp[(1) - (2)].SymbolSymbolVector);
                   (yyval.SymbolSymbolVector)->push_back(*(yyvsp[(2) - (2)].SymbolSymbolPair));
                   delete (yyvsp[(2) - (2)].SymbolSymbolPair);
               }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 308 "./src/SynthLib2Parser.y"
    {
                   (yyval.SymbolSymbolVector) = new vector<pair<string, string> >();
                   (yyval.SymbolSymbolVector)->push_back(*(yyvsp[(1) - (1)].SymbolSymbolPair));
                   delete (yyvsp[(1) - (1)].SymbolSymbolPair);
               }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 315 "./src/SynthLib2Parser.y"
    {
               (yyval.SymbolSymbolPair) = new pair<string, string>(*(yyvsp[(2) - (4)].LexerString), (yyvsp[(3) - (4)].LexerString)->substr(1, (yyvsp[(3) - (4)].LexerString)->length() - 2));
               delete (yyvsp[(2) - (4)].LexerString);
               delete (yyvsp[(3) - (4)].LexerString);
           }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 322 "./src/SynthLib2Parser.y"
    {
              (yyval.TheCmd) = new FunDefCmd(GetCurrentLocation(),
                                 *(yyvsp[(3) - (7)].LexerString), *(yyvsp[(4) - (7)].SymbolSortVector), (yyvsp[(5) - (7)].TheSortExpr), (yyvsp[(6) - (7)].TheTerm), NULL);

              delete (yyvsp[(3) - (7)].LexerString);
              delete (yyvsp[(4) - (7)].SymbolSortVector);
          }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 331 "./src/SynthLib2Parser.y"
    {
               (yyval.TheCmd) = new FunDeclCmd(GetCurrentLocation(),
                                   *(yyvsp[(3) - (8)].LexerString), *(yyvsp[(5) - (8)].SortVector), (yyvsp[(7) - (8)].TheSortExpr));
               delete (yyvsp[(3) - (8)].LexerString);
               delete (yyvsp[(5) - (8)].SortVector);
           }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 339 "./src/SynthLib2Parser.y"
    { (yyval.SortVector) = (yyvsp[(1) - (2)].SortVector); (yyval.SortVector)->push_back((yyvsp[(2) - (2)].TheSortExpr)); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 341 "./src/SynthLib2Parser.y"
    { (yyval.SortVector) = new vector<const SortExpr*>(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 344 "./src/SynthLib2Parser.y"
    {
            (yyval.SymbolSortVector) = (yyvsp[(2) - (3)].SymbolSortVector);
        }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 349 "./src/SynthLib2Parser.y"
    {
                       (yyval.SymbolSortVector) = (yyvsp[(1) - (2)].SymbolSortVector);
                       (yyval.SymbolSortVector)->push_back((yyvsp[(2) - (2)].SymbolSortPair));
                   }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 354 "./src/SynthLib2Parser.y"
    {
                       (yyval.SymbolSortVector) = new vector<ArgSortPair*>();
                   }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 360 "./src/SynthLib2Parser.y"
    {
                   (yyval.SymbolSortPair) = new ArgSortPair(GetCurrentLocation(), *(yyvsp[(2) - (4)].LexerString), (yyvsp[(3) - (4)].TheSortExpr));
                   delete (yyvsp[(2) - (4)].LexerString);
               }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 366 "./src/SynthLib2Parser.y"
    {
         (yyval.TheTerm) = new FunTerm(GetCurrentLocation(), *(yyvsp[(2) - (4)].LexerString), *(yyvsp[(3) - (4)].TermVector));
         delete (yyvsp[(2) - (4)].LexerString);
         delete (yyvsp[(3) - (4)].TermVector);
     }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 372 "./src/SynthLib2Parser.y"
    {
         (yyval.TheTerm) = new LiteralTerm(GetCurrentLocation(), (yyvsp[(1) - (1)].TheLiteral));
     }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 376 "./src/SynthLib2Parser.y"
    {
         (yyval.TheTerm) = new SymbolTerm(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString));
         delete (yyvsp[(1) - (1)].LexerString);
     }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 381 "./src/SynthLib2Parser.y"
    {
         (yyval.TheTerm) = (yyvsp[(1) - (1)].TheTerm);
     }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 386 "./src/SynthLib2Parser.y"
    {
            (yyval.TheTerm) = new LetTerm(GetCurrentLocation(), *(yyvsp[(4) - (7)].STPairVector), (yyvsp[(6) - (7)].TheTerm), new SymbolTableScope());
            delete (yyvsp[(4) - (7)].STPairVector);
        }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 392 "./src/SynthLib2Parser.y"
    {
                       (yyval.STPairVector) = (yyvsp[(1) - (2)].STPairVector);
                       (yyval.STPairVector)->push_back((yyvsp[(2) - (2)].STPair));
                   }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 397 "./src/SynthLib2Parser.y"
    {
                       (yyval.STPairVector) = new vector<LetBindingTerm*>();
                       (yyval.STPairVector)->push_back((yyvsp[(1) - (1)].STPair));
                   }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 403 "./src/SynthLib2Parser.y"
    {
                   (yyval.STPair) = new LetBindingTerm(GetCurrentLocation(),
                                           *(yyvsp[(2) - (5)].LexerString), (yyvsp[(3) - (5)].TheSortExpr), (yyvsp[(4) - (5)].TheTerm));
                   delete (yyvsp[(2) - (5)].LexerString);
               }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 410 "./src/SynthLib2Parser.y"
    {
             (yyval.TermVector) = (yyvsp[(1) - (2)].TermVector);
             (yyval.TermVector)->push_back((yyvsp[(2) - (2)].TheTerm));
         }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 415 "./src/SynthLib2Parser.y"
    {
             (yyval.TermVector) = new vector<Term*>();
         }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 420 "./src/SynthLib2Parser.y"
    {
            (yyval.TheLiteral) = new Literal(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString), new IntSortExpr(SourceLocation::None));
            delete (yyvsp[(1) - (1)].LexerString);
        }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 425 "./src/SynthLib2Parser.y"
    {
            (yyval.TheLiteral) = new Literal(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString), new BoolSortExpr(SourceLocation::None));
            delete (yyvsp[(1) - (1)].LexerString);
        }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 430 "./src/SynthLib2Parser.y"
    {
            if ((*(yyvsp[(1) - (1)].LexerString))[1] == 'x') {
                (yyval.TheLiteral) = new Literal(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString), new BVSortExpr(SourceLocation::None, ((yyvsp[(1) - (1)].LexerString)->length() - 2) * 4));
            } else {
                (yyval.TheLiteral) = new Literal(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString), new BVSortExpr(SourceLocation::None, ((yyvsp[(1) - (1)].LexerString)->length() - 2)));
            }
            delete (yyvsp[(1) - (1)].LexerString);
        }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 439 "./src/SynthLib2Parser.y"
    {
            // Lookup the type of the enumeration
            auto ConstString = *(yyvsp[(1) - (1)].LexerString);
            vector<string> Tokens;
            boost::algorithm::split(Tokens, ConstString, boost::algorithm::is_any_of(": "),
                                    boost::algorithm::token_compress_on);
            auto SortString = Tokens[0];
            SortExpr* Sort = new NamedSortExpr(SourceLocation::None, SortString);
            (yyval.TheLiteral) = new Literal(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString), Sort);
            delete (yyvsp[(1) - (1)].LexerString);
        }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 451 "./src/SynthLib2Parser.y"
    {
            (yyval.TheLiteral) = new Literal(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString), new RealSortExpr(SourceLocation::None));
            delete (yyvsp[(1) - (1)].LexerString);
        }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 456 "./src/SynthLib2Parser.y"
    {
            (yyval.TheLiteral) = new Literal(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString), new StringSortExpr(SourceLocation::None));
            delete (yyvsp[(1) - (1)].LexerString);
        }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 462 "./src/SynthLib2Parser.y"
    {
              (yyval.NTDefVector) = (yyvsp[(1) - (2)].NTDefVector);
              (yyval.NTDefVector)->push_back((yyvsp[(2) - (2)].TheNTDef));
          }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 467 "./src/SynthLib2Parser.y"
    {
              (yyval.NTDefVector) = new vector<NTDef*>();
              (yyval.NTDefVector)->push_back((yyvsp[(1) - (1)].TheNTDef));
          }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 473 "./src/SynthLib2Parser.y"
    {
          (yyval.TheNTDef) = new NTDef(GetCurrentLocation(),
                         *(yyvsp[(2) - (7)].LexerString), (yyvsp[(3) - (7)].TheSortExpr), *(yyvsp[(5) - (7)].GTermVector));
          delete (yyvsp[(2) - (7)].LexerString);
          delete (yyvsp[(5) - (7)].GTermVector);
      }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 481 "./src/SynthLib2Parser.y"
    {
              (yyval.GTermVector) = (yyvsp[(1) - (2)].GTermVector);
              (yyval.GTermVector)->push_back((yyvsp[(2) - (2)].TheGTerm));
          }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 486 "./src/SynthLib2Parser.y"
    {
              (yyval.GTermVector) = new vector<GTerm*>();
              (yyval.GTermVector)->push_back((yyvsp[(1) - (1)].TheGTerm));
          }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 492 "./src/SynthLib2Parser.y"
    {
                  (yyval.TheCmd) = new CheckSynthCmd(GetCurrentLocation());
              }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 497 "./src/SynthLib2Parser.y"
    {
                  (yyval.TheCmd) = new ConstraintCmd(GetCurrentLocation(), (yyvsp[(3) - (4)].TheTerm));
              }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 502 "./src/SynthLib2Parser.y"
    {
                  (yyval.TheCmd) = new InvConstraintCmd(GetCurrentLocation(), *(yyvsp[(3) - (7)].LexerString), *(yyvsp[(4) - (7)].LexerString), *(yyvsp[(5) - (7)].LexerString), *(yyvsp[(6) - (7)].LexerString));

                  delete (yyvsp[(3) - (7)].LexerString);
                  delete (yyvsp[(4) - (7)].LexerString);
                  delete (yyvsp[(5) - (7)].LexerString);
                  delete (yyvsp[(6) - (7)].LexerString);
              }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 513 "./src/SynthLib2Parser.y"
    {
                (yyval.TheCmd) = new SynthFunCmd(GetCurrentLocation(), *(yyvsp[(3) - (9)].LexerString),
                                     *(yyvsp[(4) - (9)].SymbolSortVector), (yyvsp[(5) - (9)].TheSortExpr), *(yyvsp[(7) - (9)].NTDefVector), new SymbolTableScope());


                delete (yyvsp[(3) - (9)].LexerString);
                delete (yyvsp[(4) - (9)].SymbolSortVector);
                delete (yyvsp[(7) - (9)].NTDefVector);
            }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 524 "./src/SynthLib2Parser.y"
    {
                (yyval.TheCmd) = new SynthFunCmd(GetCurrentLocation(), *(yyvsp[(3) - (6)].LexerString),
                                     *(yyvsp[(4) - (6)].SymbolSortVector), (yyvsp[(5) - (6)].TheSortExpr), new SymbolTableScope());
                delete (yyvsp[(3) - (6)].LexerString);
                delete (yyvsp[(4) - (6)].SymbolSortVector);
            }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 533 "./src/SynthLib2Parser.y"
    {
                (yyval.TheCmd) = new SynthInvCmd(GetCurrentLocation(), 
                                     *(yyvsp[(3) - (8)].LexerString), *(yyvsp[(4) - (8)].SymbolSortVector), *(yyvsp[(6) - (8)].NTDefVector), new SymbolTableScope());
                delete (yyvsp[(3) - (8)].LexerString);
                delete (yyvsp[(6) - (8)].NTDefVector);
            }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 541 "./src/SynthLib2Parser.y"
    {
                (yyval.TheCmd) = new SynthInvCmd(GetCurrentLocation(), 
                                     *(yyvsp[(3) - (5)].LexerString), *(yyvsp[(4) - (5)].SymbolSortVector), new SymbolTableScope());
                delete (yyvsp[(3) - (5)].LexerString);
            }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 549 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = new SymbolGTerm(GetCurrentLocation(), *(yyvsp[(1) - (1)].LexerString));
          delete (yyvsp[(1) - (1)].LexerString);
      }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 554 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = new LiteralGTerm(GetCurrentLocation(), (yyvsp[(1) - (1)].TheLiteral));
      }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 558 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = new FunGTerm(GetCurrentLocation(), *(yyvsp[(2) - (4)].LexerString), *(yyvsp[(3) - (4)].GTermVector));
          delete (yyvsp[(2) - (4)].LexerString);
          delete (yyvsp[(3) - (4)].GTermVector);
      }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 564 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = new ConstantGTerm(GetCurrentLocation(), (yyvsp[(3) - (4)].TheSortExpr));
      }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 568 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = new VariableGTerm(GetCurrentLocation(), (yyvsp[(3) - (4)].TheSortExpr), VARKIND_ANY);
      }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 572 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = new VariableGTerm(GetCurrentLocation(), (yyvsp[(3) - (4)].TheSortExpr), VARKIND_INPUT);
      }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 576 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = new VariableGTerm(GetCurrentLocation(), (yyvsp[(3) - (4)].TheSortExpr), VARKIND_LOCAL);
      }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 580 "./src/SynthLib2Parser.y"
    {
          (yyval.TheGTerm) = (yyvsp[(1) - (1)].TheGTerm);
      }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 585 "./src/SynthLib2Parser.y"
    {
             (yyval.TheGTerm) = new LetGTerm(GetCurrentLocation(), *(yyvsp[(4) - (7)].SGPairVector), (yyvsp[(6) - (7)].TheGTerm), new SymbolTableScope());
             delete (yyvsp[(4) - (7)].SGPairVector);
         }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 591 "./src/SynthLib2Parser.y"
    {
                        (yyval.SGPairVector) = (yyvsp[(1) - (2)].SGPairVector);
                        (yyval.SGPairVector)->push_back((yyvsp[(2) - (2)].SGPair));
                    }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 596 "./src/SynthLib2Parser.y"
    {
                        (yyval.SGPairVector) = new vector<LetBindingGTerm*>();
                        (yyval.SGPairVector)->push_back((yyvsp[(1) - (1)].SGPair));
                    }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 602 "./src/SynthLib2Parser.y"
    {
                    (yyval.SGPair) = new LetBindingGTerm(GetCurrentLocation(),
                                             *(yyvsp[(2) - (5)].LexerString), (yyvsp[(3) - (5)].TheSortExpr), (yyvsp[(4) - (5)].TheGTerm));
                    delete (yyvsp[(2) - (5)].LexerString);
                }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 609 "./src/SynthLib2Parser.y"
    {
              (yyval.GTermVector) = (yyvsp[(1) - (2)].GTermVector);
              (yyval.GTermVector)->push_back((yyvsp[(2) - (2)].TheGTerm));
          }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 614 "./src/SynthLib2Parser.y"
    {
              (yyval.GTermVector) = new vector<GTerm*>();
          }
    break;


/* Line 1792 of yacc.c  */
#line 2394 "src/SynthLib2Parser.tab.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


