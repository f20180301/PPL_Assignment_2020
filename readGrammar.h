/**
SHUBHAM AGARWAL -- 2018A7PS0310P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGARWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#ifndef _READGRAMMAR_
#define _READGRAMMAR_
#define TERMINALS 32
#define NON_TERMINALS 41

char *TerminalMap[] =
    {
        "PROGRAM",
        "BROP",
        "BRCL",
        "CURLYOP",
        "CURLYCL",
        "EQUALS",
        "DECLARE",
        "ID",
        "COLON",
        "LIST",
        "OF",
        "VARIABLES",
        "SEMICOLON",
        "ARRAY",
        "INTEGER",
        "SQOP",
        "DD",
        "SQCL",
        "JAGGED",
        "R1",
        "NUM",
        "REAL",
        "BOOLEAN",
        "PLUS",
        "MINUS",
        "DIVIDE",
        "MUL",
        "SIZE",
        "VALUES",
        "AND",
        "OR",
        "EPSILON"};
char *NonTerminalMap[] =
    {
        "s",
        "start",
        "declaration",
        "declaration_stmt",
        "assignment",
        "assignment_stmt",
        "primitive",
        "single_primitive",
        "multi_primitive",
        "array",
        "single_array",
        "multi_array",
        "array_dim",
        "jagged_array",
        "jagged_2d_array",
        "single_jagged_2d_array",
        "multi_jagged_2d_array",
        "jagged_3d_array",
        "single_jagged_3d_array",
        "multi_jagged_3d_array",
        "jagged_2d_dim",
        "jagged_3d_dim",
        "jag_lines",
        "jag_line",
        "jag_list",
        "num_list",
        "num_id",
        "var_list",
        "arr_pid",
        "idx",
        "type",
        "expression",
        "arithmetic_expression",
        "term",
        "factor",
        "op1",
        "op2",
        "boolean_expression",
        "or_expression",
        "fact_bool",
        "not_app"};

typedef enum
{
    PROGRAM,
    BROP,
    BRCL,
    CURLYOP,
    CURLYCL,
    EQUALS,
    DECLARE,
    ID,
    COLON,
    LIST,
    OF,
    VARIABLES,
    SEMICOLON,
    ARRAY,
    INTEGER,
    SQOP,
    DD,
    SQCL,
    JAGGED,
    R1,
    NUM,
    REAL,
    BOOLEAN,
    PLUS,
    MINUS,
    DIVIDE,
    MUL,
    SIZE,
    VALUES,
    AND,
    OR,
    EPSILON
} Terminal;

typedef enum
{
    s,
    start,
    declaration,
    declaration_stmt,
    assignment,
    assignment_stmt,
    primitive,
    single_primitive,
    multi_primitive,
    array,
    single_array,
    multi_array,
    array_dim,
    jagged_array,
    jagged_2d_array,
    single_jagged_2d_array,
    multi_jagged_2d_array,
    jagged_3d_array,
    single_jagged_3d_array,
    multi_jagged_3d_array,
    jagged_2d_dim,
    jagged_3d_dim,
    jag_lines,
    jag_line,
    jag_list,
    num_list,
    num_id,
    var_list,
    arr_pid,
    idx,
    type,
    expression,
    arithmetic_expression,
    term,
    factor,
    op1,
    op2,
    boolean_expression,
    or_expression,
    fact_bool,
    not_app
} NonTerminal;

typedef union SYMBOL
{
    Terminal t;
    NonTerminal nt;
} symbol;

typedef struct RULE // Single Node of a rule in RHS
{
    int isterm; // 0=Non-Term  // 1=Terminal
    symbol type;
    struct RULE *next;
} rule;

// wrapper for a complete rule
typedef struct GRULES // Linked list of mutiple rules corresponding to a single LHS
{
    rule *head;
    struct GRULES *next;
} grules;

typedef struct // Derivation of a particular single LHS
{
    NonTerminal nt;
    //char nonTerminal[100];
    grules *head;
} grammar;

//**************FUNCTION DECLARATIONS*******************//
void substring(char *, char *, int, int);
int find(char *, int);
void readGrammar(char *a, grammar *);

#endif