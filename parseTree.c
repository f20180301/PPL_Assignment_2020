/**
SHUBHAM AGARWAL -- 2018A7PS0301P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGRAWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#include "parseTree.h"

//*****DELETING SUBTREE***************//
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

//************CREATING A NODE OF A TREE******************//

parseTree *createTreeNode(rule *curr, parseTree *p)
{
    parseTree *node = malloc(sizeof(parseTree));
    node->isTerm = curr->isterm;

    node->parent = p;
    node->firstChild = NULL;
    node->sibling = NULL;
    node->exp_type.tag = not_app;
    node->depth = p->depth + 1;

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

//*************CREATE PARSE TREE**********************//

parseTree *createParseTree(parseTree *p, grammar *G)
{
    if (posToken == NULL)
        return NULL;

    // Check if the current node is a terminal
    // no need to expand the node
    if (p->isTerm)
    {

        //printf("TERMINAL: %s %s\n", p->Node.terminal.lexeme, TerminalMap[p->Node.terminal.t]);
        if (p->Node.terminal.t == EPSILON)
            return p;
        else if ((p->Node).terminal.t == find(posToken->token, 1))
        {
            // PRINTING
            //printf("TERMINAL: %s %s\n", p->Node.terminal.lexeme, TerminalMap[p->Node.terminal.t]);
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

//********************FUNCTION TO APPEND ROWS TO TYPE EXPRESSION TABLE*************//
void append_to_table(TypeExpression type, char *var)
{
    if (CAPACITY == 0)
    {

        table = (TypeExpressionTable *)malloc(sizeof(TypeExpressionTable) * 10);
        CAPACITY = 10;
    }
    else if (curr_table_entry == CAPACITY)
    {
        table = (TypeExpressionTable *)realloc(table, sizeof(TypeExpressionTable) * CAPACITY * 2);
        CAPACITY = CAPACITY * 2;
    }
    strcpy(table[curr_table_entry].var_name, var);

    table[curr_table_entry].tag = type.tag;
    table[curr_table_entry].info = type.info;
    table[curr_table_entry].record = type.record;

    curr_table_entry++;

    return;
}

//*************SEARCHING FOR A LEXEME IF DECLARED EARLIER**********************//

int search_type_expression_table(char *key)
{
    for (int i = 0; i < curr_table_entry; i++)
    {
        if ((strcmp(key, table[i].var_name) == 0) && table[i].tag == primitive && table[i].record.primitive_type == INTEGER)
        {
            return 1;
        }
    }
    return 0;
}

//***************TRAVERSE A JAGLINE -- FOR JAGGED_ARRAY************************//

TypeExpression jagged(int lo, int hi, parseTree *jagLines, int dimen, TypeExpression expr)
{
    expr.record.j_arr_record.dim = dimen;
    expr.record.j_arr_record.r1_high = hi;
    expr.record.j_arr_record.r1_low = lo;
    int indx = 0;     // To RECORD INDEX
    int szchk = lo;   // TO CHECK INDEX
    int sz = 0;       // TO RECORD SIZE IN EACH JAGLINE
    int numcount = 0; // TO TRACK SIZE
    int idxcount = 0; // For Jagged_3d_array to track no. of elemets in 3rd dimension
    int *line = NULL;
    int **jagl = NULL;
    parseTree *dpnumList = NULL;
    if (dimen == 2)
    {
        line = (int *)malloc(sizeof(int) * (hi - lo + 1));
    }
    else
    {
        jagl = (int **)malloc(sizeof(int *) * (hi - lo + 1));
    }

    parseTree *jagLine = jagLines->firstChild; //Pointing to a Jagline
    do
    {
        parseTree *jagList = jagLine->firstChild->sibling->sibling; //Pointing to Num like R1[NUM]
        indx = atoi(jagList->Node.terminal.lexeme);
        if (szchk != indx) // Checking index value
        {
            int dep = jagList->depth;
            if (dimen == 2)
            {
                //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Index_Mismatch \n", jagList->Node.terminal.line_num, dep);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",jagList->Node.terminal.line_num, "DECLARATION", "***", "***", "***", "***", "***", dep, "2D_Jagged_Array_Index_Mismatch");
            }
            else
            {
                //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_Index_Mismatch \n", jagList->Node.terminal.line_num, dep);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",jagList->Node.terminal.line_num, "DECLARATION", "***", "***", "***", "***", "***", dep, "3D_Jagged_Array_Index_Mismatch");
            }
            expr.tag = not_app;
            return expr;
        }
        szchk++;
        jagList = jagList->sibling->sibling->sibling->sibling; // Pointing to next Num sixe(NUM)
        sz = atoi(jagList->Node.terminal.lexeme);              //Receiving the size of the array
        numcount = 0;
        jagList = jagList->sibling->sibling->sibling->sibling; // Pointing to REAL JagList
        parseTree *numList = jagList->firstChild;              // Pointing to a numlist of rule <jag_list> => <num_list> SEMICOLON <jag_list> ||  <num_list>
        if (dimen == 2)
        {
            while (numList->sibling != NULL)
            {
                dpnumList = numList->firstChild;       // Pointing to NUM or EPSILON of rule <num_list> => NUM <num_list> || EPSILON
                if (dpnumList->Node.terminal.t == NUM) // Not more than two in a numList and single NUM to be present b/w semicolons
                {
                    numcount++;
                }
                if (dpnumList->Node.terminal.t == EPSILON)
                {
                    int dep = dpnumList->depth;
                    //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_No_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num, "DECLARATION", "***", "***", "***", "***", "***", dep, "2D_Jagged_Array_No_Element_In_Row ");
                    expr.tag = not_app;
                    return expr;
                }
                if (dpnumList->sibling->firstChild->Node.terminal.t != EPSILON)
                {
                    int dep = dpnumList->sibling->firstChild->depth;
                    //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Multiple_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num ,"DECLARATION", "***", "***", "***", "***", "***", dep, "2D_Jagged_Array_Multiple_Element_In_Row ");
                    expr.tag = not_app;
                    return expr;
                }
                numList = numList->sibling->sibling->firstChild; // Moving to next numList
            }
            if (numList->firstChild->Node.terminal.t == EPSILON)
            {
                int dep = numList->firstChild->depth;
                //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_No_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num ,"DECLARATION", "***", "***", "***", "***", "***", dep, "2D_Jagged_Array_No_Element_In_Row");
                expr.tag = not_app;
                return expr;
            }
            if (numList->firstChild->sibling->firstChild->Node.terminal.t != EPSILON)
            {
                int dep = numList->firstChild->sibling->firstChild->depth;
                //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Multiple_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num ,"DECLARATION", "***", "***", "***", "***", "***", dep, "2D_Jagged_Array_Multiple_Element_In_Row");
                expr.tag = not_app;
                return expr;
                // 3d type
            }
            numcount++;
            if (sz != numcount) // Matchng size with no. of elements counted
            {
                int dep = dpnumList->depth;
                //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Size_Mismatch \n", dpnumList->Node.terminal.line_num, dep);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num ,"DECLARATION", "***", "***", "***", "***", "***", dep, "2D_Jagged_Array_Size_Mismatch");
                expr.tag = not_app;
                return expr;
            }
            line[szchk - lo - 1] = sz;
        }
        else
        {
            int counter = 0;
            jagl[szchk - lo - 1] = (int *)malloc(sizeof(int) * (sz + 1));
            jagl[szchk - lo - 1][counter] = sz;
            while (numList->sibling != NULL) // Pointing to a numlist of rule <jag_list> => <num_list> SEMICOLON <jag_list> ||  <num_list>
            {
                idxcount = 0;
                dpnumList = numList->firstChild;              // Pointing to NUM or EPSILON of rule <num_list> => NUM <num_list> || EPSILON
                while (dpnumList->Node.terminal.t != EPSILON) // TRAVERSING THE RULE to eat all NUM in <num_list>
                {
                    idxcount++;
                    dpnumList = dpnumList->sibling->firstChild;
                }
                if (dpnumList->Node.terminal.t == EPSILON && idxcount == 0)
                {
                    int dep = dpnumList->depth;
                    //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_No_Element_In_3rd_Dimension \n", dpnumList->Node.terminal.line_num, dep);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num,"DECLARATION", "***", "***", "***", "***", "***", dep, "3D_Jagged_Array_No_Element_In_3rd_Dimension");
                    expr.tag = not_app;
                    return expr;
                }
                counter++;
                jagl[szchk - lo - 1][counter] = idxcount;
                numList = numList->sibling->sibling->firstChild;
            }
            idxcount = 0;
            dpnumList = numList->firstChild;
            while (dpnumList->Node.terminal.t != EPSILON) // TRAVERSING THE RULE to eat all NUM in <num_list>
            {
                idxcount++;
                dpnumList = dpnumList->sibling->firstChild;
            }
            if (dpnumList->Node.terminal.t == EPSILON && idxcount == 0)
            {
                int dep = dpnumList->depth;
                //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_No_Element_In_3rd_Dimension \n", dpnumList->Node.terminal.line_num, dep);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num, "DECLARATION", "***", "***", "***", "***", "***", dep, "3D_Jagged_Array_No_Element_In_3rd_Dimension");
                expr.tag = not_app;
                return expr;
            }
            counter++;
            jagl[szchk - lo - 1][counter] = idxcount;
            if (counter != sz)
            {
                int dep = dpnumList->depth;
                //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_Size_Mismatch \n", dpnumList->Node.terminal.line_num, dep);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",dpnumList->Node.terminal.line_num, "DECLARATION", "***", "***", "***", "***", "***", dep, "3D_Jagged_Array_Size_Mismatch");
                expr.tag = not_app;
                return expr;
            }
        }
        if (jagLine->sibling == NULL)
        {
            jagLine = NULL;
        }
        else
        {
            jagLine = jagLine->sibling->firstChild; // Move to next Jagline after Traversing a single Jagline
        }
    } while (jagLine != NULL);
    if (dimen == 2)
    {
        expr.record.j_arr_record.dim_bound._line = line;
    }
    else
    {
        expr.record.j_arr_record.dim_bound.jag_line = jagl;
    }
    return expr;
}

//*****************TRAVERSE PARSE TREE*************************//

void traverse_parse_tree(parseTree *t)
{
    if (t->isTerm == 0 && t->Node.nonTerminal.nt == s)
    {
        t = t->firstChild->sibling->sibling->sibling->sibling;
        traverse_parse_tree(t);
        //pointer t is pointing/moving to <start>
        return;
    }
    //pointer t is pointing/moving to <start>
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == start)
    {
        traverse_parse_tree(t->firstChild);
        traverse_parse_tree(t->firstChild->sibling);
        return;
    }

    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == declaration)
    {
        traverse_parse_tree(t->firstChild); //to dec_stmnt
        if (t->firstChild->sibling != NULL)
        {
            traverse_parse_tree(t->firstChild->sibling); //to declarations or null
        }
        return;
    }
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == declaration_stmt)
    {
        //build for the "typeExpression" of this declarartion statement, error reporting
        TypeExpression exp_table_record;
        parseTree *varlist_pointer = NULL;
        parseTree *temp = t; //just being safe, making copy
        temp = temp->firstChild;

        //***********PRIMITIVE**********************//

        if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == primitive)
        {
            ///    printf("herein\n");
            exp_table_record.tag = primitive; //primitive/jagged_array/array//terminal
            exp_table_record.info = N_A;
            temp = temp->firstChild; //pointing to var_list  //moving into single/multiple_primitive node //populate type expressions
            if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == single_primitive)
            {
                varlist_pointer = temp = temp->firstChild->sibling;
                temp = temp->sibling->sibling; //moving into colon ke baad wala node
            }
            else
            { //multiple
                varlist_pointer = temp = temp->firstChild->sibling->sibling->sibling->sibling;
                temp = temp->sibling->sibling; //moving into colon ke baad wala node
            }
            //
            exp_table_record.record.primitive_type = temp->firstChild->Node.terminal.t; //INT/BOOl/REAL(terminal)
            //storing to <single_primitive> and <primitive>
            temp->parent->exp_type = exp_table_record;
            temp->parent->parent->exp_type = exp_table_record;
        }

        //***********ARRAY**********************//

        else if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == array)
        {
            exp_table_record.tag = array;
            exp_table_record.info = Static;
            temp = temp->firstChild;

            int line = temp->firstChild->Node.terminal.line_num;
            if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == single_array)
            {
                varlist_pointer = temp = temp->firstChild->sibling;
                temp = temp->sibling->sibling; //moving into part after colon
            }
            else
            {
                varlist_pointer = temp = temp->firstChild->sibling->sibling->sibling->sibling;
                temp = temp->sibling->sibling; //moving into part after colon
            }

            temp = temp->sibling; //(ignore just after colon, arrray written)

            //Now temp at <array_dim>
            parseTree *store = temp; //at <array_dim>

            int count = 0;
            int strcount = 0;
            int strcount2 = 0;
            int r1 = -1, r2 = -1;
            int **tem = (int **)malloc(2 * sizeof(int *));
            int s_g = 1;
            int s_g2 = 1;
            char **l_indexes = (char **)malloc(sizeof(char *));
            char **u_indexes = (char **)malloc(sizeof(char *));

            int g = 1, b = 2;

            do
            {

                temp = temp->firstChild; //(moving into the child of  <array_dim>)
                temp = temp->sibling;

                //id or num =temp->firstChild;

                if (temp->firstChild->Node.terminal.t == ID) //checking if dimension is ID.
                {
                    exp_table_record.info = Dynamic;
                    r1 = -1;
                }
                else
                { // If dimension is NUM
                    r1 = atoi(temp->firstChild->Node.terminal.lexeme);
                    if (r1 == -1)
                        r1 = -100;
                }
                if (r1 == -1)
                {
                    if (strcount >= s_g)
                    {
                        l_indexes = (char **)realloc(l_indexes, (s_g * 2) * sizeof(char *));
                        s_g = s_g * 2;
                    }
                    int len = strlen(temp->firstChild->Node.terminal.lexeme);
                    l_indexes[strcount] = (char *)malloc(len * sizeof(char)); //allocating space to store left and right index
                    if (!search_type_expression_table(temp->firstChild->Node.terminal.lexeme))
                    {
                        exp_table_record.tag = not_app;
                        exp_table_record.info = N_A; //erroneous type declaration, storing N_A in the field in node.
                        int dep = temp->firstChild->depth;
                        //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Array_Range_Invalid_Variable \n", line, dep);
                        printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line, "DECLARATION", "***", "***", "***", "***", "***", dep, "Array_Range_Invalid_Variable");
                        break;
                    }
                    strcpy(l_indexes[strcount], temp->firstChild->Node.terminal.lexeme);
                    // Storing ID in form of string
                    strcount++;
                }
                else
                {
                    if (strcount >= s_g)
                    {
                        l_indexes = (char **)realloc(l_indexes, (s_g * 2) * sizeof(char *));
                        s_g = s_g * 2;
                    }
                    int len = 12;
                    l_indexes[strcount] = (char *)malloc(len * sizeof(char)); //allocating space to store left and right index
                    strcpy(l_indexes[strcount], "NumericValue");
                    // Storing ID in form of string
                    // printf("l-> %s %s %d\n",temp->firstChild->Node.terminal.lexeme,l_indexes[strcount],strcount );
                    strcount++;
                }

                temp = temp->sibling->sibling; //ignoring .. (DD)

                if (temp->firstChild->Node.terminal.t == ID) // checking if dimension is ID
                {
                    exp_table_record.info = Dynamic;

                    r2 = -1;
                }
                else
                { // If dimension is NUM
                    r2 = atoi(temp->firstChild->Node.terminal.lexeme);
                    if (r2 == -1)
                        r2 = -100;
                }
                if (r2 == -1)
                {
                    if (strcount2 >= s_g2)
                    {
                        u_indexes = (char **)realloc(u_indexes, (s_g2 * 2) * sizeof(char *));
                        s_g2 = s_g2 * 2;
                    }
                    int len = strlen(temp->firstChild->Node.terminal.lexeme);
                    u_indexes[strcount2] = (char *)malloc(len * sizeof(char)); //allocating space to store left and right index
                    if (!search_type_expression_table(temp->firstChild->Node.terminal.lexeme))
                    {
                        exp_table_record.tag = not_app;
                        exp_table_record.info = N_A; //erroneous type declaration, storing N_A in the field in node.
                        int dep = temp->firstChild->depth;
                        //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Array_Range_Invalid_Variable \n", line, dep);
                        printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line, "DECLARATION", "***", "***", "***", "***", "***", dep, "Array_Range_Invalid_Variable");
                        break;
                    }
                    strcpy(u_indexes[strcount2], temp->firstChild->Node.terminal.lexeme);
                    // Storing ID in form of string
                    // printf("u-> %s : %s %d\n",temp->firstChild->Node.terminal.lexeme,u_indexes[strcount2],strcount2 );
                    strcount2++;
                }
                else
                {
                    if (strcount2 >= s_g2)
                    {
                        u_indexes = (char **)realloc(u_indexes, (s_g2 * 2) * sizeof(char *));
                        s_g2 = s_g2 * 2;
                    }
                    int len = 12;
                    u_indexes[strcount2] = (char *)malloc(len * sizeof(char)); //allocating space to store left and right index
                    strcpy(u_indexes[strcount2], "NumericValue");
                    //printf("u -> %s %s %d\n",temp->firstChild->Node.terminal.lexeme,u_indexes[strcount2],strcount2 );
                    //Storing ID in form of string
                    strcount2++;
                }

                temp = temp->sibling->sibling; //reaches <array_dim>

                //chk type(dynamic), dimension(lower<upper), read, and error report, maintain counter for Dimension Count, dimension=count;
                //store to type_Expression_record.record.arr_record;
                if ((r1 == -1 && r2 >= 0) || (r1 >= 0 && r2 == -1) || (r1 >= 0 && r2 >= 0 && r1 <= r2) || (r1 == -1 && r2 == -1)) //(-1 for ID)
                //checking if lower_index < higher_index and ignore if one (or both) index are ID and NUM should be >= 0
                {
                    count++;
                    exp_table_record.record.arr_record.dim = count;
                    if (count > g * b)
                    {
                        tem = (int **)realloc(tem, 2 * (count - 1) * sizeof(int *));
                        g++;
                    }
                    tem[count - 1] = (int *)malloc(2 * sizeof(int)); //allocating space to store left and right index
                    tem[count - 1][0] = r1;
                    tem[count - 1][1] = r2;
                    exp_table_record.record.arr_record.dim_bound = tem;
                    exp_table_record.record.arr_record.l_indexes = l_indexes;
                    exp_table_record.record.arr_record.u_indexes = u_indexes;

                    store->parent->exp_type = exp_table_record;
                    store->parent->parent->exp_type = exp_table_record;
                }
                else //if lower_range becomes greater than higher_range
                {
                    exp_table_record.tag = not_app;
                    exp_table_record.info = N_A; //erroneous type declaration, storing N_A in the field in node.

                    int dep = store->depth;
                    //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Array_Range_Mismatch \n", line, dep);
                     printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line, "DECLARATION", "***", "***", "***", "***", "***", dep, "Array_Range_Mismatch");
                    break;
                }

            } while (temp != NULL);
        }

        //**********JAGGED ARRAY**********************//

        else if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == jagged_array)
        {
            temp = temp->firstChild; //jagged_2d_array or jagged_3d_array
            if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == jagged_2d_array)
            {
                exp_table_record.tag = jagged_array;
                exp_table_record.info = N_A;
                temp = temp->firstChild; //single_jagged_2d or multi_jagged_2d
                //printf("%s nn\n", NonTerminalMap[temp->Node.nonTerminal.nt]);
                if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == single_jagged_2d_array)
                {
                    varlist_pointer = temp = temp->firstChild->sibling;
                    temp = temp->sibling->sibling; //moving into part after colon
                }
                else
                {
                    varlist_pointer = temp = temp->firstChild->sibling->sibling->sibling->sibling;
                    temp = temp->sibling->sibling; //moving into part after colon
                }
                temp = temp->sibling->sibling; //(ignore just after colon, jagged and arrray written)
                //<jagged_2d_dim> hai , in temp
                parseTree *store = temp;                                             //at <jagged_2d_dim>
                int l_bound = atoi(temp->firstChild->sibling->Node.terminal.lexeme); //at NUM
                int u_bound = atoi(temp->firstChild->sibling->sibling->sibling->Node.terminal.lexeme);
                int dimension = 2;
                //temp <jagged_2d_dim> pe hi hai
                if (l_bound <= u_bound)
                { //chk =
                    //traverse for jaglines
                    temp = temp->sibling->sibling->sibling->sibling; //reach <jag_lines>
                    exp_table_record = jagged(l_bound, u_bound, temp, dimension, exp_table_record);
                }
                else
                {
                    //error
                    int dep = store->depth;
                    //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Jagged_Array_Range_Mismatch \n", temp->firstChild->sibling->Node.terminal.line_num, dep);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",temp->firstChild->sibling->Node.terminal.line_num, "DECLARATION", "***", "***", "***", "***", "***", dep, "Jagged_Array_Range_Mismatch");
                    exp_table_record.tag = not_app;
                }
                store->parent->exp_type = exp_table_record;
                store->parent->parent->exp_type = exp_table_record;
                store->parent->parent->parent->exp_type = exp_table_record;
            }
            else if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == jagged_3d_array)
            {
                exp_table_record.tag = jagged_array;
                exp_table_record.info = N_A;
                temp = temp->firstChild;
                if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == single_jagged_3d_array)
                {
                    varlist_pointer = temp = temp->firstChild->sibling;
                    temp = temp->sibling->sibling; //moving into part after colon
                }
                else
                {
                    varlist_pointer = temp = temp->firstChild->sibling->sibling->sibling->sibling;
                    temp = temp->sibling->sibling; //moving into part after colon
                }
                temp = temp->sibling->sibling; //(ignore just after colon, jagged and arrray written)
                parseTree *store = temp;       //at <jagged_2d/3d_dim>
                int l_bound = atoi(temp->firstChild->sibling->Node.terminal.lexeme);
                int u_bound = atoi(temp->firstChild->sibling->sibling->sibling->Node.terminal.lexeme);
                //printf("%d %d", l_bound, u_bound);
                int dimension = 3;
                if (l_bound <= u_bound)
                {
                    exp_table_record = jagged(l_bound, u_bound, temp->sibling->sibling->sibling->sibling, dimension, exp_table_record);
                }
                else
                {
                    int dep = store->depth;
                    //printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Jagged_Array_Range_Mismatch \n", temp->firstChild->sibling->Node.terminal.line_num, dep);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n",temp->firstChild->sibling->Node.terminal.line_num, "DECLARATION", "***", "***", "***", "***", "***", dep, "Jagged_Array_Range_Mismatch");
                }
                store->parent->exp_type = exp_table_record;
                store->parent->parent->exp_type = exp_table_record;
                store->parent->parent->parent->exp_type = exp_table_record;
            }
        }

        if (varlist_pointer != NULL) // SAFETY CHECK
        {
            if (varlist_pointer->isTerm == 1 && varlist_pointer->Node.terminal.t == ID)
            {
                varlist_pointer->exp_type = exp_table_record;
                //append to typeExpression Table
                append_to_table(exp_table_record, varlist_pointer->Node.terminal.lexeme);
            }
            else
            {
                //traverse all IDs
                do
                {
                    varlist_pointer->exp_type = exp_table_record;
                    varlist_pointer = varlist_pointer->firstChild;
                    varlist_pointer->exp_type = exp_table_record;
                    //append to typeExpression Table
                    append_to_table(exp_table_record, varlist_pointer->Node.terminal.lexeme);
                    varlist_pointer = varlist_pointer->sibling;
                } while (varlist_pointer != NULL);
            }
        }
        return;
    }

    /*********/ //RAJAT/************/

    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == assignment)
    {

        //<assignment> => <assignment_stmt> <assignment>
        //<assignment> => <assignment_stmt>

        traverse_parse_tree(t->firstChild);
        if (t->firstChild->sibling != NULL)
        {
            traverse_parse_tree(t->firstChild->sibling);
        }
        return;
    }

    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == assignment_stmt)
    {

        //<assignment_stmt> => <arr_pid> EQUALS <expression> SEMICOLON
	
        traverse_parse_tree(t->firstChild);                   // check for the validity of the id on left
        traverse_parse_tree(t->firstChild->sibling->sibling); // traverse the expression part
			
		int line_no = t->firstChild->sibling->Node.terminal.line_num;
		char left[100], right[100];
		char message[100];
		
		// for left
		if(t->firstChild->exp_type.tag == primitive)
			strcpy(left, TerminalMap[t->firstChild->exp_type.record.primitive_type]);
		else
			strcpy(left, NonTerminalMap[t->firstChild->exp_type.tag]);
		// for right
		if(t->firstChild->sibling->sibling->exp_type.tag == primitive)
		{
			strcpy(right, TerminalMap[t->firstChild->sibling->sibling->exp_type.record.primitive_type]);
		}
		else
			strcpy(right, NonTerminalMap[t->firstChild->sibling->sibling->exp_type.tag]);
		
        // Now check the validity of the assignment statement and make changes int the parent t node
        // both operands need to be of same type
        if (t->firstChild->exp_type.tag == not_app || t->firstChild->sibling->sibling->exp_type.tag == not_app)
        {
            //printf("ERROR In assignment\n");
            printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as LHS and RHS are of different types.");
            // ignoring in original here
        }
        else if (t->firstChild->exp_type.tag == t->firstChild->sibling->sibling->exp_type.tag)
        {
            if (t->firstChild->exp_type.tag == primitive)
            {
                //printf("INSIDE PRIMITIVE\n");
                if (t->firstChild->exp_type.record.primitive_type == t->firstChild->sibling->sibling->exp_type.record.primitive_type)
                {
                    t->exp_type = t->firstChild->exp_type;
                    strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.nonTerminal.lex_nt);
                }
                else
                {
                    //printf("ERROR... Operands should be of same Type in = \n");
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as LHS and RHS are of different types.");
                    t->exp_type.tag = not_app;
                }
            }
            else if (t->firstChild->exp_type.tag == array)
            {
                //printf("INSIDE ARRAY\n");
                // check if the dims are equal
                if (t->firstChild->exp_type.record.arr_record.dim == t->firstChild->sibling->sibling->exp_type.record.arr_record.dim)
                    t->exp_type = t->firstChild->exp_type;
                else
                {
                    //printf("ERROR... Arrays need to be of same dimension = \n");
                    sprintf(message, "Type Array as LHS and RHS are array of different sizes %d and %d", t->firstChild->exp_type.record.arr_record.dim, t->firstChild->sibling->sibling->exp_type.record.arr_record.dim);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth,message);
                    t->exp_type.tag = not_app;
                }
            }
            else if (t->firstChild->exp_type.tag == jagged_array)
            {
                //printf("INSIDE JAGGED\n");
                // check the dimensions
                if (t->firstChild->exp_type.record.j_arr_record.dim == t->firstChild->sibling->sibling->exp_type.record.j_arr_record.dim)
                    t->exp_type = t->firstChild->exp_type;
                else
                {
                    sprintf(message, "Type Array as LHS and RHS are array of different sizes %d and %d", t->firstChild->exp_type.record.j_arr_record.dim , t->firstChild->sibling->sibling->exp_type.record.j_arr_record.dim);
                   printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth,message);
                    t->exp_type.tag = not_app;
                }
            }
        }
        else
        {
            //printf("ERROR... Operands Should be of same type in =\n");
            printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as LHS and RHS are of different types.");
            t->exp_type.tag = not_app;
        }
        strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.nonTerminal.lex_nt);
        return;
    }
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == expression) // expression
    {
        //<expression> => <arithmetic_expression>
        traverse_parse_tree(t->firstChild);
        // store the info in parent t node.
        t->exp_type = t->firstChild->exp_type;
        strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.nonTerminal.lex_nt);
        return;
    }
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == arithmetic_expression) // arithmetic_expression
    {
        // can have only single term or an exp with  operand
        //<arithmetic_expression> => <term> <op1> <arithmetic_expression>
        //<arithmetic_expression> => <term>
        // two cases
        traverse_parse_tree(t->firstChild);
        if (t->firstChild->sibling == NULL)
        {
            // make amends in the parent t node.
            t->exp_type = t->firstChild->exp_type;
            strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.nonTerminal.lex_nt);
            return;
        }
        else
        {
            traverse_parse_tree(t->firstChild->sibling->sibling);
            // Check compatibility from the two statements then make amends in the parent t node.
            //<op1> => PLUS
            //<op1> => MINUS
            //<op1> => AND
            //<op1> => OR
            // if plus, minus // operands can be primitive integer, real
            // if and, or // operands can be primitive boolean
            
            int line_no = t->firstChild->sibling->firstChild->Node.terminal.line_num;
			char left[100], right[100];
			char message[100];
			
			// for left
			if(t->firstChild->exp_type.tag == primitive)
				strcpy(left, TerminalMap[t->firstChild->exp_type.record.primitive_type]);
			else
				strcpy(left, NonTerminalMap[t->firstChild->exp_type.tag]);
			// for right
			if(t->firstChild->sibling->sibling->exp_type.tag == primitive)
			{
				strcpy(right, TerminalMap[t->firstChild->sibling->sibling->exp_type.record.primitive_type]);
			}
			else
				strcpy(right, NonTerminalMap[t->firstChild->sibling->sibling->exp_type.tag]);
            
            if(t->firstChild->exp_type.tag == not_app || t->firstChild->sibling->sibling->exp_type.tag == not_app)
            {
				printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", t->firstChild->sibling->firstChild->Node.terminal.lexeme, t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as both Operands are of Different Types");
                        t->exp_type.tag = not_app;
                        return;
			}
            
            if (t->firstChild->exp_type.tag == t->firstChild->sibling->sibling->exp_type.tag) // operands need to be of same type
            {
                if (t->firstChild->sibling->firstChild->Node.terminal.t == AND || t->firstChild->sibling->firstChild->Node.terminal.t == OR)
                {
                    // operands need to boolean
                    if (t->firstChild->exp_type.tag == primitive && t->firstChild->sibling->sibling->exp_type.tag == primitive && t->firstChild->exp_type.record.primitive_type == BOOLEAN)
                    {
                        t->exp_type = t->firstChild->exp_type;
                    }
                    else
                    {
                        //printf("ERROR... Operands need to be boolean with %s\n", t->firstChild->sibling->firstChild->Node.terminal.lexeme);
                        printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", t->firstChild->sibling->firstChild->Node.terminal.lexeme, t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as both the operands need to be BOOLEAN");
                        t->exp_type.tag = not_app;
                    }
                }
                else // operator is mult and div
                {
                    // operands should not be boolean
                    if (t->firstChild->exp_type.record.primitive_type == BOOLEAN)
                    {
                        //printf("ERROR... Operands cannot be boolean with %s\n", t->firstChild->sibling->firstChild->Node.terminal.lexeme);
                        printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", t->firstChild->sibling->firstChild->Node.terminal.lexeme, t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as operands cannot be BOOLEAN");
                        t->exp_type.tag = not_app;
                    }
                    else
                    {
                        // operands are of same type
                        t->exp_type = t->firstChild->exp_type;
                        
                        if (t->firstChild->exp_type.tag == array)
						{
							// check if the dims are equal
							if (t->firstChild->exp_type.record.arr_record.dim == t->firstChild->sibling->sibling->exp_type.record.arr_record.dim)
								t->exp_type = t->firstChild->exp_type;
							else
							{
								//printf("ERROR... Arrays need to be of same dimension\n");
								 sprintf(message, "Type Array as LHS and RHS are array of different sizes %d and %d", t->firstChild->exp_type.record.arr_record.dim, t->firstChild->sibling->sibling->exp_type.record.arr_record.dim);
							printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth,message);
								t->exp_type.tag = not_app;
							}
						}
						else if (t->firstChild->exp_type.tag == jagged_array)
						{
							// check the dimensions
							if (t->firstChild->exp_type.record.j_arr_record.dim == t->firstChild->sibling->sibling->exp_type.record.j_arr_record.dim)
								t->exp_type = t->firstChild->exp_type;
							else
							{
								//printf("ERROR... Arrays need to be of same dimension\n");
								 sprintf(message, "Type Array as LHS and RHS are array of different sizes %d and %d", t->firstChild->exp_type.record.arr_record.dim, t->firstChild->sibling->sibling->exp_type.record.arr_record.dim);
							printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth,message);
								t->exp_type.tag = not_app;
							}
						}
								
                    }
                }
            }
            else
            {
                //printf("ERROR... Operands need to be of same type in %s\n", NonTerminalMap[arithmetic_expression]);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", t->firstChild->sibling->firstChild->Node.terminal.lexeme, t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as LHS and RHS are of different types.");
                t->exp_type.tag = not_app;
            }
        }
        strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.nonTerminal.lex_nt);
        return;
    }
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == term) // term
    {
        //<term> => <factor> <op2> <term>
        //<term> => <factor>
        traverse_parse_tree(t->firstChild);
        if (t->firstChild->sibling == NULL)
        {
            // make amends in the parent t node.
            t->exp_type = t->firstChild->exp_type;
        }
        else
        {

            traverse_parse_tree(t->firstChild->sibling->sibling);
            // Check compatibility of the two and make amends in the parent t node.
            //<op2> => DIVIDE
            //<op2> => MUL
            // both operand real or integer
            // in div integer / integer will give real
			
			int line_no = t->firstChild->sibling->firstChild->Node.terminal.line_num;
			char left[100], right[100];
			char message[100];
			
			// for left
			if(t->firstChild->exp_type.tag == primitive)
				strcpy(left, TerminalMap[t->firstChild->exp_type.record.primitive_type]);
			else
				strcpy(left, NonTerminalMap[t->firstChild->exp_type.tag]);
			// for right
			if(t->firstChild->sibling->sibling->exp_type.tag == primitive)
			{
				strcpy(right, TerminalMap[t->firstChild->sibling->sibling->exp_type.record.primitive_type]);
			}
			else
				strcpy(right, NonTerminalMap[t->firstChild->sibling->sibling->exp_type.tag]);
			

            // operands need to be of same type
            if (t->firstChild->exp_type.tag == t->firstChild->sibling->sibling->exp_type.tag)
            {
                if (t->firstChild->exp_type.tag == array)
                {
                    // check if the dims are equal
                    if (t->firstChild->exp_type.record.arr_record.dim == t->firstChild->sibling->sibling->exp_type.record.arr_record.dim)
                        t->exp_type = t->firstChild->exp_type;
                    else
                    {
                        //printf("ERROR... Arrays need to be of same dimension\n");
                         sprintf(message, "Type Array as LHS and RHS are array of different sizes %d and %d", t->firstChild->exp_type.record.arr_record.dim, t->firstChild->sibling->sibling->exp_type.record.arr_record.dim);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth,message);
                        t->exp_type.tag = not_app;
                    }
                }
                else if (t->firstChild->exp_type.tag == jagged_array)
                {
                    // check the dimensions
                    if (t->firstChild->exp_type.record.j_arr_record.dim == t->firstChild->sibling->sibling->exp_type.record.j_arr_record.dim)
                        t->exp_type = t->firstChild->exp_type;
                    else
                    {
                        //printf("ERROR... Arrays need to be of same dimension\n");
                         sprintf(message, "Type Array as LHS and RHS are array of different sizes %d and %d", t->firstChild->exp_type.record.arr_record.dim, t->firstChild->sibling->sibling->exp_type.record.arr_record.dim);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "=", t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth,message);
                        t->exp_type.tag = not_app;
                    }
                }
                else if (t->firstChild->exp_type.record.primitive_type == BOOLEAN) // operands cannot be boolean
                {
                    //printf("ERROR... Operands cannot be BOOLEAN with %s\n", t->firstChild->sibling->firstChild->Node.terminal.lexeme);
                    printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", t->firstChild->sibling->firstChild->Node.terminal.lexeme, t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as operands cannot be BOOLEAN");
                    t->exp_type.tag = not_app;
                }
                else
                {
                    // handle seperately the case of primitives
                    t->exp_type = t->firstChild->exp_type;
                    if (t->firstChild->sibling->Node.terminal.t == DIVIDE && t->firstChild->exp_type.record.primitive_type == INTEGER) // both operands are integer
                    {
                        t->exp_type.record.primitive_type = REAL;
                    }
                }
            }
            else
            {
                //printf("ERROR... Operands need to be of same type in %s\n", t->firstChild->sibling->firstChild->Node.terminal.lexeme);
                 printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", t->firstChild->sibling->firstChild->Node.terminal.lexeme, t->firstChild->Node.nonTerminal.lex_nt, left, t->firstChild->sibling->sibling->Node.nonTerminal.lex_nt, right, t->depth, "TYPE ERROR as LHS and RHS are of different types.");
				t->exp_type.tag = not_app;
            }
        }
        strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.nonTerminal.lex_nt);
        return;
    }
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == factor) // factor
    {
        //<factor> => BROP <expression> BRCL
        //<factor> => NUM
        //<factor> => <arr_pid>

        if (t->firstChild->isTerm == 0) // arr_pid
        {
            traverse_parse_tree(t->firstChild); // check the arr_pid
            // Now store the info in the parent tree node t
            t->exp_type = t->firstChild->exp_type;
            strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.nonTerminal.lex_nt);
            return;
        }
        else if (t->firstChild->Node.terminal.t == BROP) // go to the expression
        {
            traverse_parse_tree(t->firstChild->sibling);
            // store the info in the parent node t
            t->exp_type = t->firstChild->sibling->exp_type;
            strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->sibling->Node.nonTerminal.lex_nt);
        }
        else // this is a num
        {
            // store the info in the parent node t
            // can be INTEGER
            t->exp_type.info = N_A;
            t->exp_type.tag = primitive;
            t->exp_type.record.primitive_type = INTEGER;
            strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.terminal.lexeme);
        }
        return;
    }
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == arr_pid) // arr_pid
    {
        //<arr_pid> => ID SQOP <idx> SQCL
        //<arr_pid> => ID
        int line_no = t->firstChild->Node.terminal.line_num;
		char left[100];
		//char message[100];
			
			// for left
			if(t->firstChild->exp_type.tag == primitive)
				strcpy(left, TerminalMap[t->firstChild->exp_type.record.primitive_type]);
			else
				strcpy(left, NonTerminalMap[t->firstChild->exp_type.tag]);
        if (t->firstChild->sibling == NULL)
        {
            // copy the details of this type expression in arr_pid
            // find the typeExpression int the TypeExpression Table

            t->exp_type = search_table(t->firstChild->Node.terminal.lexeme);
            if(t->exp_type.tag == primitive)
				strcpy(left, TerminalMap[t->exp_type.record.primitive_type]);
			else
				strcpy(left, NonTerminalMap[t->exp_type.tag]);
            if (t->exp_type.tag == not_app)
            {
                //printf("TYPERROR VARIABLE DEFINED CARRIES AN ERROR\n");
                 printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "***", t->firstChild->Node.terminal.lexeme, "***", "***", "***", t->depth, "ERROR The Variable is not declared");
                strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.terminal.lexeme);
                return;
            }
        }
        else
        {
            // PRECHECK BEFORE GOING INTO IDX
            parseTree *p_idx = t->firstChild->sibling->sibling;
            //if(p_idx->isTerm == 1) // EPSILON PRESENT
            //{
            //printf("ERROR...ARRAY NOT DEREFERENCED CORRECTLY\n");
            //t->exp_type.tag = not_app;
            //return;
            //}
            int arr_size = 0;
            // get the typeExpression of the variable from the table
            TypeExpression var_exp = search_table(t->firstChild->Node.terminal.lexeme);
            if(var_exp.tag == primitive)
				strcpy(left, TerminalMap[var_exp.record.primitive_type]);
			else
				strcpy(left, NonTerminalMap[var_exp.tag]);
            if (var_exp.tag == not_app)
            {
                //printf("ERROR...%s VARIABLE DEFINED HAS ERROR\n", t->firstChild->Node.terminal.lexeme);
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "***", t->firstChild->Node.terminal.lexeme, "***", "***", "***", t->depth, "ERROR The Variable is not declared");
                t->exp_type.tag = not_app;
                return;
            }
            // Now check the size and ranges
            if (var_exp.tag == primitive)
            {
                //printf("ERROR... Primitives cannot be Dereferenced\n");
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "***", t->firstChild->Node.terminal.lexeme, TerminalMap[t->firstChild->exp_type.record.primitive_type], "***", "***", t->depth, "Primitives cannot be dereferenced");
                t->exp_type.tag = not_app;
                return;
            }
            else if (var_exp.tag == jagged_array)
            {
                arr_size = var_exp.record.j_arr_record.dim;
            }
            else if (var_exp.tag == array)
            {
                arr_size = var_exp.record.arr_record.dim;
            }
            // GO INTO IDX
            // Now recursively check
            //<idx> => <num_id> <idx>
            //<idx> => EPSILON
            // after checking all the idx's
            char lex[100];
            strcpy(lex, t->firstChild->Node.terminal.lexeme);
            if (check_idx(p_idx, var_exp, arr_size, lex, line_no)) // all idx's are valid i.e. range and dimensions
            {
                // store the info in arr_pid node
                // The expType will change will no longer be array
                t->exp_type.info = N_A;
                t->exp_type.tag = primitive;
                t->exp_type.record.primitive_type = INTEGER;
            }
            else
            {
                // store error in the arr_pid node
                t->exp_type.tag = not_app;
            }
        }
        strcpy(t->Node.nonTerminal.lex_nt, t->firstChild->Node.terminal.lexeme);
        return;
    }
}

//*****************HELPER FUNCTION TO PRINT A PARTICULAR RULE**************//

void printRule(grules *expRule, NonTerminal nt, FILE *fp)
{
    char buff[200] = "\0";
    strcat(buff, NonTerminalMap[nt]);
    strcat(buff, " =>");
    rule *curr = expRule->head;

    while (curr != NULL)
    {
        // print the rule
        if (curr->isterm)
        {
            strcat(buff, " ");
            strcat(buff, TerminalMap[curr->type.t]);
        }
        else
        {
            strcat(buff, " <");
            strcat(buff, NonTerminalMap[curr->type.nt]);
            strcat(buff, ">");
        }
        curr = curr->next;
    }
    fprintf(fp, "%-200s", buff);
}

//**********************UTILITY FUNCTION FOR PRINTING PARSE TREE***************************//

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
        fprintf(fp, "%-150s", "NULL");
        fprintf(fp, "%-35s", t->Node.terminal.lexeme);
        fprintf(fp, "%-10d", t->Node.terminal.line_num);
        fprintf(fp, "%-200s", "NULL");
        fprintf(fp, "%-10d\n", depth);
    }

    else
    {
        char *ter = "NON TERMINAL";
        fprintf(fp, "%-25s", NonTerminalMap[t->Node.nonTerminal.nt]); // print the symbol
        fprintf(fp, "%-25s", ter);
        char exp[300] = "\0";
        char exp2[300] = "\0";
        if (t->exp_type.tag == not_app)
        {
            sprintf(exp, "%-150s", "--Ignored--");
        }
        else
        {
            if (t->exp_type.tag == primitive)
            { //Primitive
                sprintf(exp, "<basicType=%s>", TerminalMap[t->exp_type.record.primitive_type]);
            }
            else if (t->exp_type.tag == array)
            {
                int dim = t->exp_type.record.arr_record.dim;
                sprintf(exp, " <type=rectangularArray, dimensions=%d, ", dim);
                for (int j = 0; j < dim; j++)
                {
                    int a = t->exp_type.record.arr_record.dim_bound[j][0];
                    int b = t->exp_type.record.arr_record.dim_bound[j][1];
                    //case for var_name in d_bind
                    if (a != -1 && b != -1)
                        sprintf(exp2, "range_R%d= (%d, %d), ", j + 1, a, b);
                    else if (a == -1 && b != -1)
                        sprintf(exp2, "range_R%d= (%s, %d), ", j + 1, t->exp_type.record.arr_record.l_indexes[j], b);
                    else if (a != -1 && b == -1)
                        sprintf(exp2, "range_R%d= (%d, %s), ", j + 1, a, t->exp_type.record.arr_record.u_indexes[j]);
                    else if (a == -1 && b == -1)
                        sprintf(exp2, "range_R%d= (%s, %s), ", j + 1, t->exp_type.record.arr_record.l_indexes[j], t->exp_type.record.arr_record.u_indexes[j]);
                    strcat(exp, exp2);
                }
                sprintf(exp2, "basicElementType = Integer>");
                strcat(exp, exp2);
            }
            else if (t->exp_type.tag == jagged_array)
            { //Jagged_ARRAY
                int dim = t->exp_type.record.j_arr_record.dim;
                sprintf(exp, " <type =jaggedArray, dimensions=%d, ", dim);
                if (dim == 2)
                {
                    int high = t->exp_type.record.j_arr_record.r1_high;
                    int low = t->exp_type.record.j_arr_record.r1_low;
                    sprintf(exp2, "range_R1=(%d, %d) range_R2 = (", low, high);
                    strcat(exp, exp2);
                    for (int j = 0; j < high - low + 1; j++)
                    {
                        sprintf(exp2, " %d ", t->exp_type.record.j_arr_record.dim_bound._line[j]);
                        strcat(exp, exp2);
                        if (j != high - low)
                        {
                            sprintf(exp2, ",");
                            strcat(exp, exp2);
                        }
                        //from (low,high) gives size  , 0 index
                    }
                }
                else
                { //dim==3
                    int high = t->exp_type.record.j_arr_record.r1_high;
                    int low = t->exp_type.record.j_arr_record.r1_low;
                    sprintf(exp2, "range_R1=(%d, %d) range_R2 = ( ", low, high);
                    strcat(exp, exp2);
                    for (int j = 0; j < high - low + 1; j++)
                    {
                        int m = t->exp_type.record.j_arr_record.dim_bound.jag_line[j][0];
                        sprintf(exp2, "%d [", m);
                        strcat(exp, exp2);
                        for (int k = 0; k < m; k++)
                        {
                            sprintf(exp2, " %d", t->exp_type.record.j_arr_record.dim_bound.jag_line[j][k + 1]);
                            strcat(exp, exp2);
                            if (k != m - 1)
                            {
                                sprintf(exp2, ",");
                                strcat(exp, exp2);
                            }
                        }
                        sprintf(exp2, " ]");
                        strcat(exp, exp2);
                        if (j != high - low)
                        {
                            sprintf(exp2, ", ");
                            strcat(exp, exp2);
                        }
                    }
                }

                sprintf(exp2, "), basicElementType = integeri> %-10s", "ji");
                strcat(exp, exp2);

                //basic=Integer by def
            }
        }

        // print the tyeExpression
        fprintf(fp, "%-150s", exp);
        fprintf(fp, "%-35s", "NULL");
        fprintf(fp, "%-10s", "NULL");
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
/*********************UTILITY FUNCTION FOR TRAVERSING A PARSE TREE***************/
TypeExpression search_table(char *lex)
{
    TypeExpression x;
    x.tag = not_app;
    // search till index m for the typeExpression
    for (int i = 0; i < curr_table_entry; i++)
    {
        if (strcmp(lex, table[i].var_name) == 0) // found the variable
        {
            x.info = table[i].info;
            x.tag = table[i].tag;
            x.record = table[i].record;
            return x;
        }
    }
    // if not found
    //printf("ERROR!!! TYPE EXPRESSION NOT FOUND IN TABLE, %s NOT PRESENT\n", lex);
    return x;
}

int check_idx(parseTree *p_idx, TypeExpression var_exp, int arr_size, char * lex, int line_no)
{
    //<idx> => <num_id> <idx>
    //<idx> => EPSILON
    int count = 0;
    int prev_1 = -1, prev_2 = -1; // for jagged array
    // keep going forward in the indexes until I get an epsilon
    // keep going forward in the indexes until I get an epsilon
    //printf("%s\n", NonTerminalMap[p_idx->Node.nonTerminal.nt]);
    while (!p_idx->firstChild->isTerm) // encountered an epsilon then exit
    {
        //<num_id> => NUM
        //<num_id> => ID
        //printf("%s\n", NonTerminalMap[p_idx->firstChild->Node.nonTerminal.nt]);
        parseTree *p_num_id = p_idx->firstChild;
        count++;

        // do the bound checking and dimension checking
        if (count > arr_size)
        {
            // ERRROR dereferenced greater than the dimension.
            //printf("ERRROR dereferenced greater than the dimension.\n");
            char message[100];
            sprintf(message, "ERROR The variable is Dereferenced Greater than its Dimensions %d", arr_size);
            printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "[ ]", lex, NonTerminalMap[var_exp.tag], "***", "***", p_idx->depth, message);
            // print the error here
            p_idx->exp_type.tag = not_app;
            return 0;
        }
        // handle seperately if num or id
        if (p_num_id->firstChild->Node.terminal.t == ID) // id
        {
            // just check if the index is an integer
            TypeExpression id_exp = search_table(p_num_id->firstChild->Node.terminal.lexeme);
            if (id_exp.tag != primitive || id_exp.record.primitive_type != INTEGER)
            {
				char left[100];
				if(p_num_id->firstChild->exp_type.tag == primitive)
					strcpy(left, TerminalMap[p_num_id->firstChild->exp_type.record.primitive_type]);
				else
					strcpy(left, NonTerminalMap[p_num_id->firstChild->exp_type.tag]);
                //printf("ERROR... ARRAY Index needs to be a INTEGER\n");
                printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "[ ]", lex, NonTerminalMap[var_exp.tag], p_num_id->firstChild->Node.terminal.lexeme, left, p_idx->depth, "ERROR Array Index needs to be an Integer");
                p_num_id->exp_type.tag = not_app;
                return 0;
            }
            // copy the lexeme in the parent node
            p_num_id->exp_type = id_exp;
            strcpy(p_num_id->Node.nonTerminal.lex_nt, p_num_id->firstChild->Node.terminal.lexeme);
            // no bound checking needed here
        }
        else // NUM
        {
            int num = atoi(p_num_id->firstChild->Node.terminal.lexeme);
            // do bound checking here
            if (var_exp.info == Static || var_exp.tag == jagged_array)
            {
                if (var_exp.tag == jagged_array)
                {
                    int low, high;
                    if (count == 1)
                    {
                        low = var_exp.record.j_arr_record.r1_low;
                        high = var_exp.record.j_arr_record.r1_high;
                        prev_1 = num - low;
                        if (!(num >= low && num <= high))
                        {
                            //printf("ERROR... INDEX OUT OF BOUND JAGGED ARRAY in 1st dimension %d\n", num);
                            char message[100];
                             sprintf(message, "Jagged Array Index Out of Bounds at index 0, limit %d --- %d", low, high);
                             printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "[ ]", lex, NonTerminalMap[var_exp.tag], p_num_id->firstChild->Node.terminal.lexeme, "INTEGER", p_idx->depth, message);
                            return 0;
                        }
                    }
                    else if (count == 2)
                    {
                        low = 0;
                        high = var_exp.record.j_arr_record.dim_bound._line[prev_1] - 1;

                        if (!(num >= low && num <= high))
                        {
                            //printf("ERROR... INDEX OUT OF BOUND JAGGED ARRAY in 2nd dimension %d\n", num);
                             char message[100];
                             sprintf(message, "Jagged Array Index Out of Bounds at index 1, limit %d --- %d", low, high);
                             printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "[ ]", lex, NonTerminalMap[var_exp.tag], p_num_id->firstChild->Node.terminal.lexeme, "INTEGER", p_idx->depth, message);
                            return 0;
                        }
                        prev_2 = num;
                    }
                    else // count is 3
                    {
                        low = 0;
                        high = var_exp.record.j_arr_record.dim_bound.jag_line[prev_1][prev_2] - 1;
                        if (!(num >= low && num <= high))
                        {
                            //printf("ERROR... INDEX OUT OF BOUND JAGGED ARRAY in 3rd dimension %d\n", num);
                             char message[100];
                             sprintf(message, "Jagged Array Index Out of Bounds at index 2, limit %d --- %d", low, high);
                             printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "[ ]", lex, NonTerminalMap[var_exp.tag], p_num_id->firstChild->Node.terminal.lexeme, "INTEGER", p_idx->depth, message);
                            return 0;
                        }
                    }
                    strcpy(p_num_id->Node.nonTerminal.lex_nt, p_num_id->firstChild->Node.terminal.lexeme);
                    p_num_id->exp_type = p_num_id->firstChild->exp_type;
                }
                else
                {
                    int low, high;
                    low = var_exp.record.arr_record.dim_bound[count - 1][0];
                    high = var_exp.record.arr_record.dim_bound[count - 1][1];
                    if (!(num >= low && num <= high))
                    {
                        //printf("ERROR... INDEX OUT OF BOUND STATIC ARRAY at %d st dimension with %d. limit %d --  %d\n", count, num, low, high);
                         char message[100];
                             sprintf(message, "Array Index Out of Bounds at index %d, limit %d --- %d", count, low, high);
                             printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "[ ]", lex, NonTerminalMap[var_exp.tag], p_num_id->firstChild->Node.terminal.lexeme, "INTEGER", p_idx->depth, message);
                        p_num_id->exp_type.tag = not_app;
                        return 0;
                    }
                    else
                    {
                        strcpy(p_num_id->Node.nonTerminal.lex_nt, p_num_id->firstChild->Node.terminal.lexeme);
                        p_num_id->exp_type = p_num_id->firstChild->exp_type;
                    }
                }
            }
        }
        // update the node
        p_idx = p_idx->firstChild->sibling;
    }
    // encountered an epsilon
    if (count == arr_size)
        return 1;
    //printf("ERROR... ARRAY LESS INDICES %d SPECIFIED than dimensions %d\n", count, arr_size);
     char message[100];
	 sprintf(message,"ERROR Array less indices %d specified than dimensions %d\n", count + 1, arr_size);
     printf("%-5d %-15s %-5s %-15s %-15s %-15s %-15s %-5d %-50s\n", line_no, "ASSIGNMENT", "[ ]", lex, NonTerminalMap[var_exp.tag], "***", "***", p_idx->depth, message);
    return 0; // dereferenced for lesser dimensions
}
/*************************END OF UTIL*******************************************/
//*************************PRINTING A PARSE TREE****************************//

void printParseTree(parseTree *t)
{
    // Initial setup.
    FILE *fp = fopen("parseTree.txt", "w");
    if (fp == NULL)
    {
        printf("ERROR OPENING FILE TO PRINT PARSE TREE\n");
    }

    fprintf(fp, "\n------------------------------------------------------Printing Parse Tree-----------------------------------------------------\n\n");
    //printf("\n------------------------------------------------------Printing Parse Tree On the Console-----------------------------------------------\n\n");
    //fprintf(fp, "%-25s %-10s %-15s %-15s %-30s %-5s %s\n\n\n", "LEXEME","LINE","TOKEN","VALUE","PARENT","LEAF","NODE");
    fprintf(fp, "%-24s %-25s %-150s %-30s %-10s %-200s %-10s\n\n\n", "SYMBOL", "TERMINAL / NON TERMINAL", "TYPE EXPRESSION", "LEXEME", "LINE", "GRAMMAR RULE", "DEPTH");

    printParseTreeUtil(t, fp, 0);

    fprintf(fp, "\n------------------------------------------------------Parse Tree Printed On the FILE-----------------------------------------------\n\n");
    fclose(fp);
}
