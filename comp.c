#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"lexerf.h"
#include"parserf.h"

extern int token_index;


int main(int argc, char** argv){

  FILE* fp = fopen(argv[1], "r");
  if(argc < 2){
      perror("Invalid compiler use\n");
      exit(1);
  }

  if(!fp){
      perror("File not opened!!\n");
      exit(-1);
  }
  Token** tokenArray = (Token**)malloc(sizeof(Token*) * token_index);
  tokenArray = lexer(fp);
  for(int i = 0; i<token_index; i++){
      print_token(tokenArray[i]);
  }
 parser(tokenArray);

  fclose(fp);
  free(tokenArray);


  return 0;
}
