/**
SHUBHAM AGARWAL -- 2018A7PS0310P
YASH RAJ SINGH -- 2018A7PS0214P
SAHIL KATTNA -- 2018A7PS0154P
AGARWAL RAJAT RAMESH -- 2018A7PS0182P
**/

#ifndef _TOKENISESOURCECODE_
#define _TOKENISESOURCECODE_

/*************************TOKEN STRUCT IMPORTS******************************/

typedef struct token
{ //TOKEN STRUCTURE
    char lexeme[200];
    char token[200];
    int lineno;
    struct token *next;
} token;

token *head = NULL;
token *tail = NULL; //Head, Tail of LinkedList for TokenStream

char *ha[13][2] = {"program", //FOR KEYWORD
                   "PROGRAM",
                   "declare",
                   "DECLARE",
                   "list",
                   "LIST",
                   "of",
                   "OF",
                   "variables",
                   "VARIABLES",
                   "size",
                   "SIZE",
                   "values",
                   "VALUES",
                   "integer",
                   "INTEGER",
                   "real",
                   "REAL",
                   "boolean",
                   "BOOLEAN",
                   "array",
                   "ARRAY",
                   "jagged",
                   "JAGGED",
                   "R1",
                   "R1"};

char *op[16][2] = { //FOR KEYWORD
    "+",
    "PLUS",
    "-",
    "MINUS",
    "*",
    "MUL",
    "/",
    "DIVIDE",
    "(",
    "BROP",
    ")",
    "BRCL",
    "{",
    "CURLYOP",
    "}",
    "CURLYCL",
    "[",
    "SQOP",
    "]",
    "SQCL",
    "&&&",
    "AND",
    "|||",
    "OR",
    ":",
    "COLON",
    ";",
    "SEMICOLON",
    "..",
    "DD",
    "=",
    "EQUALS"};

//**********************FUNCTION DECLARATIONS***************************//
char *getToken(char *);
void printLinkedList();
token *createNode(char *, int);
token *insertNode(token *);
void tokeniseSourcecode(char *, token *);

#endif