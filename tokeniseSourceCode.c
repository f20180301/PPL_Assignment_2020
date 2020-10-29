/**
SHUBHAM AGARWAL -- 2018A7PS0310P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGARWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#include "tokeniseSourceCode.h"

//**************************FUNCTION TO FIND A TOKEN FOR A LEXEME******************//

char *getToken(char *lexeme)
{
    char c = lexeme[0];
    int flag = 0;

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
                    return "ERROR";
            }
            return "ID";
        }
    }
    return "Error";
}

//**********Test function for printing entire LinkedList of Tokens***********************//

void printLinkedList()
{
    token *temp = head;
    printf("\n NOW PRINTING \n\n");
    while (temp != NULL)
    {
        printf("%s         ->       %s             --->>         %d\n", temp->lexeme, temp->token, temp->lineno);
        temp = temp->next;
    }
}

//***************************FUNTION TO CREATE A TOKEN NODE AND RETURN A DYNAMICALLY CREATED RECORD TOKEN***********************//

token *createNode(char *lexeme, int lineno)
{
    token *a;
    a = (token *)malloc(sizeof(token));
    strcpy(a->lexeme, lexeme);
    strcpy(a->token, getToken(a->lexeme)); //To be hashed
    a->lineno = lineno;
    a->next = NULL;
    return a;
}

//********************FUNTION TO INSERT THE CREATED NODE OF A LEXEME*********************//

token *insertNode(token *node)
{
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

//*********************FUNCTION TO READ TXT FILE (SOURCE CODE) AND CREATES THE LINKED LIST OF TOKENS*************************//

void tokeniseSourcecode(char a[100], token *head)
{
    FILE *fp;
    token *s;
    int count = 0;
    char buff[1000];
    fp = fopen(a, "r+");
    if (fp == NULL)
    {
        printf("ERROR OPENING SOURCE CODE FILE\n");
    }
    while (fscanf(fp, "%[^\n]\n", buff) != EOF)
    {
        count++;
        char *token_split;
        token_split = strtok(buff, "  \t");
        while (token_split != NULL)
        {
            if (token_split != NULL)
            {
                s = createNode(token_split, count);
                insertNode(s);
            }
            token_split = strtok(NULL, "  \t");
        }
    }
    //printLinkedList();
    fclose(fp);
}