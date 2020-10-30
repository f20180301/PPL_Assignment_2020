/**
SHUBHAM AGARWAL -- 2018A7PS0301P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGRAWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#ifndef _PARSETREE_
#define _PARSETREE_

#include "typeExpressionTable.h"

// TERMINAL NODE
typedef struct terminalNode
{
    Terminal t;
    char lexeme[200];
    int line_num;
} TerminalNode;

// NonTerminal Nodes
typedef struct nonTerminalNode
{
    NonTerminal nt;
    grules *expRule; // for getting the rule used in parsing
	char lex_nt[200];
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

    //TypeExpression t; // to be done//done
    // only for NonTerminal or Both ??

    struct treeNode *parent;  // parent of the node
    struct treeNode *sibling; // right sibling of the node

    //for linkedlist of children
    struct treeNode *firstChild;
    TypeExpression exp_type;
    int depth;

} parseTree;

//***********FUNCTION DECLARATIONS*************//
void deleteSubtree(parseTree *);
parseTree *createTreeNode(rule *, parseTree *);
parseTree *createParseTree(parseTree *, grammar *);
void append_to_table(TypeExpression, char *);
TypeExpression jagged(int, int, parseTree *, int, TypeExpression);
void traverse_parse_tree(parseTree *);
void printRule(grules *, NonTerminal, FILE *);
void printParseTreeUtil(parseTree *, FILE *, int);
void printParseTree(parseTree *);
TypeExpression search_table(char * lex);
int check_idx(parseTree * p_idx,TypeExpression var_exp,int arr_size, char *lex, int line_no);
#endif
