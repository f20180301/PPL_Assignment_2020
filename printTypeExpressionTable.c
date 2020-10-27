#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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

// typedef enum  {
//    INTEGER,
//    REAL,
//    BOOLEAN, 
//    TYPE_ERROR
// }primitive;
// char string_primitive[3][10] = {
//                          "Integer",
//                          "Real",
//                          "Boolean"
//                      };
// typedef enum  {
//    PRIMITIVE,
//    ARRAY,
//    J_ARRAY,
//    NA
// }NonTerminal;
// char string_dec[4][20]= {
//    "PRIMITIVE",
//    "ARRAY",
//    "JAGGED_ARRAY",
//    "NA"
// };
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



typedef struct {
    int dim;//--4
    int ** dim_bound;//(7,4),(3,4),(),() 4X2
    //store array of strings for var_names if dynamic
    char ** upar;          //NA if not -1;
    char ** down;      //array of STRINGS; //NA if not -1;
}array_record;

typedef union {
    int *_line;
    int **jag_line;//jagged
}jagged_type;

typedef struct  {
    int dim;//2d or 3d ke liye tagg--
    int r1_low;
    int r1_high;    
    jagged_type dim_bound;

}jagged_array_record;

typedef union type_Expression_record{
     Terminal primitive_type;
     array_record arr_record;
     jagged_array_record j_arr_record;
} type_Expression_record;

typedef struct TypeExpression{      //neeche wala hi use kare!
    NonTerminal tag;
    bind_info info;
    type_Expression_record record;
}TypeExpression;

typedef struct TypeExpressionTable{
    char var_name[200];
    NonTerminal tag;
    bind_info info;
    type_Expression_record record;

}TypeExpressionTable;

int CAPACITY=0;
int m=0;
void printTypeExpressionsTable(TypeExpressionTable T[],char *filename){
    FILE *fp=fopen(filename,"w");
    if(fp==NULL){
        printf("Error opening");
        return;
    }
    printf("%d\n",m);
    fprintf(fp,"%10.10s %15.15s %15.15s  %15.15s \n","Variable","Type","Bind_type","Type_Expression");
    for(int i=0;i<m;i++){
        char var_name[200],dec_name[200],bind_name[200];
        strcpy(var_name,T[i].var_name);//field1
        bind_info info=T[i].info;
        strcpy(bind_name,string_bind[T[i].info]);//field2
        NonTerminal tag=T[i].tag;
        strcpy(dec_name,NonTerminalMap[T[i].tag]);//field3
        
        fprintf(fp,"%10.10s %15.15s %15.15s ", var_name,dec_name,bind_name);

        //field4 here on
        if(tag==primitive){ //Primitive
            fprintf(fp," <basicType=%s>",TerminalMap[T[i].record.primitive_type]);
            
            //print name, dec_type, bind_info, typeExpression
        }
        else if(tag==array){  
            //Array
            int dim=T[i].record.arr_record.dim;
            fprintf(fp," <type=rectangularArray, dimensions=%d, " ,dim);
            
            for(int j=0;j<dim;j++){
                int a= T[i].record.arr_record.dim_bound[j][0];
                int b= T[i].record.arr_record.dim_bound[j][1];
                //case for var_name in d_bind
                
                 fprintf(fp,"range_R%d= (%d, %d), ",j+1,a,b);
                
        
            }
            fprintf(fp,"basicElementType = Integer>");
            //basic=Integer by def

        }
        else if(tag==jagged_array){    //Jagged_ARRAY
            int dim=T[i].record.j_arr_record.dim;
            
            fprintf(fp," <type =jaggedArray, dimensions=%d, ",dim);
          
            if(dim==2){
                     int  high=T[i].record.j_arr_record.r1_high;
                     int low=T[i].record.j_arr_record.r1_low   ;
                      fprintf(fp,"range_R1=(%d, %d) range_R2 = (",low,high);
                        
                      for(int j=0;j<high-low+1;j++){
                         fprintf(fp," %d ",T[i].record.j_arr_record.dim_bound._line[j]);
                         if(j!=high-low)fprintf(fp,",");
                        //from (low,high) gives size  , 0 index 
                      }
                     
            }
            else{   //dim==3
                      int  high=T[i].record.j_arr_record.r1_high;
                     int low=T[i].record.j_arr_record.r1_low   ;
                     fprintf(fp,"range_R1=(%d, %d) range_R2 = ( ",low,high);
                    //  <type =jaggedArray, dimensions=3, range_R1=(4, 7), 
                    //3 [ 5, 3, 5] , 
                     //1 [  5], 2 [ 4, 3] ,3 [ 5, 4, 4]
                     for(int j=0;j<high-low+1;j++){
                         int m=T[i].record.j_arr_record.dim_bound.jag_line[j][0];
                         fprintf(fp,"%d [",m);
                         for(int k=0;k<m;k++){
                             fprintf(fp," %d", T[i].record.j_arr_record.dim_bound.jag_line[j][k+1]);
                             if(k!=m-1)fprintf(fp,",");
                         }
                          fprintf(fp," ]");
                          if(j!=high-low)fprintf(fp,", ");
                     }
                   
            }
            fprintf(fp,"), basicElementType = integer>");
           
       //basic=Integer by def
        }
        else{
            fprintf(fp," NOT A VALID TYPE EXPRESSION");
        }
         fprintf(fp,"\n");

    }
    fclose(fp);
}
            /********PARSE TREE NODE*********/

TypeExpressionTable * append_to_table(TypeExpressionTable * table,TypeExpression type,char * var){
    if(CAPACITY==0){
        table=(TypeExpressionTable *)malloc(sizeof(TypeExpressionTable)*1);
        CAPACITY=1;
    }
    else if(m==CAPACITY){
        table=(TypeExpressionTable *)realloc(table,sizeof(TypeExpressionTable)*CAPACITY*2);
        CAPACITY=CAPACITY*2;
        printf("reallocated %d %d\n",CAPACITY,m);
    }
    
       
        strcpy(table[m].var_name,var);
        table[m].tag=type.tag;
        table[m].info=type.info;
        table[m].record=type.record;
       
        m++;
    
    return table;
}

int main(){
    TypeExpressionTable *temp;
    TypeExpression t;
    //strcpy(t[0].var_name,"var_a");
    t.info=Static;
    t.tag=array;
    t.record.arr_record.dim=3;
    t.record.arr_record.dim_bound=(int **)malloc(sizeof(int *)*3);
    t.record.arr_record.dim_bound[0]=(int *)malloc(sizeof(int)*2);
    t.record.arr_record.dim_bound[0][0]=9;
    t.record.arr_record.dim_bound[0][1]=11;
    t.record.arr_record.dim_bound[1]=(int *)malloc(sizeof(int)*2);
    t.record.arr_record.dim_bound[1][0]=9;
    t.record.arr_record.dim_bound[1][1]=12;
    t.record.arr_record.dim_bound[2]=(int *)malloc(sizeof(int)*2);
    t.record.arr_record.dim_bound[2][0]=0;
    t.record.arr_record.dim_bound[2][1]=11;
   // printf("tag: %s\n",string_dec[t[0].tag]);
   temp=append_to_table(temp,t,"var_a");
   // strcpy(t[1].var_name,"var_p");
    t.info=N_A;
    t.tag=primitive;
    t.record.primitive_type=INTEGER;
    //printf("tag: %s\n",string_dec[t[1].tag]);
    temp=append_to_table(temp,t,"var_b");
//     strcpy(t[2].var_name,"var_2d_j");
    t.info=N_A;
    t.tag=jagged_array;
    t.record.j_arr_record.dim=2;
    t.record.j_arr_record.r1_low=3;
    t.record.j_arr_record.r1_high=8;
    t.record.j_arr_record.dim_bound._line=(int *)malloc(sizeof(int)*(8-3+1));
    for(int i=0;i<8-3+1;i++){
          t.record.j_arr_record.dim_bound._line[i]=i+8;
    }
    temp=append_to_table(temp,t,"var_c");
int arr[]={ 5,4, 3,  5, 4, 4,1,6,0};
    //strcpy(t[3].var_name,"var_3d_j");
    t.info=N_A;
    t.tag=jagged_array;
    t.record.j_arr_record.dim=3;
    t.record.j_arr_record.r1_low=2;
    t.record.j_arr_record.r1_high=4;
    t.record.j_arr_record.dim_bound.jag_line=(int * *)malloc(sizeof(int *)*(4-2+1));
    for(int i=0;i<4-2+1;i++){
          t.record.j_arr_record.dim_bound.jag_line[i]=(int  *)malloc(sizeof(int )*(i+3));
          t.record.j_arr_record.dim_bound.jag_line[i][0]=i+2; //2,3,4->1,2,3 elements and size is stored as 1, 2,3 for size i+1->make i+2
          for(int k=1;k<i+3;k++){
              t.record.j_arr_record.dim_bound.jag_line[i][k]=arr[k-1];
          }
    }
    temp=append_to_table(temp,t,"var_d");
    //strcpy(t[4].var_name,"var_p");
    t.info=N_A;
    t.tag=primitive;
    t.record.primitive_type=BOOLEAN;
    temp=append_to_table(temp,t,"var_e");
    printTypeExpressionsTable(temp,"new.txt");
    return 0;
}