#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100

//type expression Table //fill while TRAVERSING PARSE-TREE
enum type_var{primitive, rectangular, jagged};
enum bind_type{static_bind, dynamic_bind, not_applicable};
typedef struct typeExpression{
    char name[100];
    enum type_var type;
    enum bind_type bind;
    //UNION
}typeExpression;

typeExpression typeExpressionTable[MAX];
void printTypeexpressiontable(){        //PRINTING TABLE (3 enteries)
    for(int i=0;i<3;i++){
        printf("%s %d %d\n", typeExpressionTable[i].name, typeExpressionTable[i].type, typeExpressionTable[i].bind);
    }
    return;
}
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

char * getToken(char *lexeme){
    if(strcmp(lexeme,"list")==0||strcmp(lexeme,"declare")==0){
        return "Keyword";
    }
    else if(strcmp(lexeme,"q2")==0){
        return "Var";
    }
    else{
        return "ERROR";
    }
    //DO SILMILARLY
}

token * createNode(char *lexeme,int lineno){      //Return Dynamically Created Record for TOKEN
    token *a;
    a=(token *)malloc(sizeof(token));
    strcpy(a->lexeme,lexeme);
    strcpy(a->token,getToken(a->lexeme));//To be hashed
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
    //checkingTokeniser
    tokeniseSourcecode("filename.txt",head);
    //checking TypeExpression
    typeExpression t;
    strcpy(t.name,"q1");
    t.type=primitive;
    t.bind=not_applicable;
    printf("%s %d %d\n\n", t.name, t.type, t.bind);
    //
    typeExpressionTable[0]=t;
    typeExpressionTable[1]=t;
    typeExpressionTable[2]=t;
    //
    printTypeexpressiontable();
    //end
    return 0;
}
//driver