#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<ctype.h>
#include"lexerf.h"
#include"codegeneratorf.h"



//typedef struct Node{
   // char* value;
   // Tokentype type;
   // struct Node* left;
  //  struct Node* right;
//}Node;

Node* create_node(char* value, Tokentype type){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void print_error(char* errorType){
    fprintf(stderr, "Error: %s\n", errorType);
    exit(-1);
}

void print_tree(Node* root, char* prestatement, int spaces){
    if(root == NULL){
        return;
    }
    for(int i = 0; i<spaces; i++){
        printf("   ");
    }
    printf("%s: %s\n",prestatement, root->value);

   print_tree(root->left, "left", spaces+1);
   print_tree(root->right, "right", spaces+1);
}


Node* parser(Token** tokenArray){
    Token* current_token = tokenArray[0];
    int i = 0;

    Node* root = create_node("PROGRAM",BEGINNING); 

    Node* current_node = root;
    while(current_token->Type != EOFILE){
        if(current_node == NULL){
            exit(-1);
        }
        if(current_node == root){
            //printf("root\n");
        }

        current_token = tokenArray[i];
        switch(current_token->Type){
            case KEYWORD:
                if(!strcmp(current_token->value, "exit")){
                    Node* exitNode = create_node(current_token->value, KEYWORD);
                    root->right = exitNode;
                    current_node = current_node->right;
                    current_token = tokenArray[++i];
                

                if(!strcmp(current_token->value, "(") && current_token->Type == SEPARATOR){
                    Node* oParen = create_node(current_token->value, SEPARATOR);
                    current_node->left = oParen;
                    current_token = tokenArray[++i];
                    if(current_token->Type == INT){
                             Node* exp = create_node(current_token->value, INT);
                             current_node->left->left = exp;
                             current_token = tokenArray[++i];
                             if(!strcmp(current_token->value, ")") && current_token->Type == SEPARATOR){
                                Node* cParen = create_node(current_token->value, SEPARATOR);
                                current_node->left->right = cParen;
                                current_token = tokenArray[++i];
                                if(current_token->value != NULL && !strcmp(current_token->value, ";") && current_token->Type == SEPARATOR){
                                    Node* semi = create_node(current_token->value, SEPARATOR);
                                    current_node->right = semi;
                                    current_node = root;
                                    break;
                                }else{
                                    print_error("Semi Colon Not found");
                                }
                             }else{
                                 print_error("No Closed Paren");
                             }
                    }else{
                        print_error("Not an Integer");
                    }
                }else{
                    print_error("No Open Paren");
                }
        }
                break;
            case SEPARATOR:
                //
                break;
            case INT:
                printf("INTEGER\n");
        }
        i++;

    }
    return root;
}




