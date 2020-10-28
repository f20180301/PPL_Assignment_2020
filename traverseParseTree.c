#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

/**********************GRAMMAR STRUCT IMPORTS************************/
#define TOTAL_TERMINALS 34
#define TOTAL_NON_TERMINALS 34
#define NON_TERMINALS 34
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

/***************TYPE EXPRESSION TABLE AND STRUCTS**********************/

// typedef enum  {
//    INTEGER,
//    REAL,
//    BOOLEAN,
//    TYPE_ERROR
// }primitive;

// typedef enum  {
//    PRIMITIVE,
//    ARRAY,
//    J_ARRAY,
//    NA
// }declaration_type;

typedef enum
{
    Static,
    Dynamic,
    N_A
} bind_info;

char string_bind[3][25] = {
    "Static",
    "Dynamic",
    "NotApplicable"};

typedef union
{
    int *_line;
    int **jag_line; //jagged
} jagged_type;

typedef struct
{
    int dim;         //--4
    int **dim_bound; //(7,4),(3,4),(),() 4X2
} array_record;

typedef struct
{
    int dim; //2d or 3d ke liye tagg--
    int r1_low;
    int r1_high;
    jagged_type dim_bound;

} jagged_array_record;

typedef union type_Expression_record
{
    Terminal primitive_type; //INT<BOOL<REAL
    array_record arr_record;
    jagged_array_record j_arr_record;
} type_Expression_record;

typedef struct TypeExpression
{
    NonTerminal tag; ///primitive,array,jagged
    bind_info info;  //static dyanmic
    type_Expression_record record;
} TypeExpression;

typedef struct TypeExpressionTable
{
    char var_name[200]; //extra
    NonTerminal tag;
    bind_info info;
    type_Expression_record record;
} TypeExpressionTable;

/***************TYPE EXPRESSION TABLE AND STRUCTS**********************/

/********PARSE TREE NODE*********/
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
    TypeExpression exp_type;
} parseTree;
/********PARSE TREE NODE*********/
int CAPACITY = 1;
int m = 0;

TypeExpression jagged(int lo, int hi, parseTree *jagLines, int dimen, TypeExpression expr)
{
    expr.record.j_arr_record.dim = dimen;
    expr.record.j_arr_record.r1_high = hi;
    expr.record.j_arr_record.r1_low = lo;
    int indx = 0;     // To RECORD INDEX
    int szchk = lo;   // TO CHECK INDEX
    int sz = 0;       // TO RECORD SIZE IN EACH JAGLINE
    int numcount = 0; // TO TRACK SIZE
    int idxcount = 0; // For Jagged_3d array to track no. of elemets in 3rd dimension
    int *line = NULL;
    int **jagl = NULL;
    parseTree *dpnumList;
    if (dimen == 2)
    {
        line = (int *)malloc(sizeof(int) * (hi - lo + 1));
    }
    else
    {
        jagl = (int **)malloc(sizeof(int *) * (hi - lo + 1));
    }

    parseTree *jagLine = jagLines->firstChild; //Being Safe, Creating a copy
    do
    {
        parseTree *jagList = jagLine->firstChild->sibling->sibling; //Pointing to Num
        indx = atoi(jagList->Node.terminal.lexeme);
        if (szchk != indx) // Checking index value
        {
            //error IF R1[VALUE] IS CORRECT OR NOT
            break;
        }
        szchk++;
        jagList = jagList->sibling->sibling->sibling;          // Pointing to next Num sixe(NUM)
        sz = atoi(jagList->Node.terminal.lexeme);              //Receiving the size of the array
        jagList = jagList->sibling->sibling->sibling->sibling; // Pointing to JagList
        parseTree *numList = jagList->firstChild;              // Pointing to a numlist of rule <jag_list> => <num_list> SEMICOLON <jag_list> ||  <num_list>
        if (dimen == 2)
        {
            while (numList->sibling != NULL)
            {
                dpnumList = numList->firstChild;                                     // Pointing to NUM of rule <num_list> => NUM <num_list> || NUM
                if (dpnumList->Node.terminal.t == NUM && dpnumList->sibling == NULL) // Not more than two in a numList and single NUM to be present b/w semicolons
                {
                    numcount++;
                }
                else if (dpnumList->sibling != NULL)
                {
                    // Like 3d array
                    //error
                }
                else if (dpnumList->Node.terminal.t == EPSILON)
                {
                    //error Empty
                }
                numList = numList->sibling->sibling->firstChild;
            }
            if (numList->firstChild->Node.terminal.t == EPSILON)
            {
                //error Last element is EPSILON
            }
            numcount++;
            if (sz != numcount) // Matchng size with no. of elements counted
            {
                //error No. of elements do not match size
            }
            line[szchk - lo - 1] = sz;
        }
        else
        {
            int counter = 1;
            jagl[lo - szchk - 1] = (int *)malloc(sizeof(int) * (sz + 1));
            jagl[lo - szchk - 1][0] = sz;
            while (numList->sibling != NULL) // Pointing to a numlist of rule <jag_list> => <num_list> SEMICOLON <jag_list> ||  <num_list>
            {
                idxcount = 0;
                dpnumList = numList->firstChild;   // Pointing to NUM of rule <num_list> => NUM <num_list> || NUM
                while (dpnumList->sibling != NULL) // TRAVERSING THE RULE to eat all NUM in <num_list>
                {
                    idxcount++;
                    dpnumList = dpnumList->sibling->firstChild;
                }
                if (dpnumList->Node.terminal.t == EPSILON)
                {
                    // error EPSILON ENCOUNTERED
                }
                idxcount++;
                jagl[lo - szchk - 1][counter] = idxcount;
                counter++;
                numList = numList->sibling->sibling->firstChild;
            }
            dpnumList = numList->firstChild;
            idxcount = 0;
            while (dpnumList->sibling != NULL) // TRAVERSING THE RULE to eat all NUM in <num_list>
            {
                idxcount++;
                dpnumList = dpnumList->sibling->firstChild;
            }
            if (dpnumList->Node.terminal.t == EPSILON)
            {
                // error EPSILON ENCOUNTERED
            }
            idxcount++;
            jagl[lo - szchk - 1][counter] = idxcount;
            if (counter != sz)
            {
                //error Size don't match
            }
        }
        jagLine = jagLine->sibling; // Move to next Jagline after Traversing a single Jagline
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

TypeExpressionTable *append_to_table(TypeExpressionTable *table, TypeExpression type, char *var)
{
    if (CAPACITY == 0)
    {
        table = (TypeExpressionTable *)malloc(sizeof(TypeExpressionTable) * 10);
        CAPACITY = 10;
    }
    else if (m == CAPACITY)
    {
        table = (TypeExpressionTable *)realloc(table, sizeof(TypeExpressionTable) * CAPACITY * 2);
        CAPACITY = CAPACITY * 2;
    }
    else
    {

        strcpy(table[m].var_name, var);
        table[m].tag = type.tag;
        table[m].info = type.info;
        table[m].record = type.record;

        m++;
    }
    return table;
}

int strCount = 0;
char **indexes;

void traverse_parse_tree(parseTree *t, TypeExpressionTable *table)
{

    if (t->isTerm == 0 && t->Node.nonTerminal.nt == s)
    {
        t = t->firstChild->sibling->sibling->sibling->sibling;
        traverse_parse_tree(t, table);
        //pointer t is pointing/moving to <start>
        return;
    }
    //pointer t is pointing/moving to <start>
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == start)
    {
        traverse_parse_tree(t->firstChild, table);
        traverse_parse_tree(t->firstChild->sibling, table);
        return;
    }

    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == declaration)
    {
        traverse_parse_tree(t->firstChild, table); //to dec_stmnt
        if (t->firstChild->sibling != NULL)
        {
            traverse_parse_tree(t->firstChild->sibling, table); //to declarations or null
        }
        return;
    }
    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == declaration_stmt)
    {
        //build for the "typeExpression" of this declarartion statement, error reporting
        TypeExpression exp_table_record;
        parseTree *varlist_pointer = NULL;
        parseTree *temp = t;     //just being safe, making copy
        temp = temp->firstChild; //moving to primitive/array/jagged_array

        //*********PRIMITIVE***************//

        if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == primitive)
        {
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
            //daalnaa
            //storing to <single_primitive> and <primitive>
            temp->parent->exp_type = exp_table_record;
            temp->parent->parent->exp_type = exp_table_record;
        }

        //**********ARRAY*********************//

        else if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == array)
        {
            exp_table_record.tag = array;
            exp_table_record.info = Static;
            temp = temp->firstChild; // multi_array or single_array
            if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == single_array)
            {
                varlist_pointer = temp = temp->firstChild->sibling;
                temp = temp->sibling->sibling; //moving into colon ke baad wala node
            }
            else
            {
                varlist_pointer = temp = temp->firstChild->sibling->sibling->sibling->sibling;
                temp = temp->sibling->sibling; //moving into colon ke baad wala node
            }
            temp = temp->sibling; //(ignore just after colon, arrray written)
            //<array_dim> hai aab, in temp
            parseTree *store = temp; //at <array_dim>

            int count = 0;
            int r1 = -1, r2 = -1;
            int **tem = (int **)malloc(2 * sizeof(int *));
            // int strCount=0;
            // char **indexes;
            int g = 1, b = 2;
            do
            {
                temp = temp->firstChild; //(moving into the child of  <array_dim>)
                temp = temp->sibling;
                //id or num =temp->firstChild;

                if (temp->firstChild->Node.terminal.t == ID) //checking if dimension is ID.
                {
                    exp_table_record.info = Dynamic;
                    indexes = (char **)malloc(sizeof(char *));
                    indexes[strcount] = temp->firstChild; // Storing ID in form of string
                    strcount++;
                    r1 = -1;
                }
                else // If dimension is NUM
                    r1 = temp->firstChild;

                temp = temp->sibling->sibling; //ignoring .. (DD)

                if (temp->firstChild->Node.terminal.t == "ID") // checking if dimension is ID
                {
                    exp_table_record.info = Dynamic;
                    indexes = (char **)malloc(sizeof(char *));
                    indexes[strcount] = temp->firstChild; // Storing ID in form of string
                    strcount++;
                    r2 = -1;
                }
                else // If dimension is NUM
                    r2 = temp->firstChild;
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
                }
                else //if lower_range becomes greater than higher_range
                {
                    exp_table_record.info = N_A;
                    int line = temp->Node.terminal.line_number;
                    int dep = ? ;
                    printf("%d   DECLARATION   ***   ***   ***   ***   ***   %d   Array_Range_Mismatch \n", line, dep);
                    break;
                }

            } while (temp != NULL);
            //storing to single_array and array
            store->parent->exp_type = exp_table_record;
            store->parent->parent->exp_type = exp_table_record;
            //temp2=daalde
        }
        //while exiting put in node

        //************JAGGED ARRAY***********************//

        else if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == jagged_array)
        {
            temp = temp->firstChild; //jagged_2d or jagged_3d
            if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == jagged_2d_array)
            {
                exp_table_record.tag = jagged_array;
                exp_table_record.info = N_A;
                temp = temp->firstChild; //single_jagged_2d or multi_jagged_2d
                if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == single_jagged_2d_array)
                {
                    varlist_pointer = temp = temp->firstChild->sibling;
                    temp = temp->sibling->sibling; //moving into colon ke baad wala node
                }
                else
                {
                    varlist_pointer = temp = temp->firstChild->sibling->sibling->sibling->sibling;
                    temp = temp->sibling->sibling; //moving into colon ke baad wala node
                }
                temp = temp->sibling->sibling; //(ignore just after colon, jagged and arrray written)
                //<jagged_2d_dim> hai , in temp
                parseTree *store = temp;                 //at <jagged_2d_dim>
                int l_bound = temp->firstChild->sibling; //at NUM
                int u_bound = temp->firstChild->sibling->sibling->sibling;
                int dimension = 2;
                //temp <jagged_dim> pe hi hai
                if (l_bound <= u_bound)
                { //chk =
                    //traverse for jaglines
                    temp = temp->sibling->sibling->sibling->sibling; //reach <jag_lines>
                    exp_table_record = jagged(l_bound, u_bound, temp, 2, exp_table_record);

                    //read all jag_lines one by one upto, NULL
                    // do
                    // {
                    //     temp = temp->firstChild;                //temp pointing on <jag_line>
                    //     parseTree *temp_jag = temp->firstChild; //point to R1
                    //     temp_jag = temp_jag->sibling->sibling;  //pointing on NUM
                    //     int indx = atoi(temp_jag->Node.terminal.lexeme);
                    //     if (idx != szchk)
                    //     {
                    //         //error
                    //         break;
                    //     }
                    //     szchk++;
                    //     //chk index in l_bound,u_bound and particularly increasing//#######TO BE DONE
                    //     temp_jag = temp_jag->sibling->sibling->sibling->sibling; //pointing to size NUM(size NUM)
                    //     int sz = atoi(temp_jag->Node.terminal.lexeme);           //Recording the no. of elements in this jag_line
                    //     //store in the cnt of jagged_R2_type
                    //     temp_jag = temp_jag->sibling->sibling->sibling->sibling; //pointing to jag_list;
                    //     temp_jag = temp_jag->firstChild;                         //pointing to num list
                    //     parseTree *num_temp_jag = temp_jag;                      //pointed to num list
                    //     do
                    //     {
                    //         num_temp_jag = num_temp_jag->firstChild; // Pointing to NUM or EPSILON
                    //         if (num_temp_jag->isTerm == 1 && num_temp_jag->Node.terminal.t == EPSILON)
                    //         {
                    //             //error
                    //             break;
                    //         }
                    //         else if (num_temp_jag->isTerm == 1 && num_temp_jag->Node.terminal.t == NUM)

                    //     } while (num_temp != NULL); // Reading a num_list

                    //     //read all <jag_list(initialisation wala values)>
                    //     //validity CHK ACCORDING TO SZ variable me size kya ha
                    //     //epsion empty ; ; checking required

                    //     //moving temp to <jag_lines>
                    //     temp = temp->sibling;
                    // } while (temp != NULL);
                }
                else
                {
                    //error
                }
                store->parent->exp_type = exp_table_record;
                store->parent->parent->exp_type = exp_table_record;
                store->parent->parent->parent->exp_type = exp_table_record;

                // exp_table_record.record.j_arr_record=temp->child->value;
            }
            else if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == jagged_3d_array)
            {
                exp_table_record.tag = jagged_array;
                exp_table_record.info = N_A;
                temp = temp->firstChild;
                if (temp->isTerm == 0 && temp->Node.nonTerminal.nt == single_jagged_3d_array)
                {
                    varlist_pointer = temp = temp->firstChild->sibling;
                    temp = temp->sibling->sibling; //moving into colon ke baad wala node
                }
                else
                {
                    varlist_pointer = temp = temp->firstChild->sibling->sibling->sibling->sibling;
                    temp = temp->sibling->sibling; //moving into colon ke baad wala node
                }
                temp = temp->sibling->sibling; //(ignore just after colon, jagged and arrray written)
                                               //<jagged_dim> hai , in temp
                parseTree *store = temp;       //at <jagged_2d/3d_dim>
                                               //CONTINUE THIS WORK//CONTINUE THIS WORK//CONTINUE THIS WORK
                int l_bound = temp->firstChild->sibling->firstChild;
                int u_bound = temp->firstChild->sibling->sibling->sibling->firstChild;
                int dimension = 3;
                if (l_bound <= u_bound)
                { //chk =
                    //kamm alag hoga.
                    exp_table_record = jagged(l_bound, u_bound, temp, 3, exp_table_record);
                }
                else
                {
                    //error
                }
                store->parent->exp_type = exp_table_record;
                store->parent->parent->exp_type = exp_table_record;
                store->parent->parent->parent->exp_type = exp_table_record;
                //   exp_table_record.record.j_arr_record=temp->firstChild;
            }
        }
        //reach, type expression record ready,
        //populate varlist_pointer, with typeexpression,
        //populate typeExpressionTable with variable created.
        if (varlist_pointer->isTerm == 1 && varlist_pointer->Node.terminal.t == ID)
        {
            varlist_pointer->exp_type = exp_table_record;
            //append to typeExpression Table
            table = append_to_table(table, exp_table_record, varlist_pointer->Node.terminal.lexeme);
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
                table = append_to_table(table, exp_table_record, varlist_pointer->Node.terminal.lexeme);
                varlist_pointer = varlist_pointer->sibling;
            } while (varlist_pointer != NULL);
        }
        return;
    }

    /*********/ //RAJAT/************/

    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == assignment)
    {
        traverse_parse_tree(t->firstChild, table);
        if (t->firstChild->sibling != NULL)
        {
            traverse_parse_tree(t->firstChild->sibling, table);
        }
        return;
    }

    else if (t->isTerm == 0 && t->Node.nonTerminal.nt == assignment_stmt)
    { //<assignment_stmt>
        //=> <arr_pid> EQUALS <expression> SEMICOLON
        type_left;
        type_right;
        //CONTINUE THIS WORK//CONTINUE THIS WORK//CONTINUE THIS WORK
        t = t->firstChild;
        (check validity)             //-->boundck, etc if array type var                   int a = 2 + 3.7 + 5;
            t = t->sibling->sibling; //move to expree(send to function, find return type) -- , + , &&
        if (type_left != type_right)
        {
            // ERROR //otherwise no isseus
        }
        return;
    }
}

int main()
{

    return 0;
}
