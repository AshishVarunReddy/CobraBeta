#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<ctype.h>
#include"lexerf.h"




typedef struct Node{
    char* value;
    Tokentype type;
    struct Node* left;
    struct Node* right;
}Node;

Node* create_node(char* value, Tokentype type){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
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


void parser(Token** tokenArray){
    Token* current_token = tokenArray[0];
    int i = 0;

    Node* root = create_node("PROGRAM",BEGINNING); 

    Node* current_node = root;
    while(current_token->Type != EOFILE){
        if(current_node == NULL){
            return;
        }
        if(current_node == root){
            printf("root\n");
        }

        switch(current_token->Type){
            case KEYWORD:
                if(!strcmp(current_token->value, "exit")){
                    Node* exitNode = create_node(current_token->value, KEYWORD);
                    root->right = exitNode;
                    current_node = current_node->right;
                    current_token = tokenArray[++i];
                }

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
                                if(!strcmp(current_token->value, ";") && current_token->Type == SEPARATOR){
                                    Node* semi = create_node(current_token->value, SEPARATOR);
                                    current_node->right = semi;
                                    current_token = tokenArray[++i];
                                    break;
                                }else{
                                    perror("Missing SemiColon\n");
                                    exit(-1);
                                }
                             }else{
                                 perror("Invalid Syntax\n");
                                 exit(-1);
                             }
                    }else{
                        perror("Invalid Syntax\n");
                        exit(-1);
                    }
                }else{
                    perror("Error found\n");
                    exit(-1);
                }
                break;
            case SEPARATOR:
                //
                break;
            case INT:
                //
                break;
        }
        print_tree(root, "root", 0);
        current_token = tokenArray[i++];
    }
    free(root);
}




