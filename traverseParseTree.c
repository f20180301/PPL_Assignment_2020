#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*************************TOKEN STRUCT IMPORTS******************************/
#define MAX 100

char *ha[13][2];  
char *op[16][2]; //FOR KEYWORD


typedef struct token{       //TOKEN STRUCTURE
    char lexeme[200];
    char token[200];
    int lineno;
    struct token* next;
}token;

token * head=NULL;
token * tail=NULL;      //Head, Tail of LinkedList for TokenStream

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
        "TRE",
        "FAL",
        "SIZE",
        "VALUES",
        "AND",
        "OR",
        "EPSILON"
        };
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
        "array_dim",
        "jagged_array",
        "jagged_2d_array",
        "jagged_3d_array",
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
        "fact_bool"};

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
    TRE,
    FAL,
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
    array_dim,
    jagged_array,
    jagged_2d_array,
    jagged_3d_array,
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
    fact_bool
} NonTerminal;


typedef union SYMBOL {
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

typedef enum{
    Static,
    Dynamic,
    N_A
}bind_info;

char string_bind[3][25] = {
                        "Static",
                        "Dynamic",
                        "NotApplicable"
                     };



typedef union {
    int *_line;
    int **jag_line;//jagged
}jagged_type;

typedef struct {
    int dim;//--4
    int ** dim_bound;//(7,4),(3,4),(),() 4X2
}array_record;

typedef struct  {
    int dim;//2d or 3d ke liye tagg--
    int r1_low;
    int r1_high;    
    jagged_type dim_bound;

}jagged_array_record;

typedef union type_Expression_record{
     Terminal primitive_type;//INT<BOOL<REAL
     array_record arr_record;
     jagged_array_record j_arr_record;
} type_Expression_record;

typedef struct TypeExpression{
    NonTerminal tag;///primitive,array,jagged
    bind_info info;//static dyanmic
    type_Expression_record record;
}TypeExpression;

typedef struct TypeExpressionTable{
    char var_name[200];     //extra
    NonTerminal tag;
    bind_info info;
    type_Expression_record record;
}TypeExpressionTable;

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
	grules * expRule; // for getting the rule used in parsing
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
	int isTerm; // tag for term/non_term and leaf / non_leaf
	NodeType Node;	// Details corresponding to a Terminal or a NonTerminal Node
	
	//TypeExpression t; // to be done
	// only for NonTerminal or Both ??
	
	struct treeNode * parent; // parent of the node
	struct treeNode * sibling; // right sibling of the node
	
	//for linkedlist of children
	struct treeNode* firstChild;
	TypeExpression exp_type;
} parseTree;
            /********PARSE TREE NODE*********/
int CAPACITY=1;
int m=0;
TypeExpressionTable * append_to_table(TypeExpressionTable * table,TypeExpression type,char * var){
    if(CAPACITY==0){
        table=(TypeExpressionTable *)malloc(sizeof(TypeExpressionTable)*10);
        CAPACITY=10;
    }
    else if(m==CAPACITY){
        table=(TypeExpressionTable *)realloc(table,sizeof(TypeExpressionTable)*CAPACITY*2);
        CAPACITY=CAPACITY*2;
    }
    else{
       
        strcpy(table[m].var_name,var);
        table[m].tag=type.tag;
        table[m].info=type.info;
        table[m].record=type.record;
       
        m++;
    }
    return table;
}

void traverse_parse_tree(parseTree *t,TypeExpressionTable * table){
    if(t->isTerm==0&&t->Node.nonTerminal.nt==s){
        t=t->firstChild->sibling->sibling->sibling->sibling;   
        traverse_parse_tree(t,table);
        //pointer t is pointing/moving to <start>
        return;
    }
     //pointer t is pointing/moving to <start>
    else if(t->isTerm==0&&t->Node.nonTerminal.nt==start){
        traverse_parse_tree(t->firstChild,table);
        traverse_parse_tree(t->firstChild->sibling,table);
        return;
    }
    
    else if(t->isTerm==0&&t->Node.nonTerminal.nt==declaration){
        traverse_parse_tree(t->firstChild,table); //to dec_stmnt
        if(t->firstChild->sibling!=NULL){
        traverse_parse_tree(t->firstChild->sibling,table);//to declarations or null
        }
        return;
   }
   else if(t->isTerm==0&&t->Node.nonTerminal.nt==declaration_stmt){
       //build for the "typeExpression" of this declarartion statement, error reporting
       TypeExpression exp_table_record;
        parseTree* varlist_pointer=NULL; 
       parseTree * temp=t;    //just being safe, making copy
       temp=temp->firstChild;

       if(temp->isTerm==0&&temp->Node.nonTerminal.nt==primitive){
           exp_table_record.tag=primitive;          //primitive/jagged_array/array//terminal
           exp_table_record.info=N_A;
           temp=temp->firstChild;   //pointing to var_list  //moving into single/multiple_primitive node //populate type expressions
           if(temp->isTerm==0&&temp->Node.nonTerminal.nt==single_primitive){
               varlist_pointer=temp=temp->firstChild->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           else{//multiple
               varlist_pointer=temp=temp->firstChild->sibling->sibling->sibling->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           //
           exp_table_record.record.primitive_type=temp->firstChild->Node.terminal.t; //INT/BOOl/REAL(terminal)
           //daalnaa
           //storing to <single_primitive> and <primitive>
           temp->parent->exp_type=exp_table_record;
           temp->parent->parent->exp_type=exp_table_record;
         }

       else if(temp->isTerm==0&&temp->Node.nonTerminal.nt==array){
           exp_table_record.tag=array;
           temp=temp->firstChild; 
            if(temp->isTerm==0&&temp->Node.nonTerminal.nt==single_array){
               varlist_pointer=temp=temp->firstChild->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           else{
               varlist_pointer=temp=temp->firstChild->sibling->sibling->sibling->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
            temp=temp->sibling;//(ignore just after colon, arrray written)
            //<array_dim> hai aab, in temp
            parseTree * store=temp;//at <array_dim>
            
            do{
                temp=temp->firstChild ;//(moving into the child of  <array_dim>)
                temp=temp->sibling;
                ////id or num =temp->firstChild;
                temp=temp->sibling->sibling;
               //// id or num =temp->firstChild; 
                temp=temp->sibling->sibling;//reaches <array_dim>
                //chk type(dynamic), dimension(lower<upper), read, and error report, maintain counter for Dimension Count, dimension=count;
                //store to type_Expression_record.record.arr_record;
            }while(temp!=NULL);
            //storing to sinle_array and array
            store->parent->exp_type=exp_table_record;
            store->parent->parent->exp_type=exp_table_record;
//temp2=daalde
       }
       //while exiting put in node

       else if(temp->isTerm==0&&temp->Node.nonTerminal.nt==jagged_array){
           temp=temp->firstChild;
           if(temp->isTerm==0&&temp->Node.nonTerminal.nt==jagged_2d_array){
           exp_table_record.tag=jagged_array;
           exp_table_record.info=N_A;
           temp=temp->firstChild; 
                if(temp->isTerm==0&&temp->Node.nonTerminal.nt==single_jagged2darray){
                    varlist_pointer=temp=temp->firstChild->sibling;
                    temp=temp->sibling->sibling;//moving into colon ke baad wala node
                }
                else{
                    varlist_pointer=temp=temp->firstChild->sibling->sibling->sibling->sibling;
                    temp=temp->sibling->sibling;//moving into colon ke baad wala node
                }
           temp=temp->sibling->sibling;//(ignore just after colon, jagged and arrray written)
            //<jagged_2d_dim> hai , in temp
            parseTree * store=temp;//at <jagged_2d/3d_dim>
            int l_bound=temp->firstChild->sibling->firstChild;
            int u_bound=temp->firstChild->sibling->sibling->sibling->firstChild;
            int dimension=2;
            //temp <jagged_dim> pe hi hai
            if(l_bound<=u_bound){//chk =
                //traverse for jaglines
                temp=temp->sibling->sibling->sibling->sibling; //reach <jag_lines>
                //read all jag_lines one by one upto, NULL
                do{
                //temp pointing on <jag_line>
                temp=temp->firstChild;
                parseTree* temp_jag=temp->firstChild;  //point to R1             
                temp_jag=temp_jag->sibling->sibling;//pointing on NUM
                int indx=temp_jag;
                //chk index in l_bound,u_bound and particularly increasing//#######TO BE DONE
                temp_jag=temp_jag->sibling->sibling->sibling->sibling;//pointing to size NUM(size NUM)
                int sz=temp_jag;
                //store in the cnt of jagged_R2_type
                temp_jag=temp_jag->sibling->sibling->sibling->sibling;//pointing to jag_list;
                //read all <jag_list(initialisation wala values)>
                //validity CHK ACCORDING TO SZ variable me size kya ha
                //epsion empty ; ; checking required


                //moving temp to <jag_lines>
                temp=temp->sibling;
                }while(temp!=NULL);

            }
            else{
                //error
            }
            store->parent->exp_type=exp_table_record;
            store->parent->parent->exp_type=exp_table_record;
            store->parent->parent->parent->exp_type=exp_table_record;
            
          // exp_table_record.record.j_arr_record=temp->child->value;

       }
       else if(temp->isTerm==0&&temp->Node.nonTerminal.nt==jagged_3d_array){
           exp_table_record.tag=jagged_array;
           exp_table_record.info=N_A;
           temp=temp->firstChild; 
           if(temp->isTerm==0&&temp->Node.nonTerminal.nt==single_jagged3darray){
               varlist_pointer=temp=temp->firstChild->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           else{
               varlist_pointer=temp=temp->firstChild->sibling->sibling->sibling->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           temp=temp->sibling->sibling;//(ignore just after colon, jagged and arrray written)
            //<jagged_dim> hai , in temp
             parseTree * store=temp;//at <jagged_2d/3d_dim>
        //CONTINUE THIS WORK//CONTINUE THIS WORK//CONTINUE THIS WORK
            int l_bound=temp->firstChild->sibling->firstChild;
            int u_bound=temp->firstChild->sibling->sibling->sibling->firstChild;
            int dimension=3;
            if(l_bound<=u_bound){//chk =
                //kamm alag hoga.
            }
            else{
                //error
            }
            store->parent->exp_type=exp_table_record;
            store->parent->parent->exp_type=exp_table_record;
            store->parent->parent->parent->exp_type=exp_table_record;
        //   exp_table_record.record.j_arr_record=temp->firstChild;
       }
       }
       //reach, type expression record ready,
       //populate varlist_pointer, with typeexpression,
       //populate typeExpressionTable with variable created.
       if(varlist_pointer->isTerm==1&&varlist_pointer->Node.terminal.t==ID){
            varlist_pointer->exp_type=exp_table_record;
            //append to typeExpression Table
            table=append_to_table(table,exp_table_record,varlist_pointer->Node.terminal.lexeme);
       }
       else{
           //traverse all IDs
           do{
               varlist_pointer->exp_type=exp_table_record;
               varlist_pointer=varlist_pointer->firstChild;
               varlist_pointer->exp_type=exp_table_record;
//append to typeExpression Table
               table=append_to_table(table,exp_table_record,varlist_pointer->Node.terminal.lexeme);
               varlist_pointer=varlist_pointer->sibling;
           }while(varlist_pointer!=NULL);
       }
       return;
   }

   
   
   
   /*********///RAJAT/************/
   
   
    else if(t->isTerm==0&&t->Node.nonTerminal.nt==assignment){
        traverse_parse_tree(t->firstChild,table);
        if(t->firstChild->sibling!=NULL){
        traverse_parse_tree(t->firstChild->sibling,table);
        }
        return;
}

else if(t->isTerm==0&&t->Node.nonTerminal.nt==assignment_stmt){//<assignment_stmt> 
//=> <arr_pid> EQUALS <expression> SEMICOLON
type_left;type_right;
//CONTINUE THIS WORK//CONTINUE THIS WORK//CONTINUE THIS WORK
t=t->firstChild;(check validity)//-->boundck, etc if array type var                   int a = 2 + 3.7 + 5;
t=t->sibling->sibling;//move to expree(send to function, find return type) -- , + , &&
if(type_left!=type_right){
   // ERROR //otherwise no isseus
}
return;
}
}

int main(){

    return 0;
}