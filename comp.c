#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>

typedef enum{
    SEMI,
    OPEN_PAREN,
    CLOSED_PAREN
} TypeSeperator;

typedef enum{
    EXIT,
} TypeKeyword;

typedef enum{
    INT,
} TypeLiteral;


typedef struct{
    TypeKeyword type;
}TokenKeyword;

typedef struct{
    TypeSeperator type;
}TokenSeperator;

typedef struct{
    TypeLiteral type;
    int value;
}TokenLiteral;

TokenLiteral* gen_num(int* current_index, char* current){
    TokenLiteral* literal = (TokenLiteral*)malloc(sizeof(TokenLiteral));
    int n = atoi(&current[*current_index]);
    while(isdigit(current[*current_index]) && current[*current_index] != '\0'){
        (*current_index)++;
    }
    literal->type = INT;
    literal->value = n;
    return literal;
}

TokenKeyword* gen_key(int* current_index, char* current){
    TokenKeyword* key = (TokenKeyword*)malloc(sizeof(TokenKeyword));
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
        printf("%s\n", keyword_char);
        key->type = EXIT;
    }
    free(keyword);
    return key;
}

void lexer(FILE* fp){
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
    TokenLiteral* test_tok;
    TokenKeyword* key;
    int current_index = 0;
    while(current[current_index]!='\0'){
        if(isdigit(current[current_index])){
           test_tok = gen_num(&current_index, current);
        }else if(isalpha(current[current_index])){
            key = gen_key(&current_index, current);
        }
        current_index++;
    }
    printf("The number given is %d\n", test_tok->value);
    if(key->type == EXIT)
        printf("process exiting...\n");
    free(key);
    free(test_tok);
}


int main(){

  FILE* fp = fopen("test.cra", "r");
  if(!fp){
      perror("File not opened!!\n");
      exit(-1);
  }

  lexer(fp);
  fclose(fp);
  return 0;
}
