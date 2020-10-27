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
    type_Expression_record record;
}TypeExpression;

typedef struct TypeExpressionTable{
    char var_name[200];
    NonTerminal tag;
    bind_info info;
    type_Expression_record record;

}TypeExpressionTable;

void printTypeExpressionsTable(TypeExpressionTable T[],int m,char *filename){
    FILE *fp=fopen(filename,"w");
    if(fp==NULL){
        printf("Error opening");
        return;
    }
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

int main(){
    TypeExpressionTable t[5];
    strcpy(t[0].var_name,"var_a");
    t[0].info=Static;
    t[0].tag=array;
    t[0].record.arr_record.dim=3;
    t[0].record.arr_record.dim_bound=(int **)malloc(sizeof(int *)*3);
    t[0].record.arr_record.dim_bound[0]=(int *)malloc(sizeof(int)*2);
    t[0].record.arr_record.dim_bound[0][0]=9;
    t[0].record.arr_record.dim_bound[0][1]=11;
    t[0].record.arr_record.dim_bound[1]=(int *)malloc(sizeof(int)*2);
    t[0].record.arr_record.dim_bound[1][0]=9;
    t[0].record.arr_record.dim_bound[1][1]=12;
    t[0].record.arr_record.dim_bound[2]=(int *)malloc(sizeof(int)*2);
    t[0].record.arr_record.dim_bound[2][0]=0;
    t[0].record.arr_record.dim_bound[2][1]=11;
   // printf("tag: %s\n",string_dec[t[0].tag]);
    strcpy(t[1].var_name,"var_p");
    t[1].info=N_A;
    t[1].tag=primitive;
    t[1].record.primitive_type=INTEGER;
    //printf("tag: %s\n",string_dec[t[1].tag]);
    
    strcpy(t[2].var_name,"var_2d_j");
    t[2].info=N_A;
    t[2].tag=jagged_array;
    t[2].record.j_arr_record.dim=2;
    t[2].record.j_arr_record.r1_low=3;
    t[2].record.j_arr_record.r1_high=8;
    t[2].record.j_arr_record.dim_bound._line=(int *)malloc(sizeof(int)*(8-3+1));
    for(int i=0;i<8-3+1;i++){
          t[2].record.j_arr_record.dim_bound._line[i]=i+8;
    }
int arr[]={ 5,4, 3,  5, 4, 4,1,6,0};
    strcpy(t[3].var_name,"var_3d_j");
    t[3].info=N_A;
    t[3].tag=jagged_array;
    t[3].record.j_arr_record.dim=3;
    t[3].record.j_arr_record.r1_low=2;
    t[3].record.j_arr_record.r1_high=4;
    t[3].record.j_arr_record.dim_bound.jag_line=(int * *)malloc(sizeof(int *)*(4-2+1));
    for(int i=0;i<4-2+1;i++){
          t[3].record.j_arr_record.dim_bound.jag_line[i]=(int  *)malloc(sizeof(int )*(i+3));
          t[3].record.j_arr_record.dim_bound.jag_line[i][0]=i+2; //2,3,4->1,2,3 elements and size is stored as 1, 2,3 for size i+1->make i+2
          for(int k=1;k<i+3;k++){
              t[3].record.j_arr_record.dim_bound.jag_line[i][k]=arr[k-1];
          }
    }
    strcpy(t[4].var_name,"var_p");
    t[4].info=N_A;
    t[4].tag=primitive;
    t[4].record.primitive_type=BOOLEAN;
    printTypeExpressionsTable(t,5,"new.txt");
    return 0;
}