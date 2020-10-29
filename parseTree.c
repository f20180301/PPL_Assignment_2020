/**
SHUBHAM AGARWAL -- 2018A7PS0310P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGARWAL RAJAT RAMESH -- 2018A7PS0182P
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
            int dep = 0;
            if (dimen == 2)
            {
                printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Index_Mismatch \n", jagList->Node.terminal.line_num, dep);
            }
            else
            {
                printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_Index_Mismatch \n", jagList->Node.terminal.line_num, dep);
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
                    int dep = 0;
                    printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_No_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                    expr.tag = not_app;
                    return expr;
                }
                if (dpnumList->sibling->firstChild->Node.terminal.t != EPSILON)
                {
                    int dep = 0;
                    printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Multiple_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                    expr.tag = not_app;
                    return expr;
                }
                numList = numList->sibling->sibling->firstChild; // Moving to next numList
            }
            if (numList->firstChild->Node.terminal.t == EPSILON)
            {
                int dep = 0;
                printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_No_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                expr.tag = not_app;
                return expr;
            }
            if (numList->firstChild->sibling->firstChild->Node.terminal.t != EPSILON)
            {
                int dep = 0;
                printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Multiple_Element_In_Row \n", dpnumList->Node.terminal.line_num, dep);
                expr.tag = not_app;
                return expr;
                // 3d type
            }
            numcount++;
            if (sz != numcount) // Matchng size with no. of elements counted
            {
                int dep = 0;
                printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   2D_Jagged_Array_Size_Mismatch \n", dpnumList->Node.terminal.line_num, dep);
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
                    int dep = 0;
                    printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_No_Element_In_3rd_Dimension \n", dpnumList->Node.terminal.line_num, dep);
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
                int dep = 0;
                printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_No_Element_In_3rd_Dimension \n", dpnumList->Node.terminal.line_num, dep);
                expr.tag = not_app;
                return expr;
            }
            counter++;
            jagl[szchk - lo - 1][counter] = idxcount;
            if (counter != sz)
            {
                int dep = 0;
                printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   3D_Jagged_Array_Size_Mismatch \n", dpnumList->Node.terminal.line_num, dep);
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

                    int dep = 0;
                    printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Array_Range_Mismatch \n", line, dep);
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
                    int dep = 0;
                    printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Jagged_Array_Range_Mismatch \n", temp->firstChild->sibling->Node.terminal.line_num, dep);
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
                    int dep = 0;
                    printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Jagged_Array_Range_Mismatch \n", temp->firstChild->sibling->Node.terminal.line_num, dep);
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
        traverse_parse_tree(t->firstChild);
        if (t->firstChild->sibling != NULL)
        {
            traverse_parse_tree(t->firstChild->sibling);
        }
        return;
    }

    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == assignment_stmt)
    { //<assignment_stmt>
        // //=> <arr_pid> EQUALS <expression> SEMICOLON
        // type_left;type_right;
        // //CONTINUE THIS WORK//CONTINUE THIS WORK//CONTINUE THIS WORK
        // t=t->firstChild;(check validity)//-->boundck, etc if array type var                   int a = 2 + 3.7 + 5;
        // t=t->sibling->sibling;//move to expree(send to function, find return type) -- , + , &&
        // if(type_left!=type_right){
        //    // ERROR //otherwise no isseus
        // }
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
        fprintf(fp, "%-300s", "NULL");
        fprintf(fp, "%-35s", t->Node.terminal.lexeme);
        fprintf(fp, "%-10d", t->Node.terminal.line_num);
        fprintf(fp, "%-250s", "NULL");
        fprintf(fp, "%-10d\n", depth);
    }

    else
    {
        char *ter = "NON TERMINAL";
        fprintf(fp, "%-25s", NonTerminalMap[t->Node.nonTerminal.nt]); // print the symbol
        fprintf(fp, "%-25s", ter);
        if (t->exp_type.tag == not_app)
        {
            fprintf(fp, "%-300s", "--Ignored--");
        }
        else
        {
            if (t->exp_type.tag == primitive)
            { //Primitive
                fprintf(fp, "<basicType=%s>%-200s", TerminalMap[t->exp_type.record.primitive_type], "");
            }
            else if (t->exp_type.tag == array)
            {
                int dim = t->exp_type.record.arr_record.dim;
                fprintf(fp, " <type=rectangularArray, dimensions=%d, ", dim);

                for (int j = 0; j < dim; j++)
                {
                    int a = t->exp_type.record.arr_record.dim_bound[j][0];
                    int b = t->exp_type.record.arr_record.dim_bound[j][1];
                    //case for var_name in d_bind
                    if (a != -1 && b != -1)
                        fprintf(fp, "range_R%d= (%d, %d), ", j + 1, a, b);
                    else if (a == -1 && b != -1)
                        fprintf(fp, "range_R%d= (%s, %d), ", j + 1, t->exp_type.record.arr_record.l_indexes[j], b);
                    else if (a != -1 && b == -1)
                        fprintf(fp, "range_R%d= (%d, %s), ", j + 1, a, t->exp_type.record.arr_record.u_indexes[j]);
                    else if (a == -1 && b == -1)
                        fprintf(fp, "range_R%d= (%s, %s), ", j + 1, t->exp_type.record.arr_record.l_indexes[j], t->exp_type.record.arr_record.u_indexes[j]);
                }
                fprintf(fp, "basicElementType = Integer>%-110s", "");
            }
            else if (t->exp_type.tag == jagged_array)
            { //Jagged_ARRAY
                int dim = t->exp_type.record.j_arr_record.dim;

                fprintf(fp, " <type =jaggedArray, dimensions=%d, ", dim);

                if (dim == 2)
                {
                    int high = t->exp_type.record.j_arr_record.r1_high;
                    int low = t->exp_type.record.j_arr_record.r1_low;
                    fprintf(fp, "range_R1=(%d, %d) range_R2 = (", low, high);

                    for (int j = 0; j < high - low + 1; j++)
                    {
                        fprintf(fp, " %d ", t->exp_type.record.j_arr_record.dim_bound._line[j]);
                        if (j != high - low)
                            fprintf(fp, ",");
                        //from (low,high) gives size  , 0 index
                    }
                }
                else
                { //dim==3
                    int high = t->exp_type.record.j_arr_record.r1_high;
                    int low = t->exp_type.record.j_arr_record.r1_low;
                    fprintf(fp, "range_R1=(%d, %d) range_R2 = ( ", low, high);
                    for (int j = 0; j < high - low + 1; j++)
                    {
                        int m = t->exp_type.record.j_arr_record.dim_bound.jag_line[j][0];
                        fprintf(fp, "%d [", m);
                        for (int k = 0; k < m; k++)
                        {
                            fprintf(fp, " %d", t->exp_type.record.j_arr_record.dim_bound.jag_line[j][k + 1]);
                            if (k != m - 1)
                                fprintf(fp, ",");
                        }
                        fprintf(fp, " ]");
                        if (j != high - low)
                            fprintf(fp, ", ");
                    }
                }
                fprintf(fp, "), basicElementType = integeri> %-10s", "ji");

                //basic=Integer by def
            }
        }

        // print the tyeExpression

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
    fprintf(fp, "%-24s %-25s %-300s %-30s %-10s %-200s %-10s\n\n\n", "SYMBOL", "TERMINAL / NON TERMINAL", "TYPE EXPRESSION", "LEXEME", "LINE", "GRAMMAR RULE", "DEPTH");

    printParseTreeUtil(t, fp, 0);

    fprintf(fp, "\n------------------------------------------------------Parse Tree Printed On the FILE-----------------------------------------------\n\n");
    fclose(fp);
}
