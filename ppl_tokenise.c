#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100

char *ha[13][2];
char *op[16][2]; //FOR KEYWORD

//type expression Table //fill while TRAVERSING PARSE-TREE
enum type_var
{
    primitive,
    rectangular,
    jagged
};
enum bind_type
{
    static_bind,
    dynamic_bind,
    not_applicable
};
// typedef struct typeExpression{
//     char name[100];
//     enum type_var type;
//     enum bind_type bind;
//     //UNION...
// }typeExpression;

// typeExpression typeExpressionTable[MAX];
// void printTypeexpressiontable(){        //PRINTING TABLE (3 enteries)
//     for(int i=0;i<3;i++){
//         printf("%s %d %d\n", typeExpressionTable[i].name, typeExpressionTable[i].type, typeExpressionTable[i].bind);
//     }
//     return;
// }
//in new file
//type expression table

//TokenisedStream
typedef struct token
{ //TOKEN STRUCTURE
    char lexeme[200];
    char token[200];
    int lineno;
    struct token *next;
} token;

token *head = NULL;
token *tail = NULL; //Head, Tail of LinkedList for TokenStream

char *getToken(char *lexeme)
{
    char c = lexeme[0];
    int flag = 0;
    /* if(c>=48&&c<=57)
    {
        int length=strlen(lexeme);
         int i=0,fl=0;
        while(lexeme[i++] != '\0')    
       {
        if(lexeme[i] == '.')    // decimal point is present
        {
            fl = 1;
            break;
        }
       }
       if(fl)
       return "Real";
       else
       return "Integer";
       
    }
    else if(strcmp(lexeme,"true")==0||strcmp(lexeme,"false")==0)
    return "Boolean";*/
    if ((c >= '0' && c <= '9') || strlen(lexeme) > 1 && c == '-' && (lexeme[1] >= '0' && lexeme[1] <= '9'))
        return "NUM";
    else if (flag == 0)
    {
        for (int i = 0; i < 13; i++)
            if (strcmp(lexeme, ha[i][0]) == 0)
                return ha[i][1];

        flag = 1;
    }
    if (flag == 1)
    {
        for (int i = 0; i < 16; i++)
        {
            if (strcmp(lexeme, op[i][0]) == 0)
                return op[i][1];
        }
        flag = 2;
    }
    if (flag == 2)
    {
        char d = lexeme[0];
        int i = 1;
        if (d == '_' || (d >= 65 && d <= 90) || (d >= 97 && d <= 122))
        {
            while (lexeme[i] != '\0')
            {
                d = lexeme[i];
                if ((i < 20) && (d == '_' || (d >= 65 && d <= 90) || (d >= 97 && d <= 122) || (d >= 48 && d <= 57)))
                {
                    i++;
                    continue;
                }
                else
                {
                    //printf("yaha %d %s \n\n",i,lexeme);
                    return "ERROR";
                }
            }
            return "ID";
        }
    }
    return "Error";
}

token *createNode(char *lexeme, int lineno)
{ //Return Dynamically Created Record for TOKEN
    token *a;
    a = (token *)malloc(sizeof(token));
    strcpy(a->lexeme, lexeme);
    strcpy(a->token, getToken(a->lexeme)); //To be hashed
    a->lineno = lineno;
    a->next = NULL;
    return a;
}
token *insertNode(token *node)
{ //Inserts Node to LinkedList Head, Tail USED
    if (head == NULL && tail == NULL)
    {
        head = tail = node;
        return head;
    }
    else
    {
        tail->next = node;
        tail = node;
    }
    return head;
}
void printLinkedList()
{ //Test function for printing entire LinkedList of Tokens
    token *temp = head;
    printf("\n NOW PRINTING \n\n");
    while (temp != NULL)
    {
        printf("%10.10s %10.10s %d\n", temp->lexeme, temp->token, temp->lineno);
        temp = temp->next;
    }
}

void tokeniseSourcecode(char a[100], token *head)
{ //Function for File Redaing and creating LinkedList of Tokens
    FILE *fp;
    token *s;
    int count = 0;
    char buff[1000];
    fp = fopen(a, "r+");
    if (fp == NULL)
    {
        printf("ERRROR\n");
    }
    while (fscanf(fp, "%[^\n]\n", buff) != EOF)
    {
        count++;
        char *token_split;
        token_split = strtok(buff, " \t");
        while (token_split != NULL)
        {
            if (token_split != NULL)
            {
                s = createNode(token_split, count);
                insertNode(s);
            }
            token_split = strtok(NULL, " \t");
        }
    }
    printLinkedList();
    fclose(fp);
}
//TokenisedStream

//driver
int main()
{
    //checkingTokeniser
    ha[0][0] = "program";
    ha[0][1] = "PROGRAM";
    ha[1][0] = "declare";
    ha[1][1] = "DECLARE";
    ha[2][0] = "list";
    ha[2][1] = "LIST";
    ha[3][0] = "of";
    ha[3][1] = "OF";
    ha[4][0] = "variables";
    ha[4][1] = "VARIABLES";
    ha[5][0] = "size";
    ha[5][1] = "SIZE";
    ha[6][0] = "values";
    ha[6][1] = "VALUES";
    ha[7][0] = "integer";
    ha[7][1] = "INTEGER";
    ha[8][0] = "real";
    ha[8][1] = "REAL";
    ha[9][0] = "boolean";
    ha[9][1] = "BOOLEAN";
    ha[10][0] = "array";
    ha[10][1] = "ARRAY";
    ha[11][0] = "jagged array";
    ha[11][1] = "JAGGED";
    ha[12][0] = "R1";
    ha[12][1] = "R1";

    op[0][0] = "+";
    op[0][1] = "PLUS";
    op[1][0] = "-";
    op[1][1] = "MINUS"; //ADD DASH WALA -- IF NEEDED HERE ; dono se bhejega MINUS;
    op[2][0] = "*";
    op[2][1] = "MUL";
    op[3][0] = "/";
    op[3][1] = "DIVIDE";
    op[4][0] = "(";
    op[4][1] = "BROP";
    op[5][0] = ")";
    op[5][1] = "BRCL";
    op[6][0] = "{";
    op[6][1] = "CURLYOP";
    op[7][0] = "}";
    op[7][1] = "CURLYCL";
    op[8][0] = "[";
    op[8][1] = "SQOP";
    op[9][0] = "]";
    op[9][1] = "SQCL";
    op[10][0] = "&&&";
    op[10][1] = "AND";
    op[11][0] = "|||";
    op[11][1] = "OR";
    op[12][0] = ":";
    op[12][1] = "COLON";
    op[13][0] = ";";
    op[13][1] = "SEMICOLON";
    op[14][0] = "..";
    op[14][1] = "DD";
    op[15][0] = "=";
    op[15][1] = "EQUALS";

    tokeniseSourcecode("testcases/t1.txt", head);
    //checking TypeExpression
    // typeExpression t;
    // strcpy(t.name,"q1");
    // t.type=primitive;
    // t.bind=not_applicable;
    // //printf("%s %d %d\n\n", t.name, t.type, t.bind);
    // //
    // typeExpressionTable[0]=t;
    // typeExpressionTable[1]=t;
    // typeExpressionTable[2]=t;
    //
    // printTypeexpressiontable();
    //end
    return 0;
}
//driver
