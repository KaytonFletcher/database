/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "dependencies/Parser.y"


	#include "ParseTree.h" 
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <iostream>

	extern "C" int yylex();
	extern "C" int yyparse();
	extern "C" void yyerror(char *s);
  
	// these data structures hold the result of the parsing
	struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
	struct TableList *tables; // the list of tables and aliases in the query
	struct AndList *whereList; // the predicate in the WHERE clause
	struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
	struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
	int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
	int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query


#line 94 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    Name = 258,                    /* Name  */
    Float = 259,                   /* Float  */
    Int = 260,                     /* Int  */
    String = 261,                  /* String  */
    SELECT = 262,                  /* SELECT  */
    GROUP = 263,                   /* GROUP  */
    DISTINCT = 264,                /* DISTINCT  */
    BY = 265,                      /* BY  */
    FROM = 266,                    /* FROM  */
    WHERE = 267,                   /* WHERE  */
    SUM = 268,                     /* SUM  */
    AS = 269,                      /* AS  */
    AND = 270,                     /* AND  */
    OR = 271                       /* OR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define Name 258
#define Float 259
#define Int 260
#define String 261
#define SELECT 262
#define GROUP 263
#define DISTINCT 264
#define BY 265
#define FROM 266
#define WHERE 267
#define SUM 268
#define AS 269
#define AND 270
#define OR 271

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 26 "dependencies/Parser.y"

 	struct FuncOperand *myOperand;
	struct FuncOperator *myOperator; 
	struct TableList *myTables;
	struct ComparisonOp *myComparison;
	struct Operand *myBoolOperand;
	struct OrList *myOrList;
	struct AndList *myAndList;
	struct NameList *myNames;
	char *actualChars;
	char whichOne;

#line 192 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_Name = 3,                       /* Name  */
  YYSYMBOL_Float = 4,                      /* Float  */
  YYSYMBOL_Int = 5,                        /* Int  */
  YYSYMBOL_String = 6,                     /* String  */
  YYSYMBOL_SELECT = 7,                     /* SELECT  */
  YYSYMBOL_GROUP = 8,                      /* GROUP  */
  YYSYMBOL_DISTINCT = 9,                   /* DISTINCT  */
  YYSYMBOL_BY = 10,                        /* BY  */
  YYSYMBOL_FROM = 11,                      /* FROM  */
  YYSYMBOL_WHERE = 12,                     /* WHERE  */
  YYSYMBOL_SUM = 13,                       /* SUM  */
  YYSYMBOL_AS = 14,                        /* AS  */
  YYSYMBOL_AND = 15,                       /* AND  */
  YYSYMBOL_OR = 16,                        /* OR  */
  YYSYMBOL_17_ = 17,                       /* ','  */
  YYSYMBOL_18_ = 18,                       /* '('  */
  YYSYMBOL_19_ = 19,                       /* ')'  */
  YYSYMBOL_20_ = 20,                       /* '-'  */
  YYSYMBOL_21_ = 21,                       /* '+'  */
  YYSYMBOL_22_ = 22,                       /* '*'  */
  YYSYMBOL_23_ = 23,                       /* '/'  */
  YYSYMBOL_24_ = 24,                       /* '<'  */
  YYSYMBOL_25_ = 25,                       /* '>'  */
  YYSYMBOL_26_ = 26,                       /* '='  */
  YYSYMBOL_YYACCEPT = 27,                  /* $accept  */
  YYSYMBOL_SQL = 28,                       /* SQL  */
  YYSYMBOL_WhatIWant = 29,                 /* WhatIWant  */
  YYSYMBOL_Function = 30,                  /* Function  */
  YYSYMBOL_Atts = 31,                      /* Atts  */
  YYSYMBOL_Tables = 32,                    /* Tables  */
  YYSYMBOL_CompoundExp = 33,               /* CompoundExp  */
  YYSYMBOL_Op = 34,                        /* Op  */
  YYSYMBOL_AndList = 35,                   /* AndList  */
  YYSYMBOL_OrList = 36,                    /* OrList  */
  YYSYMBOL_Condition = 37,                 /* Condition  */
  YYSYMBOL_BoolComp = 38,                  /* BoolComp  */
  YYSYMBOL_Literal = 39,                   /* Literal  */
  YYSYMBOL_SimpleExp = 40                  /* SimpleExp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   60

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  14
/* YYNRULES -- Number of rules.  */
#define YYNRULES  37
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  71

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   271


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      18,    19,    22,    21,    17,    20,     2,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      24,    26,    25,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,    84,    91,    97,   102,   109,   117,   123,
     129,   136,   143,   151,   161,   174,   184,   190,   199,   210,
     215,   220,   225,   231,   245,   254,   262,   271,   280,   287,
     294,   302,   310,   318,   326,   338,   346,   354
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "Name", "Float", "Int",
  "String", "SELECT", "GROUP", "DISTINCT", "BY", "FROM", "WHERE", "SUM",
  "AS", "AND", "OR", "','", "'('", "')'", "'-'", "'+'", "'*'", "'/'",
  "'<'", "'>'", "'='", "$accept", "SQL", "WhatIWant", "Function", "Atts",
  "Tables", "CompoundExp", "Op", "AndList", "OrList", "Condition",
  "BoolComp", "Literal", "SimpleExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,    44,    40,    41,
      45,    43,    42,    47,    60,    62,    61
};
#endif

#define YYPACT_NINF (-16)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      24,     5,     9,   -16,    27,    -2,    22,    17,    18,   -16,
      18,    19,    -1,    33,    27,    35,    -1,   -16,   -16,   -16,
      -1,    -1,    20,     2,    26,     3,    18,   -16,    25,    28,
     -16,   -16,   -16,   -16,   -16,   -16,    -1,    38,    30,    39,
     -16,     2,   -16,   -16,    23,    37,    29,    -1,   -16,   -16,
     -16,   -16,    31,    36,   -13,    41,    43,   -16,    34,    23,
     -16,   -16,   -16,    23,    27,   -16,    30,   -16,   -16,    18,
     -16
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,    10,     0,     0,     0,     5,     6,     1,
       7,     0,     0,     0,     0,     0,     0,    37,    35,    36,
       0,     0,     0,    17,     0,     0,     4,    11,     0,     0,
      18,     8,    19,    20,    21,    22,     0,     0,     0,     0,
       9,    16,    14,    12,     0,     2,     0,     0,    34,    32,
      33,    31,     0,    26,     0,     0,     0,    15,    24,     0,
      28,    29,    30,     0,     0,    13,     0,    25,    27,     3,
      23
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -16,   -16,   -16,   -16,    -4,   -16,   -15,    12,   -12,    -3,
     -16,   -16,    -8,   -16
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     6,     7,     8,    25,    22,    36,    45,    52,
      53,    63,    54,    23
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      10,    28,    17,    18,    19,    29,    30,    11,     3,     9,
      26,    60,    61,    62,     4,    38,    12,    20,     5,    21,
      39,    42,    32,    33,    34,    35,    48,    49,    50,    51,
       3,     1,    57,    13,    14,    15,    24,    16,    27,    31,
      37,    43,    46,    56,    40,    55,    65,    41,    44,    66,
      58,    64,    59,    47,    70,    68,    67,     0,     0,     0,
      69
};

static const yytype_int8 yycheck[] =
{
       4,    16,     3,     4,     5,    20,    21,     9,     3,     0,
      14,    24,    25,    26,     9,    12,    18,    18,    13,    20,
      17,    36,    20,    21,    22,    23,     3,     4,     5,     6,
       3,     7,    47,    11,    17,    17,     3,    18,     3,    19,
      14,     3,     3,    14,    19,     8,     3,    19,    18,    15,
      19,    10,    16,    41,    66,    63,    59,    -1,    -1,    -1,
      64
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    28,     3,     9,    13,    29,    30,    31,     0,
      31,     9,    18,    11,    17,    17,    18,     3,     4,     5,
      18,    20,    33,    40,     3,    32,    31,     3,    33,    33,
      33,    19,    20,    21,    22,    23,    34,    14,    12,    17,
      19,    19,    33,     3,    18,    35,     3,    34,     3,     4,
       5,     6,    36,    37,    39,     8,    14,    33,    19,    16,
      24,    25,    26,    38,    10,     3,    15,    36,    39,    31,
      35
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    27,    28,    28,    29,    29,    29,    29,    30,    30,
      31,    31,    32,    32,    33,    33,    33,    33,    33,    34,
      34,    34,    34,    35,    35,    36,    36,    37,    38,    38,
      38,    39,    39,    39,    39,    40,    40,    40
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     9,     3,     1,     1,     2,     4,     5,
       1,     3,     3,     5,     3,     5,     3,     1,     2,     1,
       1,     1,     1,     5,     3,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* SQL: SELECT WhatIWant FROM Tables WHERE AndList  */
#line 78 "dependencies/Parser.y"
{
	tables = (yyvsp[-2].myTables);
	whereList = (yyvsp[0].myAndList);	
	groupingAtts = NULL;
}
#line 1258 "y.tab.c"
    break;

  case 3: /* SQL: SELECT WhatIWant FROM Tables WHERE AndList GROUP BY Atts  */
#line 85 "dependencies/Parser.y"
{
	tables = (yyvsp[-5].myTables);
	whereList = (yyvsp[-3].myAndList);	
	groupingAtts = (yyvsp[0].myNames);
}
#line 1268 "y.tab.c"
    break;

  case 4: /* WhatIWant: Function ',' Atts  */
#line 92 "dependencies/Parser.y"
{
	attsToSelect = (yyvsp[0].myNames);
	distinctAtts = 0;
}
#line 1277 "y.tab.c"
    break;

  case 5: /* WhatIWant: Function  */
#line 98 "dependencies/Parser.y"
{
	attsToSelect = NULL;
}
#line 1285 "y.tab.c"
    break;

  case 6: /* WhatIWant: Atts  */
#line 103 "dependencies/Parser.y"
{
	distinctAtts = 0;
	finalFunction = NULL;
	attsToSelect = (yyvsp[0].myNames);
}
#line 1295 "y.tab.c"
    break;

  case 7: /* WhatIWant: DISTINCT Atts  */
#line 110 "dependencies/Parser.y"
{
	distinctAtts = 1;
	finalFunction = NULL;
	attsToSelect = (yyvsp[0].myNames);
	finalFunction = NULL;
}
#line 1306 "y.tab.c"
    break;

  case 8: /* Function: SUM '(' CompoundExp ')'  */
#line 118 "dependencies/Parser.y"
{
	distinctFunc = 0;
	finalFunction = (yyvsp[-1].myOperator);
}
#line 1315 "y.tab.c"
    break;

  case 9: /* Function: SUM DISTINCT '(' CompoundExp ')'  */
#line 124 "dependencies/Parser.y"
{
	distinctFunc = 1;
	finalFunction = (yyvsp[-1].myOperator);
}
#line 1324 "y.tab.c"
    break;

  case 10: /* Atts: Name  */
#line 130 "dependencies/Parser.y"
{
	(yyval.myNames) = (struct NameList *) malloc (sizeof (struct NameList));
	(yyval.myNames)->name = (yyvsp[0].actualChars);
	(yyval.myNames)->next = NULL;
}
#line 1334 "y.tab.c"
    break;

  case 11: /* Atts: Atts ',' Name  */
#line 137 "dependencies/Parser.y"
{
	(yyval.myNames) = (struct NameList *) malloc (sizeof (struct NameList));
	(yyval.myNames)->name = (yyvsp[0].actualChars);
	(yyval.myNames)->next = (yyvsp[-2].myNames);
}
#line 1344 "y.tab.c"
    break;

  case 12: /* Tables: Name AS Name  */
#line 144 "dependencies/Parser.y"
{
	(yyval.myTables) = (struct TableList *) malloc (sizeof (struct TableList));
	(yyval.myTables)->tableName = (yyvsp[-2].actualChars);
	(yyval.myTables)->aliasAs = (yyvsp[0].actualChars);
	(yyval.myTables)->next = NULL;
}
#line 1355 "y.tab.c"
    break;

  case 13: /* Tables: Tables ',' Name AS Name  */
#line 152 "dependencies/Parser.y"
{
	(yyval.myTables) = (struct TableList *) malloc (sizeof (struct TableList));
	(yyval.myTables)->tableName = (yyvsp[-2].actualChars);
	(yyval.myTables)->aliasAs = (yyvsp[0].actualChars);
	(yyval.myTables)->next = (yyvsp[-4].myTables);
}
#line 1366 "y.tab.c"
    break;

  case 14: /* CompoundExp: SimpleExp Op CompoundExp  */
#line 162 "dependencies/Parser.y"
{
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));
	(yyval.myOperator)->leftOperator->leftOperator = NULL;
	(yyval.myOperator)->leftOperator->leftOperand = (yyvsp[-2].myOperand);
	(yyval.myOperator)->leftOperator->right = NULL;
	(yyval.myOperator)->leftOperand = NULL;
	(yyval.myOperator)->right = (yyvsp[0].myOperator);
	(yyval.myOperator)->code = (yyvsp[-1].whichOne);	

}
#line 1382 "y.tab.c"
    break;

  case 15: /* CompoundExp: '(' CompoundExp ')' Op CompoundExp  */
#line 175 "dependencies/Parser.y"
{
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = (yyvsp[-3].myOperator);
	(yyval.myOperator)->leftOperand = NULL;
	(yyval.myOperator)->right = (yyvsp[0].myOperator);
	(yyval.myOperator)->code = (yyvsp[-1].whichOne);	

}
#line 1395 "y.tab.c"
    break;

  case 16: /* CompoundExp: '(' CompoundExp ')'  */
#line 185 "dependencies/Parser.y"
{
	(yyval.myOperator) = (yyvsp[-1].myOperator);

}
#line 1404 "y.tab.c"
    break;

  case 17: /* CompoundExp: SimpleExp  */
#line 191 "dependencies/Parser.y"
{
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = NULL;
	(yyval.myOperator)->leftOperand = (yyvsp[0].myOperand);
	(yyval.myOperator)->right = NULL;	

}
#line 1416 "y.tab.c"
    break;

  case 18: /* CompoundExp: '-' CompoundExp  */
#line 200 "dependencies/Parser.y"
{
	(yyval.myOperator) = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	(yyval.myOperator)->leftOperator = (yyvsp[0].myOperator);
	(yyval.myOperator)->leftOperand = NULL;
	(yyval.myOperator)->right = NULL;	
	(yyval.myOperator)->code = '-';

}
#line 1429 "y.tab.c"
    break;

  case 19: /* Op: '-'  */
#line 211 "dependencies/Parser.y"
{
	(yyval.whichOne) = '-';
}
#line 1437 "y.tab.c"
    break;

  case 20: /* Op: '+'  */
#line 216 "dependencies/Parser.y"
{
	(yyval.whichOne) = '+';
}
#line 1445 "y.tab.c"
    break;

  case 21: /* Op: '*'  */
#line 221 "dependencies/Parser.y"
{
	(yyval.whichOne) = '*';
}
#line 1453 "y.tab.c"
    break;

  case 22: /* Op: '/'  */
#line 226 "dependencies/Parser.y"
{
	(yyval.whichOne) = '/';
}
#line 1461 "y.tab.c"
    break;

  case 23: /* AndList: '(' OrList ')' AND AndList  */
#line 232 "dependencies/Parser.y"
{
        // here we need to pre-pend the OrList to the AndList
        // first we allocate space for this node
        (yyval.myAndList) = (struct AndList *) malloc (sizeof (struct AndList));

        // hang the OrList off of the left
        (yyval.myAndList)->left = (yyvsp[-3].myOrList);

        // hang the AndList off of the right
        (yyval.myAndList)->rightAnd = (yyvsp[0].myAndList);

}
#line 1478 "y.tab.c"
    break;

  case 24: /* AndList: '(' OrList ')'  */
#line 246 "dependencies/Parser.y"
{
        // just return the OrList!
        (yyval.myAndList) = (struct AndList *) malloc (sizeof (struct AndList));
        (yyval.myAndList)->left = (yyvsp[-1].myOrList);
        (yyval.myAndList)->rightAnd = NULL;
}
#line 1489 "y.tab.c"
    break;

  case 25: /* OrList: Condition OR OrList  */
#line 255 "dependencies/Parser.y"
{
        // here we have to hang the condition off the left of the OrList
        (yyval.myOrList) = (struct OrList *) malloc (sizeof (struct OrList));
        (yyval.myOrList)->left = (yyvsp[-2].myComparison);
        (yyval.myOrList)->rightOr = (yyvsp[0].myOrList);
}
#line 1500 "y.tab.c"
    break;

  case 26: /* OrList: Condition  */
#line 263 "dependencies/Parser.y"
{
        // nothing to hang off of the right
        (yyval.myOrList) = (struct OrList *) malloc (sizeof (struct OrList));
        (yyval.myOrList)->left = (yyvsp[0].myComparison);
        (yyval.myOrList)->rightOr = NULL;
}
#line 1511 "y.tab.c"
    break;

  case 27: /* Condition: Literal BoolComp Literal  */
#line 272 "dependencies/Parser.y"
{
        // in this case we have a simple literal/variable comparison
        (yyval.myComparison) = (yyvsp[-1].myComparison);
        (yyval.myComparison)->left = (yyvsp[-2].myBoolOperand);
        (yyval.myComparison)->right = (yyvsp[0].myBoolOperand);
}
#line 1522 "y.tab.c"
    break;

  case 28: /* BoolComp: '<'  */
#line 281 "dependencies/Parser.y"
{
        // construct and send up the comparison
        (yyval.myComparison) = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        (yyval.myComparison)->code = LESS_THAN;
}
#line 1532 "y.tab.c"
    break;

  case 29: /* BoolComp: '>'  */
#line 288 "dependencies/Parser.y"
{
        // construct and send up the comparison
        (yyval.myComparison) = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        (yyval.myComparison)->code = GREATER_THAN;
}
#line 1542 "y.tab.c"
    break;

  case 30: /* BoolComp: '='  */
#line 295 "dependencies/Parser.y"
{
        // construct and send up the comparison
        (yyval.myComparison) = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        (yyval.myComparison)->code = EQUALS;
}
#line 1552 "y.tab.c"
    break;

  case 31: /* Literal: String  */
#line 303 "dependencies/Parser.y"
{
        // construct and send up the operand containing the string
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = STRING;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 1563 "y.tab.c"
    break;

  case 32: /* Literal: Float  */
#line 311 "dependencies/Parser.y"
{
        // construct and send up the operand containing the FP number
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = DOUBLE;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 1574 "y.tab.c"
    break;

  case 33: /* Literal: Int  */
#line 319 "dependencies/Parser.y"
{
        // construct and send up the operand containing the integer
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = INT;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 1585 "y.tab.c"
    break;

  case 34: /* Literal: Name  */
#line 327 "dependencies/Parser.y"
{
        // construct and send up the operand containing the name
        (yyval.myBoolOperand) = (struct Operand *) malloc (sizeof (struct Operand));
        (yyval.myBoolOperand)->code = NAME;
        (yyval.myBoolOperand)->value = (yyvsp[0].actualChars);
}
#line 1596 "y.tab.c"
    break;

  case 35: /* SimpleExp: Float  */
#line 339 "dependencies/Parser.y"
{
        // construct and send up the operand containing the FP number
        (yyval.myOperand) = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        (yyval.myOperand)->code = DOUBLE;
        (yyval.myOperand)->value = (yyvsp[0].actualChars);
}
#line 1607 "y.tab.c"
    break;

  case 36: /* SimpleExp: Int  */
#line 347 "dependencies/Parser.y"
{
        // construct and send up the operand containing the integer
        (yyval.myOperand) = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        (yyval.myOperand)->code = INT;
        (yyval.myOperand)->value = (yyvsp[0].actualChars);
}
#line 1618 "y.tab.c"
    break;

  case 37: /* SimpleExp: Name  */
#line 355 "dependencies/Parser.y"
{
        // construct and send up the operand containing the name
        (yyval.myOperand) = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        (yyval.myOperand)->code = NAME;
        (yyval.myOperand)->value = (yyvsp[0].actualChars);
}
#line 1629 "y.tab.c"
    break;


#line 1633 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 363 "dependencies/Parser.y"


