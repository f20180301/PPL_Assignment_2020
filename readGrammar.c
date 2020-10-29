/**
SHUBHAM AGARWAL -- 2018A7PS0310P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGARWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#include "readGrammar.h"
//*************TO CREATE SUBSTRING OF A STRING -- HELPER FUNCTION************************//
void substring(char *a, char *b, int str, int end)
{
    int c = 0;
    for (int i = str; i <= end; i++)
    {
        a[c++] = b[i];
    }
    a[c] = '\0';
}

//*************HELPER FUNCTION FOR READ GRAMMAR**********************//

int find(char *str, int isterm)
{

    //Terminal
    if (isterm == 1)
    {
        for (int i = 0; i < TERMINALS; i++)
        {
            if (strcmp(str, TerminalMap[i]) == 0)
                return i;
        }
    }

    //Non terminal
    else
    {
        for (int i = 0; i < NON_TERMINALS; i++)
        {
            if (strcmp(str, NonTerminalMap[i]) == 0)
                return i;
        }
    }
    printf("Error - NOT FOUND %s || RETURNING START SYMBOL \n", str);
    return 0;
}

//***************FUNTION TO READ GRAMMAR**************************//

void readGrammar(char a[100], grammar *G)
{
    FILE *fp;
    int count = 0;
    int count2 = 0;
    char buff[1000];
    char tmp[100];
    char tmp2[100];
    fp = fopen(a, "r+");
    if (fp == NULL)
    {
        printf("ERROR OPENING FILE TO READ GRAMMAR\n");
        return;
    }
    char *grammar_split = NULL;
    grammar *gramcurr = G;
    grules *grulescurr = NULL;
    rule *rulecurr = NULL;
    while (fscanf(fp, "%[^\n]\n", buff) != EOF)
    {
        count++;
        grammar_split = strtok(buff, " ");
        if (grammar_split == NULL)
        {
            printf("ERROR TOKENIZING THE LINE TO READ GRAMMAR\n");
        }
        if (grammar_split != NULL)
        {
            substring(tmp, grammar_split, 1, strlen(grammar_split) - 2); // REMOVING THE OPENING < AND CLOSING > FROM LHS NON TERMINAL
        }
        if (count == 1) // FIRST ITERATION
        {
            //Initialsing grammar
            gramcurr->nt = find(tmp, 0); //Initialised nt
            grulescurr = malloc(sizeof(grules));
            gramcurr->head = grulescurr; // Initialised head

            //Initialising grules
            rulecurr = malloc(sizeof(rule));
            grulescurr->head = rulecurr;
            grulescurr->next = NULL;

            //Initialising rule
            rulecurr->next = NULL;
        }

        else if (gramcurr->nt == find(tmp, 0))
        {
            //Need to make a new grules for the same LHS
            grulescurr->next = malloc(sizeof(grules));
            grulescurr = grulescurr->next;
            //Initialsing a new node of grules
            grulescurr->next = NULL;
            rulecurr = malloc(sizeof(rule));
            rulecurr->next = NULL;
            grulescurr->head = rulecurr;
        }
        else
        {
            //Need to go add a new fresh rule for a fresh rule new LHS
            gramcurr++;
            //Initialsing grammar
            gramcurr->nt = find(tmp, 0); //Initialised nt
            grulescurr = malloc(sizeof(grules));
            gramcurr->head = grulescurr; // Initialised head

            //Initialising grules
            rulecurr = malloc(sizeof(rule));
            grulescurr->head = rulecurr;
            grulescurr->next = NULL;

            //Initialising rule
            rulecurr->next = NULL;
            //ruleprev = NULL;
        }

        while (grammar_split != NULL) // TRAVERSIG THE RHS OF A RULE
        {
            grammar_split = strtok(NULL, " ");
            if (grammar_split != NULL)
            {
                if (strcmp(grammar_split, "=>") == 0) // EATING THE => IN GRAMMAR TXT FILE
                {
                    continue;
                }
                count2++;
                if (count2 != 1) // IF NOT THE FIRST ITERATION
                {
                    rulecurr->next = malloc(sizeof(rule));
                    rulecurr = rulecurr->next;
                    rulecurr->next = NULL;
                }
                if (grammar_split[0] == '<') // ENCOUNTERED A NON-TERMINAL
                {
                    substring(tmp2, grammar_split, 1, strlen(grammar_split) - 2);
                    rulecurr->isterm = 0;
                    rulecurr->type.nt = find(tmp2, 0);
                }
                else // ENCOUNTERED A TERMINAL
                {
                    rulecurr->isterm = 1;
                    rulecurr->type.t = find(grammar_split, 1);
                }
            }
        }
        count2 = 0;
    }
    fclose(fp);
    return;
}