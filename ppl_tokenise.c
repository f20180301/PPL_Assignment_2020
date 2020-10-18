#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//type expression Table

//type expression table

//TokenisedStream
typedef struct token{       //TOKEN STRUCTURE
    char lexeme[200];
    char token[200];
    int lineno;
    struct token* next;
}token;

token * head=NULL;
token * tail=NULL;      //Head, Tail of LinkedList for TokenStream


token * createNode(char *lexeme,int lineno){      //Return Dynamically Created Record for TOKEN
    token *a;
    a=(token *)malloc(sizeof(token));
    strcpy(a->lexeme,lexeme);
    strcpy(a->token,"apt");//To be hashed
    a->lineno=lineno;
    a->next=NULL;
    return a;
}
token * insertNode(token *node){        //Inserts Node to LinkedList Head, Tail USED
    if(head==NULL && tail==NULL){
        head=tail=node;
        return head;
    }
    else{
        tail->next=node;
        tail=node;
    }
    return head;
}
void printLinkedList(){         //Test function for printing entire LinkedList of Tokens
    token * temp=head;
    printf("\n NOW PRINTING \n\n");
    while(temp!=NULL){
          printf("%s -> %s --->> %d\n",temp->lexeme,temp->token,temp->lineno);
          temp=temp->next;
    }
}

void tokeniseSourcecode(char a[100],token *head){       //Function for File Redaing and creating LinkedList of Tokens
    FILE *fp;
    token *s;
    int count=0;
    char buff[1000];
    fp=fopen(a,"r+");
    if(fp==NULL){
        printf("ERRROR\n");
    }
    while(fscanf(fp,"%[^\n]\n", buff)!=EOF){
       count++;
       char *token_split;
       token_split=strtok(buff," ");
       while(token_split!=NULL){
           if(token_split!=NULL){
           s=createNode(token_split,count);
           insertNode(s);
           }
           token_split=strtok(NULL," ");
       }
    }
    printLinkedList();
    fclose(fp);
}
//TokenisedStream

//driver
int main(){
    tokeniseSourcecode("filename.txt",head);
    return 0;
}
//driver