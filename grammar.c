#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TOTAL_TERMINALS 35
#define TOTAL_NON_TERMINALS 37
#define NON_TERMINALS 37
// 1->2->3
// |
// |
// a->b->c

// DECLARATION -> 1-> 2 -> NULL
//                |  |
//                |  declaration_stmt
//                declaration_stmt -> declaration -> NULL

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

void substring(char *a, char *b, int str, int end)
{
    int c = 0;
    for (int i = str; i <= end; i++)
    {
        a[c++] = b[i];
    }
    a[c] = '\0';
}

int find(char *str, int isterm)
{

    //Terminal
    if (isterm == 1)
    {
        for (int i = 0; i < TOTAL_TERMINALS; i++)
        {
            if (strcmp(str, TerminalMap[i]) == 0)
                return i;
        }
    }

    //Non terminal
    else
    {
        for (int i = 0; i < TOTAL_NON_TERMINALS; i++)
        {
            if (strcmp(str, NonTerminalMap[i]) == 0)
                return i;
        }
    }
    printf("Error %s\n", str);
    return 0;
}

typedef union SYMBOL
{
    Terminal t;
    NonTerminal nt;
} symbol;

// typedef enum { nonterminal,
//                terminal } Isterm;

typedef struct RULE // Single Node of a rule in RHS
{
    int isterm; // 0=Non-Term  // 1=Terminal
    symbol type;
    struct RULE *next;
} rule;

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

void readGrammar(char a[100], grammar *G)
{
    FILE *fp;
    int count = 0;
    int count2 = 0;
    char buff[1000];
    char tmp[100];
    char tmp2[100];
    fp = fopen(a, "r+");
    if (fp == NULL)
    {
        printf("ERRROR\n");
        return;
    }
    char *grammar_split = NULL;
    grammar *gramcurr = G;
    grules *grulescurr = NULL;
    rule *rulecurr = NULL;
    while (fscanf(fp, "%[^\n]\n", buff) != EOF)
    {
        //printf("%s\n", buff);
        count++;
        grammar_split = strtok(buff, " ");
        if (grammar_split == NULL)
        {
            //printf("NULL\n");
        }
        if (grammar_split != NULL)
        {
            //printf("%s\n", grammar_split);
            substring(tmp, grammar_split, 1, strlen(grammar_split) - 2);
            //printf("%s %s\n", grammar_split, tmp);
        }
        if (count == 1)
        {
            //Initialsing grammar
            //printf("%s %s\n", grammar_split, tmp);
            gramcurr->nt = find(tmp, 0); //Initialised nt
            grulescurr = malloc(sizeof(grules));
            gramcurr->head = grulescurr; // Initialised head

            //Initialising grules
            rulecurr = malloc(sizeof(rule));
            grulescurr->head = rulecurr;
            grulescurr->next = NULL;

            //Initialising rule
            rulecurr->next = NULL;
        }

        else if (gramcurr->nt == find(tmp, 0))
        {
            //Need to make a new grules for the same LHS
            grulescurr->next = malloc(sizeof(grules));
            grulescurr = grulescurr->next;
            //Initialsing a new node of grules
            grulescurr->next = NULL;
            //ruleprev = NULL;
            rulecurr = malloc(sizeof(rule));
            rulecurr->next = NULL;
            grulescurr->head = rulecurr;
        }
        else
        {
            //Need to go add a new fresh rule for a fresh and juicy new LHS
            gramcurr++;
            //Initialsing grammar
            gramcurr->nt = find(tmp, 0); //Initialised nt
            //printf("%s <--->\n", NonTerminalMap[gramcurr->nt]);
            grulescurr = malloc(sizeof(grules));
            gramcurr->head = grulescurr; // Initialised head

            //Initialising grules
            rulecurr = malloc(sizeof(rule));
            grulescurr->head = rulecurr;
            grulescurr->next = NULL;

            //Initialising rule
            rulecurr->next = NULL;
            //ruleprev = NULL;
        }

        while (grammar_split != NULL)
        {
            grammar_split = strtok(NULL, " ");
            if (grammar_split != NULL)
            {
                if (strcmp(grammar_split, "=>") == 0)
                {
                    //printf("=>\n");
                    continue;
                }
                count2++;
                if (count2 != 1)
                {
                    rulecurr->next = malloc(sizeof(rule));
                    rulecurr = rulecurr->next;
                    rulecurr->next = NULL;
                }
                if (grammar_split[0] == '<')
                {
                    //printf("NONTERMINAL\n");
                    substring(tmp2, grammar_split, 1, strlen(grammar_split) - 2);
                    rulecurr->isterm = 0;
                    rulecurr->type.nt = find(tmp2, 0);
                }
                else
                {
                    //printf("TERMINAL\n");
                    rulecurr->isterm = 1;
                    rulecurr->type.t = find(grammar_split, 1);
                }
            }
        }
        count2 = 0;
    }
    fclose(fp);
    return;
}

void main()
{
    grammar *g = malloc(NON_TERMINALS * sizeof(grammar));
    readGrammar("gr.txt", g);
    grules *gcurr = NULL;
    for (int i = 0; i < 31; i++)
    {
        gcurr = g[i].head;
        if (g[i].head == NULL)
        {
            printf("ERRRRRRRRR\n");
            continue;
        }
        printf("%s --> ", NonTerminalMap[g[i].nt]);
        while (gcurr != NULL)
        {
            rule *r = gcurr->head;
            while (r != NULL)
            {
                if (r->isterm == 1)
                {
                    printf("%s ", TerminalMap[r->type.t]);
                }
                else
                {
                    printf("%s ", NonTerminalMap[r->type.nt]);
                }
                r = r->next;
            }
            gcurr = gcurr->next;
            printf(" | ");
        }
        printf("\n");
    }

    return;
}