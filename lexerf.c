#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>
#include"lexerf.h"


void print_token(Token* token){
    printf("\nPrinting token: ");
    printf("value: '%s'  ,", (token->value));

    switch(token->Type){
        case INT:
            printf("Type: INT\n");
            break;
        case KEYWORD:
            printf("Type: KEYWORD\n");
            break;
        case SEPARATOR:
            printf("Type: SEPARATOR\n");
            break;
        case OPERATOR:
            printf("Type: OPERATOR\n");
            break;
        case EOFILE:
            printf("Type: EOFILE\n");
            break;
        case BEGINNING:
            printf("Type: BEGINNING\n");
            break;
        default:
            printf("Undefined type\n");
    }
    
}

Token* gen_num(int* current_index, char* current){
    Token* literal = (Token*)malloc(sizeof(Token));
    int n = atoi(&current[*current_index]);
    int count = 0;
    while(isdigit(current[*current_index]) && current[*current_index] != '\0'){
        (*current_index)++;
        count++;
    }
    char* cf = (char*)malloc((count+1) * sizeof(char));
    *(cf+count) = '\0';
    sprintf(cf, "%d", n);
    literal->Type = INT;
    literal->value = cf;
    return literal;
}

Token* gen_key(int* current_index, char* current){
    Token* key = (Token*)malloc(sizeof(Token));
    char* keyword = (char*)malloc(sizeof(char)*32);
    int keyword_index = 0;
    while(isalpha(current[*current_index]) && current[*current_index] != '\0'){
        keyword[keyword_index++] = current[*current_index];
        (*current_index)++;
    }
    char keyword_char[keyword_index+1];
    for(int i = 0; i<keyword_index; i++){
        keyword_char[i] = keyword[i];
    }

    keyword_char[keyword_index] = '\0';

    if(!strcmp(keyword_char, "exit")){
        key->Type = KEYWORD;
        key->value = keyword;
    }else{
        key->Type = STRING;
        key->value = keyword;
    }
    return key;
}

Token* gen_seperator_or_operator(int* current_index, char* current, Tokentype type){
    Token* token = (Token*)malloc(sizeof(Token));
    char* s = (char*)malloc(2*sizeof(char));
    snprintf(s, 2, "%c", current[*current_index]);

    token->Type = type;
    token->value = s;

    return token;
}

int token_index = 0;


Token** lexer(FILE* fp){
    char* buffer = 0;
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = (char*)malloc(sizeof(char)* len+1);
    buffer[len] = '\0';
    fread(buffer, 1, len, fp);
    *(buffer+ sizeof(char)*len - 1) = '\0';
    printf("Buffer: %s\n", buffer);
    for(int i = 0; i<sizeof(char)*len+1; i++){
        if(buffer[i] == 10){
            printf("NewLine character detected at %d in the total buffer len of %zu\n", i, sizeof(char)*len + 1);
        }
    }
    char* current = buffer;
    int current_index = 0;
    Token** tokenArray = (Token**)malloc(sizeof(Token*)*12);
    while(current[current_index]!='\0'){
        if(token_index+1 > 12){
            tokenArray = (Token**)realloc(tokenArray, (token_index+1)*sizeof(Token*));
        }
        if(isdigit(current[current_index])){
           Token* test_tok = gen_num(&current_index, current);
           tokenArray[token_index++] = test_tok;
           current_index--;
        }else if(isalpha(current[current_index])){
            Token* key = gen_key(&current_index, current);
            tokenArray[token_index++] = key;
            current_index--;
        }else if(current[current_index] == ';'){
            Token* semi = (Token*)malloc(sizeof(Token));

            semi = gen_seperator_or_operator(&current_index, current, SEPARATOR);
            tokenArray[token_index++] = semi;
        }else if(current[current_index] == '('){
            Token* openP = gen_seperator_or_operator(&current_index, current, SEPARATOR);
            tokenArray[token_index++] = openP;
        }else if(current[current_index] == '+'){
            Token* plus = gen_seperator_or_operator(&current_index, current, OPERATOR);
            tokenArray[token_index++] = plus;
        }else if(current[current_index] == ')'){
            Token* closeP = gen_seperator_or_operator(&current_index, current, SEPARATOR);
            tokenArray[token_index++] = closeP;
        }else if(current[current_index] == ' '){
            current_index++;
            continue;
        }else if(current[current_index] == '\n'){
            printf("found newline character\n");
        }else if(current[current_index] == '-'){
            Token* minus = gen_seperator_or_operator(&current_index, current, OPERATOR);
            tokenArray[token_index++] = minus;
        }else if(current[current_index] == '/'){
            Token* divide = gen_seperator_or_operator(&current_index, current, OPERATOR);
            tokenArray[token_index++] = divide;
        }else if(current[current_index] == '*'){
            Token* multiply = gen_seperator_or_operator(&current_index, current, OPERATOR);
            tokenArray[token_index++] = multiply;
        }else{
            perror("Unknown Character");
            printf("%c\n", current[current_index]);
            exit(-1);
        }

        current_index++;
    }
    
    if(token_index+1 > 12){
        tokenArray = (Token**)realloc(tokenArray, (token_index+1)*sizeof(Token*));
    }
    Token* fintok = (Token*)malloc(sizeof(Token));
    fintok->value = "Eofile";
    fintok->Type = EOFILE;
    tokenArray[token_index++] = fintok;
    free(buffer);
    return tokenArray;
}



