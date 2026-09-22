#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"lexerf.h"
#include"parserf.h"
#include"codegeneratorf.h"
#include"hashtable.h"

extern int token_index;


int main(int argc, char** argv){
//Opening the Source file...
  FILE* fp = fopen(argv[1], "r");
  if(argc < 2){
      perror("Invalid compiler use\n");
      exit(1);
  }

  if(!fp){
      perror("File not opened!!\n");
      exit(-1);
  }

//Feeding the Source file to lexer ig..
  Token** tokenArray = (Token**)malloc(sizeof(Token*) * token_index);
  tokenArray = lexer(fp);
  for(int i = 0; i<token_index; i++){
      print_token(tokenArray[i]);
  }
 
//Then parsing..
 item*** variable_s;
 variable_s = (item***)malloc(sizeof(item**));
 Node* parseNode = parser(tokenArray, variable_s);
 item** s = *variable_s;

 printf("holy\n");
//generating assembly..
 generate_code(parseNode, variable_s);

//closing the source file...
  fclose(fp);
  free(tokenArray);
  free(parseNode);

  return 0;
}
