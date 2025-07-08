#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>

typedef enum{
    INT,
    KEYWORD,
    SEPARATOR,
}Tokentype;


typedef struct{
    Tokentype Type;
    char* value;
}Token;


void print_token(Token* token){
    printf("\nPrinting token...\n");
    printf("value: %s\n", (token->value));
    if(token->Type == INT){
        printf("The type is INT\n");
    }else if(token->Type == KEYWORD){
        printf("The type is KEYWORD\n");
    }else if(token->Type == SEPARATOR){
        printf("The type is KEYWORD\n");
    }else{
        printf("Unknown type\n");
    }
    printf("\n");
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
    }
    return key;
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
    char* current = (char*)malloc(sizeof(char)*len + 1);
    printf("Buffer: %s", buffer);
    current = buffer;
    int current_index = 0;
    Token** tokenArray = (Token**)malloc(sizeof(Token*)*2);
    while(current[current_index]!='\0'){
        if(token_index+1 > 2){
            tokenArray = (Token**)realloc(tokenArray, (token_index+1)*sizeof(Token*));
        }
        if(isdigit(current[current_index])){
           Token* test_tok = gen_num(&current_index, current);
           print_token(test_tok);
           tokenArray[token_index++] = test_tok;
           current_index--;
        }else if(isalpha(current[current_index])){
            Token* key = gen_key(&current_index, current);
            print_token(key);
            tokenArray[token_index++] = key;
            current_index--;
        }else if(current[current_index] == ';'){
            Token* semi = (Token*)malloc(sizeof(Token));
            semi->value = ";";
            semi->Type = SEPARATOR;
            print_token(semi);
            tokenArray[token_index++] = semi;
        }else if(current[current_index] == '('){
            Token* openP = (Token*)malloc(sizeof(Token));
            openP->value = "(";
            openP->Type = SEPARATOR;
            print_token(openP);
            tokenArray[token_index++] = openP;
        }else if(current[current_index] == ')'){
            Token* closeP = (Token*)malloc(sizeof(Token));
            closeP->value = ")";
            closeP->Type = SEPARATOR;
            print_token(closeP);
            tokenArray[token_index++] = closeP;
        }
        current_index++;
    }
    return tokenArray;
}


int main(){

  FILE* fp = fopen("test.cra", "r");
  if(!fp){
      perror("File not opened!!\n");
      exit(-1);
  }
  Token** tokenArray = (Token**)malloc(sizeof(Token*) * token_index);
  tokenArray = lexer(fp);
  print_token(tokenArray[1]);
  fclose(fp);
  free(tokenArray);
  return 0;
}
