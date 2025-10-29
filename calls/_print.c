#include"_print.h"

void handle_print(Node** current_node, Token** tokenArray, int* ip){
    Node* init_node = (*current_node);
    if(strcmp(tokenArray[*ip]->value, "print")){
    printf("error Aborting...\n");
    exit(1);
    }
    Node* printNode = create_node(tokenArray[*ip]->value, tokenArray[*ip]->Type);
    (*current_node)->right = printNode;
    (*current_node) = printNode;
    (*ip)++;

    if(*tokenArray[*ip]->value != '('){
    fprintf(stderr, "No opening brackets.. Aborting...\n");
    exit(1);
    }
    Node* oParen = create_node(tokenArray[*ip]->value, tokenArray[*ip]->Type);
    (*current_node)->left = oParen;
    (*current_node) = oParen;
    (*ip)++;

    Node* str = create_node(tokenArray[*ip]->value, tokenArray[*ip]->Type);
    (*current_node)->left = str;

    (*ip)++;

    if(*tokenArray[*ip]->value != ')'){
      fprintf(stderr, "No closing brackets.. Aborting...\n");
      exit(1);
    }

    Node* cParen = create_node(tokenArray[*ip]->value, tokenArray[*ip]->Type);
    (*current_node)->right = cParen;
    (*current_node) = init_node;
    (*ip)++;

    if(*tokenArray[*ip]->value != ';'){
        fprintf(stderr, "No semiColon.. Aborting..\n");
        exit(1);
    }
    
    Node* semi = create_node(tokenArray[*ip]->value, tokenArray[*ip]->Type);
    (*current_node)->right = semi;
    (*current_node) = semi;
    
}


