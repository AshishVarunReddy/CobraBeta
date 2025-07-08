#include<stdio.h>
#include<stdlib.h>

#ifndef LEXER_H

#define LEXER_H

typedef enum{
    INT,
    KEYWORD,
    SEPARATOR,
    EOFILE,
}Tokentype;


typedef struct{
    Tokentype Type;
    char* value;
}Token;


void print_token(Token* token);

Token* gen_num(int* current_index, char* current);

Token* gen_key(int* current_index, char*current);

Token** lexer(FILE* fp);

extern int token_index;

#endif
