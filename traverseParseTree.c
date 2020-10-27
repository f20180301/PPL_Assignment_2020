#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef enum  {
   INTEGER,
   REAL,
   BOOLEAN, 
   TYPE_ERROR
}primitive;

typedef enum  {
   PRIMITIVE,
   ARRAY,
   J_ARRAY,
   NA
}declaration_type;

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
     primitive primitive_type;
     array_record arr_record;
     jagged_array_record j_arr_record;
} type_Expression_record;

typedef struct TypeExpression{
    declaration_type tag;
    type_Expression_record record;
}TypeExpression;


void traverse_parse_tree(/*receive params t, T*/){
    if(t->__=="START"){
        traverse(t->firstChild,T);
        traverse(t->firstChild->sibling,T);
    }
    else if(t->=="DECLARATION"){
        traverse(t->firstChild); //to dec_stmnt
        if(t->firstChild->sibling!=NULL){
        traverse(t->firstChild->sibling);
        }
   }
   else if(t->="DELARATION_STATEMENT"){
       //build for the "typeExpression" of this declarartion statement, error reporting
       TypeExpression exp_table_record;
       * temp=t;    //just being safe, making copy
       temp=temp->firstChild;
       if(temp->=="Primitive"){
           temp.tag=PRIMITIVE;
           temp=temp->firstchild; 
           * varlist_pointer;   //pointing to var_list  //moving into single/multiple_primitive node
           if(SINGLE){
               varlist_pointer=temp=temp->firstChild->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           else{//multiple
               varlist_pointer=temp=temp->firstchild->sibling->sibling->sibling->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           //
           exp_table_record.record.primitive_type=temp->child->value(int, real or bool);
           //daalnaa
         }
       else if(temp->=="Array"){
           temp.tag=ARRAY;
           ///temp2=temp
           temp=temp->fiirstchild; 
           * varlist_pointer; 
            if(SINGLE){
               varlist_pointer=temp=temp->fiirstchild->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
               //daalte hue hi nikalo.
           }
           else{
               varlist_pointer=temp=temp->fiirstchild->sibling->sibling->sibling->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
            temp=temp->sibling;//(ignore just after colon, arrray written)
            //<array_dim> hai aab, in temp
            
            do{
                temp=temp->firstChild //(moving into the child of  <array_dim>)
                temp=temp->sibling;
                id or num =temp->firstChild;
                temp=temp->sibling->sibling;
                id or num =temp->firstChild; 
                temp=temp->sibling->sibling;//reaches <array_dim>
                //chk type(dynamic), dimension(lower<upper), read, and error report, maintain counter for Dimension Count, dimension=count;
                store to type_Expression_record.record.arr_record;
            }while(temp!=NULL);
//temp2=daalde
       }
       //while exiting put in node

       else if(temp->=="Jagged_Array"){
           temp=temp->firstChild;
        if(temp->=="Jagged_2d_Array"){
           temp.tag=J_ARRAY;
           temp=temp->fiirstchild; 
           * varlist_pointer; 
                if(SINGLE){
               varlist_pointer=temp=temp->fiirstchild->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
                }
                else{
                    varlist_pointer=temp=temp->fiirstchild->sibling->sibling->sibling->sibling;
                    temp=temp->sibling->sibling;//moving into colon ke baad wala node
                }
           temp=temp->sibling->sibling;//(ignore just after colon, jagged and arrray written)
            //<jagged_dim> hai , in temp
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
                * temp_jag=temp->firstChild;  //point to R1             
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
            
            
          // exp_table_record.record.j_arr_record=temp->child->value;

       }
       else if(temp->=="Jagged_3d_Array"){
           temp.tag=J_ARRAY;
           temp=temp->fiirstchild; 
           * varlist_pointer; 
           if(SINGLE){
               varlist_pointer=temp=temp->fiirstchild->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           else{
               varlist_pointer=temp=temp->fiirstchild->sibling->sibling->sibling->sibling;
               temp=temp->sibling->sibling;//moving into colon ke baad wala node
           }
           temp=temp->sibling->sibling;//(ignore just after colon, jagged and arrray written)
            //<jagged_dim> hai , in temp
        //CONTINUE THIS WORK//CONTINUE THIS WORK//CONTINUE THIS WORK
            int l_bound=temp->firstChild->sibling->firstChild
            int u_bound=temp-firstChild->sibling->sibling->sibling->firstChild
            int dimension=3;
            if(l_bound<=u_bound){//chk =
                //kamm alag hoga.
            }
            else{
                //error
            }
           exp_table_record.record.j_arr_record=temp->child;
       }
       }
       //reach, type expression record ready,
       //populate varlist_pointer, with typeexpression,
       //populate typeExpressionTable with variable created.
   }

    else if(t->=="ASSIGNMENT"){
        traverse(t->firstChild);
        if(t->firstChild->sibling!=NULL){
        traverse(t->firstChild->sibling);
        }
}

else if(t->"Assign_Stmnt") //<assignment_stmt> 
//=> <arr_pid> EQUALS <expression> SEMICOLON
{
type_left;type_right;
//CONTINUE THIS WORK//CONTINUE THIS WORK//CONTINUE THIS WORK
t=t->firstChild;(check validity)//-->boundck, etc if array type var                   int a = 2 + 3.7 + 5;
t=t->sibling->sibling;//move to expree(send to function, find return type) -- , + , &&
(type_left!=type_right){
   // ERROR //otherwise no isseus
}
}

int main(){
return 0;
}