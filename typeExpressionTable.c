/**
SHUBHAM AGARWAL -- 2018A7PS0301P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGRAWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#include "typeExpressionTable.h"

//********************8PRINTING TYPE EXPRESSION TABLE**************************//

void printTypeExpressionsTable(TypeExpressionTable T[], char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("ERROR OPENING FILE TO PRINT TYPE EXPRESSION TABLE");
        return;
    }

    fprintf(fp, "%10.10s %15.15s %15.15s  %15.15s \n", "Variable", "Type", "Bind_type", "Type_Expression");
    for (int i = 0; i < curr_table_entry; i++)
    {
        char var_name[200], dec_name[200], bind_name[200];
        strcpy(var_name, T[i].var_name); //field1
        bind_info info = T[i].info;
        strcpy(bind_name, string_bind[T[i].info]); //field2
        NonTerminal tag = T[i].tag;
        strcpy(dec_name, NonTerminalMap[T[i].tag]); //field3

        fprintf(fp, "%10.10s %15.15s %15.15s ", var_name, dec_name, bind_name);

        //field4 here on
        if (tag == primitive)
        { //Primitive
            fprintf(fp, " <basicType=%s>", TerminalMap[T[i].record.primitive_type]);

            //print name, dec_type, bind_info, typeExpression
        }
        else if (tag == array)
        {
            //Array
            int dim = T[i].record.arr_record.dim;
            fprintf(fp, " <type=rectangularArray, dimensions=%d, ", dim);

            for (int j = 0; j < dim; j++)
            {
                int a = T[i].record.arr_record.dim_bound[j][0];
                int b = T[i].record.arr_record.dim_bound[j][1];
                //case for var_name in d_bind
                if (a != -1 && b != -1)
                    fprintf(fp, "range_R%d= (%d, %d), ", j + 1, a, b);
                else if (a == -1 && b != -1)
                    fprintf(fp, "range_R%d= (%s, %d), ", j + 1, T[i].record.arr_record.l_indexes[j], b);
                else if (a != -1 && b == -1)
                    fprintf(fp, "range_R%d= (%d, %s), ", j + 1, a, T[i].record.arr_record.u_indexes[j]);
                else if (a == -1 && b == -1)
                    fprintf(fp, "range_R%d= (%s, %s), ", j + 1, T[i].record.arr_record.l_indexes[j], T[i].record.arr_record.u_indexes[j]);
            }
            fprintf(fp, "basicElementType = Integer>");
            //basic=Integer by def
        }
        else if (tag == jagged_array)
        { //Jagged_ARRAY
            int dim = T[i].record.j_arr_record.dim;

            fprintf(fp, " <type =jaggedArray, dimensions=%d, ", dim);

            if (dim == 2)
            {
                int high = T[i].record.j_arr_record.r1_high;
                int low = T[i].record.j_arr_record.r1_low;
                fprintf(fp, "range_R1=(%d, %d) range_R2 = (", low, high);

                for (int j = 0; j < high - low + 1; j++)
                {
                    fprintf(fp, " %d ", T[i].record.j_arr_record.dim_bound._line[j]);
                    if (j != high - low)
                        fprintf(fp, ",");
                    //from (low,high) gives size  , 0 index
                }
            }
            else
            { //dim==3
                int high = T[i].record.j_arr_record.r1_high;
                int low = T[i].record.j_arr_record.r1_low;
                fprintf(fp, "range_R1=(%d, %d) range_R2 = ( ", low, high);
                //  <type =jaggedArray, dimensions=3, range_R1=(4, 7),
                //3 [ 5, 3, 5] ,
                //1 [  5], 2 [ 4, 3] ,3 [ 5, 4, 4]
                for (int j = 0; j < high - low + 1; j++)
                {
                    int m = T[i].record.j_arr_record.dim_bound.jag_line[j][0];
                    fprintf(fp, "%d [", m);
                    for (int k = 0; k < m; k++)
                    {
                        fprintf(fp, " %d", T[i].record.j_arr_record.dim_bound.jag_line[j][k + 1]);
                        if (k != m - 1)
                            fprintf(fp, ",");
                    }
                    fprintf(fp, " ]");
                    if (j != high - low)
                        fprintf(fp, ", ");
                }
            }
            fprintf(fp, "), basicElementType = integer>");

            //basic=Integer by def
        }
        else
        {
            fprintf(fp, " NOT A VALID TYPE EXPRESSION");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
