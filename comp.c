#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"lexerf.h"
#include"parserf.h"
#include"codegeneratorf.h"
#include"hashtable.h"

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
 
 item*** variable_s;
 variable_s = (item***)malloc(sizeof(item**));
 Node* parseNode = parser(tokenArray, variable_s);
 printf("holy\n");
 item** s = *variable_s;
 for(int i = 0; i<2; i++){
     printf("sigilite: %s\n", s[i]->key);
 }

 generate_code(parseNode, variable_s);

  fclose(fp);
  free(tokenArray);
  free(parseNode);

  return 0;
}
