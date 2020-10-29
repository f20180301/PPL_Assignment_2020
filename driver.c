/**
SHUBHAM AGARWAL -- 2018A7PS0301P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGRAWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readGrammar.c"
#include "tokeniseSourceCode.c"
#include "parseTree.c"
#include "typeExpressionTable.c"
int main(int argc, char *argv[])
{

    // 1. Read the grammar.
    grammar *g = malloc(NON_TERMINALS * sizeof(grammar));
    readGrammar("grammar.txt", g);

    // 2. Tokenize the source code
    if (argc == 2)
        tokeniseSourcecode(argv[1], head);
    else
    {
        printf("Please provide the sourcefile.txt as commaand line\n");
        return 0;
    }

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
    {
        printf("ERROR READING INPUT TESTCASE (IT OCCURS USUALLY IF AN UNKNOWN SYMBOLS APPEAR WHICH CANNOT DOEAS NOT MATCH ANY RULE)\n");
        return 0;
    }
    //4. Printing the parse tree
    printParseTree(p);

    traverse_parse_tree(p);
    printParseTree(p);
    printTypeExpressionsTable(table, "typeExpressionTable.txt");
    return 0;
}
