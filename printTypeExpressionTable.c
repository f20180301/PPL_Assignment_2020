#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef enum  {
   INTEGER,
   REAL,
   BOOLEAN, 
   TYPE_ERROR
}primitive;
char string_primitive[3][10] = {
                         "Integer",
                         "Real",
                         "Boolean"
                     };
typedef enum  {
   PRIMITIVE,
   ARRAY,
   J_ARRAY,
   NA
}declaration_type;
char string_dec[4][10]= {
   "PRIMITIVE",
   "ARRAY",
   "J_ARRAY",
   "NA"
};
typedef enum{
    Static,
    Dynamic,
    Not_Appplicable
}bind_info;
char string_bind[3][10] = {
                        "Static",
                        "Dynamic",
                        "Not_Appplicable"
                     };

typedef union {
    int *_line;
    int **jag_line;//jagged
}jagged_type;

typedef struct {
    int dim;//--4
    int ** dim_bound;//(7,4),(3,4),(),() 4X2
    //store array of strings for var_names if dynamic
}array_record;


typedef struct  {
    int dim;//2d or 3d ke liye tagg--
    int r1_low;
    int r1_high;    
    jagged_type dim_bound;

}jagged_array_record;

typedef union type_Expression_record{
     primitive primitive_type;
     array_record arr_record;
     jagged_array_record j_arr_record;
} type_Expression_record;

typedef struct TypeExpression{      //neeche wala hi use kare!
    declaration_type tag;
    type_Expression_record record;
}TypeExpression;

typedef struct TypeExpressionTable{
    char var_name[200];
    declaration_type tag;
    bind_info info;
    type_Expression_record record;

}TypeExpressionTable;

void printTypeExpressionsTable(TypeExpressionTable T[],int m){
    for(int i=0;i<m;i++){
        char var_name[200],dec_name[200],bind_name[200];
        strcpy(var_name,T[i].var_name);
        bind_info info=T[i].bind_info;
        strcpy(bind_name,string_bind[T[i].var_name]);
        declaration_type tag=T[i].tag;
        strcpy(dec_name,string_dec[T[i].var_name]);
        if(tag==0){ //Primitive
            string_primitive[T[i].record.primitive_type];
            //print name, dec_type, bind_info, typeExpression
        }
        else if(tag==1){    //Array
            int dim=T[i].record.arr_record.dim;
            for(int i=0;i<dim;i++){
                int a= T[i].record.arr_record.dim_bound[i][0];
                int b= T[i].record.arr_record.dim_bound[i][1];
                //var_name in d_bind
        
            }
            //basic=Integer by def

        }
        else if(tag==2){    //Jagged_ARRAY
            int dim=T[i].record.j_arr_record.dim;
            if(dim==2){
                      T[i].record.j_arr_record.r1_high
                      T[i].record.j_arr_record.r1_low   
                      T[i].record.j_arr_record.dim_bound._line[i]//from (low,high) gives size  , 0 index 
            }
            else{   //dim==3
                    T[i].record.j_arr_record.r1_high
                    T[i].record.j_arr_record.r1_low 
                    T[i].record.j_arr_record.dim_bound.jag_line[i]//from (low,high)  gives size, 0  index
                    T[i].record.j_arr_record.dim_bound.jag_line[i][j]//i from (low,high)  gives size,j basewd on size of [i]th jagged_row

            }
       //basic=Integer by def
        }
    }
}

int main(){
    TypeExpression t;
    t.tag=ARRAY;
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
    printf("tag %d",t.tag);
    t.record.primitive_type=BOOLEAN;
    int x=t.record.arr_record.dim;
    for(int i=0;i<x;i++){
       //int a= t.record.arr_record.dim_bound[i][0];
        //int b=t.record.arr_record.dim_bound[i][1];
        printf("%s   \n",string_primitive[ t.record.primitive_type]);
    }
    return 0;
}