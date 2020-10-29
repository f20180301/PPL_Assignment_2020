/**
SHUBHAM AGARWAL -- 2018A7PS0310P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGARWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#ifndef _PARSETREE_
#define _PARSETREE_

#include "typeExpressionTable.h"

// TERMINAL NODE
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

    //TypeExpression t; // to be done//done
    // only for NonTerminal or Both ??

    struct treeNode *parent;  // parent of the node
    struct treeNode *sibling; // right sibling of the node

    //for linkedlist of children
    struct treeNode *firstChild;
    TypeExpression exp_type;

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

#endif