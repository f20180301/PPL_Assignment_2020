#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************TOKEN STRUCT IMPORTS******************************/
#define MAX 100

char *ha[13][2];
char *op[16][2]; //FOR KEYWORD

typedef struct token
{ //TOKEN STRUCTURE
    char lexeme[200];
    char token[200];
    int lineno;
    struct token *next;
} token;

token *head = NULL;
token *tail = NULL; //Head, Tail of LinkedList for TokenStream

/**************************END TOKEN STRUCT***************************************/

/**************************TOKEN FUNC IMPORTS*****************************************************/
char *getToken(char *lexeme)
{
    char c = lexeme[0];
    int flag = 0;

    if ((c >= 48 && c <= 57) || strcmp(lexeme, "true") == 0 || strcmp(lexeme, "false") == 0)
        return "NUM";
    else if (flag == 0)
    {
        for (int i = 0; i < 13; i++)
            if (strcmp(lexeme, ha[i][0]) == 0)
                return ha[i][1];

        flag = 1;
    }
    if (flag == 1)
    {
        for (int i = 0; i < 16; i++)
        {
            if (strcmp(lexeme, op[i][0]) == 0)
                return op[i][1];
        }
        flag = 2;
    }
    if (flag == 2)
    {
        char d = lexeme[0];
        int i = 1;
        if (d == '_' || (d >= 65 && d <= 90) || (d >= 97 && d <= 122))
        {
            while (lexeme[i] != '\0')
            {
                d = lexeme[i];
                if ((i < 20) && (d == '_' || (d >= 65 && d <= 90) || (d >= 97 && d <= 122) || (d >= 48 && d <= 57)))
                {
                    i++;
                    continue;
                }
                else
                    return "ERROR";
            }
            return "ID";
        }
    }
    return "Error";
}

void printLinkedList()
{ //Test function for printing entire LinkedList of Tokens
    token *temp = head;
    printf("\n NOW PRINTING \n\n");
    while (temp != NULL)
    {
        printf("%-10s->%-10s--->>%d\n", temp->lexeme, temp->token, temp->lineno);
        temp = temp->next;
    }
}
token *createNode(char *lexeme, int lineno)
{ //Return Dynamically Created Record for TOKEN
    token *a;
    a = (token *)malloc(sizeof(token));
    strcpy(a->lexeme, lexeme);
    strcpy(a->token, getToken(a->lexeme)); //To be hashed
    a->lineno = lineno;
    a->next = NULL;
    return a;
}
token *insertNode(token *node)
{ //Inserts Node to LinkedList Head, Tail USED
    if (head == NULL && tail == NULL)
    {
        head = tail = node;
        return head;
    }
    else
    {
        tail->next = node;
        tail = node;
    }
    return head;
}
void tokeniseSourcecode(char a[100], token *head)
{ //Function for File Redaing and creating LinkedList of Tokens
    FILE *fp;
    token *s;
    int count = 0;
    char buff[1000];
    fp = fopen(a, "r+");
    if (fp == NULL)
    {
        printf("ERRROR\n");
    }
    while (fscanf(fp, "%[^\n]\n", buff) != EOF)
    {
        count++;
        char *token_split;
        //token_split=strtok(buff," ");
        token_split = strtok(buff, " \t");
        while (token_split != NULL)
        {
            if (token_split != NULL)
            {
                s = createNode(token_split, count);
                insertNode(s);
            }
            //token_split=strtok(NULL," ");
            token_split = strtok(NULL, " \t");
        }
    }
    printLinkedList();
    fclose(fp);
}
/**************************END TOKEN FUNC IMPORTS***********************************************/

/**********************GRAMMAR STRUCT IMPORTS************************/
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

#define TOTAL_TERMINALS 35
#define TOTAL_NON_TERMINALS 37
#define NON_TERMINALS 37

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
/***********GRAMMAR STRUCT IMPORTS END***************/

/*****************************FUNCTION GRAMMAR IMPORTS**********************/
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

/********************GRAMMAR FUNC END**********************************/

/************************** STRUCTURE DEFINITIONS *****************************/
// Terminal Nodes
typedef struct terminalNode
{
    Terminal t;
    char lexeme[100];
    int line_num;
} TerminalNode;

// NonTerminal Nodes
typedef struct nonTerminalNode
{
    NonTerminal nt;
    grules *expRule; // for getting the rule used in parsing

} NonTerminalNode;

// Union for Term / NonTerm Nodes
typedef union nodeType
{
    TerminalNode terminal;
    NonTerminalNode nonTerminal;
} NodeType;

// ParseTree Node
typedef struct treeNode
{
    int isTerm;    // tag for term/non_term and leaf / non_leaf
    NodeType Node; // Details corresponding to a Terminal or a NonTerminal Node

    //TypeExpression t; // to be done
    // only for NonTerminal or Both ??

    struct treeNode *parent;  // parent of the node
    struct treeNode *sibling; // right sibling of the node

    //for linkedlist of children
    struct treeNode *firstChild;

} parseTree;

/**********************STRUCTURE END*********************************************/

/*******************************FUNCTIONS*************************************/

void deleteSubtree(parseTree *node)
{
    if (node == NULL)
        return;
    //delete all the children here
    parseTree *child = node->firstChild;
    while (child != NULL)
    {
        parseTree *nextChild = child->sibling;
        deleteSubtree(child);
        child = nextChild;
    }
    free(child);
}

parseTree *createTreeNode(rule *curr, parseTree *p)
{
    parseTree *node = malloc(sizeof(parseTree));
    node->isTerm = curr->isterm;

    node->parent = p;
    node->firstChild = NULL;
    node->sibling = NULL;

    if (curr->isterm)
    {
        node->Node.terminal.t = curr->type.t;
    }
    else
    {
        node->Node.nonTerminal.nt = curr->type.nt;
    }

    return node;
}
token *posToken;
parseTree *createParseTree(parseTree *p, grammar *G)
{
    if (posToken == NULL)
        return NULL;

    // Check if the current node is a terminal
    // no need to expand the node
    if (p->isTerm)
    {

        //printf("OUTSIDE TERMINAL: %s %s\n", p->Node.terminal.lexeme, TerminalMap[p->Node.terminal.t]);
        if (p->Node.terminal.t == EPSILON)
            return p;
        else if ((p->Node).terminal.t == find(posToken->token, 1))
        {
            // PRINTING
            printf("TERMINAL: %s %s\n", p->Node.terminal.lexeme, TerminalMap[p->Node.terminal.t]);
            //printf("TERMINAL: %s \n", TerminalMap[p->Node.terminal.t]);
            posToken = posToken->next;
            return p;
        }
        else
        {
            return NULL;
        }
    }

    // Now expand the current NonTerminal
    token *start = posToken;
    // Now pick the rule corresponding to the given NonTerminal

    grules *exp = G[p->Node.nonTerminal.nt].head;
    int found = 0;
    while (exp != NULL) // picking a partcular rule
    {
        //printf("The Rule Selected Now is: %s--> \n", NonTerminalMap[p->Node.nonTerminal.nt]);
        // Now traverse over the elements of the rule selected
        int ans = 1;
        rule *curr = exp->head;
        parseTree *prevChild = NULL;
        while (curr != NULL) // a particular element of the rule
        {
            //create The Node
            if (curr->isterm)
            {
                //printf(" %s ", TerminalMap[curr->type.t]);
            }
            else
            {
                //printf("< %s >", NonTerminalMap[curr->type.nt]);
            }
            parseTree *child = createTreeNode(curr, p);
            if (curr->isterm)
            {
                child->Node.terminal.line_num = posToken->lineno;
                strcpy(child->Node.terminal.lexeme, posToken->lexeme);
            }

            // Add to the first child of parent if prevChild is NULL
            if (prevChild == NULL)
                p->firstChild = child;
            else
                prevChild->sibling = child;

            prevChild = child;

            if (!createParseTree(child, G)) // cannot add this child
            {
                //printf("ERROR\n");
                ans = 0;
                break;
            }

            // move to the next rule as previous place
            curr = curr->next;
        }
        if (ans)
        {
            //printf("DONE\n");
            found = 1;
            break;
        }
        else
        {
            // backTracking Step
            // Revert the changes made
            // func for deleting the child nodes
            posToken = start;

            // delete all the childs of p ... starting from firstChild
            parseTree *child = p->firstChild;

            while (child != NULL)
            {
                parseTree *nextChild = child->sibling;
                // delete the subtree corresponding to this node
                deleteSubtree(child);
                child = nextChild;
            }
        }
        exp = exp->next;
    }
    if (!found)
    {
        // The non terminal which came cannot be expanded
        return NULL;
    }
    // PRINTING
    //printf("NON TERMINAL-->%s \n", NonTerminalMap[p->Node.nonTerminal.nt]);

    p->Node.nonTerminal.expRule = exp;
    return p; // return the node itself
}

/*PRINT PARSE TREE FUNC*/

// Symbol name
// Whether terminal or non terminal
// Type expression stored in the corresponding node (if non-leaf)
// Name of lexeme (if leaf node)
// Line number (if leaf node)
// Grammar rule applied for expansion of this node while parsing (if non leaf)
// Depth of node (root of the parse tree at depth 0)

void printRule(grules *expRule, NonTerminal nt, FILE *fp)
{
    char buff[200] = "\0";
    strcat(buff, NonTerminalMap[nt]);
    strcat(buff, " =>");
    //fprintf(fp, "%s => ", NonTerminalMap[nt]);
    rule *curr = expRule->head;

    while (curr != NULL)
    {
        // print the rule
        if (curr->isterm)
        {
            //fprintf(fp, " %s", TerminalMap[curr->type.t]);
            strcat(buff, " ");
            strcat(buff, TerminalMap[curr->type.t]);
        }
        else
        {
            //fprintf(fp, " <%s>", NonTerminalMap[curr->type.nt]);
            strcat(buff, " <");
            strcat(buff, NonTerminalMap[curr->type.nt]);
            strcat(buff, ">");
        }
        curr = curr->next;
    }
    fprintf(fp, "%-100s", buff);
}

void printParseTreeUtil(parseTree *t, FILE *fp, int depth)
{
    if (t == NULL)
        return;
    // print here the node and call the function for the children
    if (t->isTerm)
    {
        char *ter = "TERMINAL";
        fprintf(fp, "%-25s", TerminalMap[t->Node.terminal.t]); // print the symbol
        fprintf(fp, "%-25s", ter);
        fprintf(fp, "%-50s", "NULL");
        fprintf(fp, "%-30s", t->Node.terminal.lexeme);
        fprintf(fp, "%-10d", t->Node.terminal.line_num);
        fprintf(fp, "%-100s", "NULL");
        fprintf(fp, "%-10d\n", depth);
    }
    else
    {
        char *ter = "NON TERMINAL";
        fprintf(fp, "%-25s", NonTerminalMap[t->Node.nonTerminal.nt]); // print the symbol
        fprintf(fp, "%-25s", ter);
        fprintf(fp, "%-50s", "TO BE DONE"); // print the tyeExpression
        fprintf(fp, "%-30s", "NULL");
        fprintf(fp, "%-10s", "NULL");
        //fprintf(fp, "%-100s", "NULL"); // print The rule here
        printRule(t->Node.nonTerminal.expRule, t->Node.nonTerminal.nt, fp);
        fprintf(fp, "%-10d\n", depth);
    }
    parseTree *child = t->firstChild;
    while (child != NULL)
    {
        printParseTreeUtil(child, fp, depth + 1);
        child = child->sibling;
    }
}

void printParseTree(parseTree *t)
{
    // Initial setup.
    FILE *fp = fopen("parseTree.txt", "w");

    fprintf(fp, "\n------------------------------------------------------Printing Parse Tree-----------------------------------------------------\n\n");
    //printf("\n------------------------------------------------------Printing Parse Tree On the Console-----------------------------------------------\n\n");
    //fprintf(fp, "HELLO\n\n");
    //fprintf(fp, "%-25s %-10s %-15s %-15s %-30s %-5s %s\n\n\n", "LEXEME","LINE","TOKEN","VALUE","PARENT","LEAF","NODE");
    fprintf(fp, "%-25s %-25s %-50s %-30s %-10s %-100s %-10s\n\n\n", " SYMBOL", " TERMINAL / NON TERMINAL", " TYPE EXPRESSION", " LEXEME", " LINE", " GRAMMAR RULE", " DEPTH");

    printParseTreeUtil(t, fp, 0);

    fprintf(fp, "\n------------------------------------------------------Parse Tree Printed On the FILE-----------------------------------------------\n\n");
    fclose(fp);
}

/******************FUNCTION END*************************************/

int main()
{
    { // KEYWORD TABLE
        ha[0][0] = "program";
        ha[0][1] = "PROGRAM";
        ha[1][0] = "declare";
        ha[1][1] = "DECLARE";
        ha[2][0] = "list";
        ha[2][1] = "LIST";
        ha[3][0] = "of";
        ha[3][1] = "OF";
        ha[4][0] = "variables";
        ha[4][1] = "VARIABLES";
        ha[5][0] = "size";
        ha[5][1] = "SIZE";
        ha[6][0] = "values";
        ha[6][1] = "VALUES";
        ha[7][0] = "integer";
        ha[7][1] = "INTEGER";
        ha[8][0] = "real";
        ha[8][1] = "REAL";
        ha[9][0] = "boolean";
        ha[9][1] = "BOOLEAN";
        ha[10][0] = "array";
        ha[10][1] = "ARRAY";
        ha[11][0] = "jagged";
        ha[11][1] = "JAGGED";
        ha[12][0] = "R1";
        ha[12][1] = "R1";
        op[0][0] = "+";
        op[0][1] = "PLUS";
        op[1][0] = "-";
        op[1][1] = "MINUS";
        op[2][0] = "*";
        op[2][1] = "MUL";
        op[3][0] = "/";
        op[3][1] = "DIVIDE";
        op[4][0] = "(";
        op[4][1] = "BROP";
        op[5][0] = ")";
        op[5][1] = "BRCL";
        op[6][0] = "{";
        op[6][1] = "CURLYOP";
        op[7][0] = "}";
        op[7][1] = "CURLYCL";
        op[8][0] = "[";
        op[8][1] = "SQOP";
        op[9][0] = "]";
        op[9][1] = "SQCL";
        op[10][0] = "&&&";
        op[10][1] = "AND";
        op[11][0] = "|||";
        op[11][1] = "OR";
        op[12][0] = ":";
        op[12][1] = "COLON";
        op[13][0] = ";";
        op[13][1] = "SEMICOLON";
        op[14][0] = "..";
        op[14][1] = "DD";
        op[15][0] = "=";
        op[15][1] = "EQUALS";
    }
    // 1. Read the grammar.
    grammar *g = malloc(NON_TERMINALS * sizeof(grammar));
    readGrammar("gr.txt", g);

    // 2. Tokenize the source code
    tokeniseSourcecode("file.txt", head);

    // 3. Create the parseTree
    //create the start node
    posToken = head;
    parseTree *p = malloc(sizeof(parseTree));
    p->isTerm = 0;
    p->Node.nonTerminal.expRule = NULL;
    p->Node.nonTerminal.nt = s;
    p->parent = p;
    p->sibling = NULL;
    p->firstChild = NULL;

    createParseTree(p, g);

    if (posToken != NULL)
        printf("ERRRRRRORRRRRRRRRRR\n");
    //4. Printing the parse tree
    printParseTree(p);
    return 0;
}
