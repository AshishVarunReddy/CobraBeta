#include<stdio.h>

#ifndef LEXER_H

#define LEXER_H

typedef enum{
    BEGINNING,
    STRING,
    INT,
    KEYWORD,
    SEPARATOR,
    IDENTIFIER,
    OPERATOR,
    EOFILE,
}Tokentype;


typedef struct{
    Tokentype Type;
    char* value;
}Token;


void print_token(Token* token);

Token* gen_num(int* current_index, char* current);

Token* gen_key_or_ident(int* current_index, char*current);

Token* gen_seperator_or_operator(int* current_index, char*current, Tokentype type);

Token** lexer(FILE* fp);

extern int token_index;

#endif
