/**
SHUBHAM AGARWAL -- 2018A7PS0310P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGARWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#ifndef _TYPEEXPRESSION_
#define _TYPEEXPRESSION_
int CAPACITY = 0;
int curr_table_entry = 0;

typedef enum
{
    Static,
    Dynamic,
    N_A
} bind_info;

char string_bind[3][25] =
    {
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
    char **l_indexes;
    char **u_indexes;
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

TypeExpressionTable *table = NULL;

//********************FUNCTION DECLARATIONS*******************************//
void printTypeExpressionsTable(TypeExpressionTable *, char *);

#endif