#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"lexerf.h"

extern int token_index;


int main(){

  FILE* fp = fopen("test.cra", "r");
  if(!fp){
      perror("File not opened!!\n");
      exit(-1);
  }
  Token** tokenArray = (Token**)malloc(sizeof(Token*) * token_index);
  tokenArray = lexer(fp);
  for(int i = 0; i<token_index; i++){
      print_token(tokenArray[i]);
  }
  fclose(fp);
  free(tokenArray);
  return 0;
}
