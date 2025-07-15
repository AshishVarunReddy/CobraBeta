#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<ctype.h>
#include"lexerf.h"
#include"codegeneratorf.h"
#include"stackf.h"

extern int token_index;

/*typedef struct Node{
    char* value;
    Tokentype type;
    struct Node* left;
    struct Node* right;
}Node;*/

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

int op_prec(char s){
    if(s == '^')return 3;
    if(s == '*' || s == '/')return 2;
    if(s == '+' || s == '-')return 1;

    return -1;
}

char** exp_convertor(char** expr, int iter, int* mov){
    int count = 0;
    for(int x = 0; x<iter; x++){
        if(!strcmp(expr[x], ")") || !strcmp(expr[x] , "(")){
            count++;
        }
    }
    *mov = count;
    int z = iter-1;
    char** prefix = (char**)malloc(iter* sizeof(char*)+1);
    prefix[iter] = NULL;
    stack* st = create_stack_element("STACK_END");
    for(int i = iter-1; i>=0; i--){
       
       if(isalnum(*expr[i])){
            prefix[z] = expr[i];
            z--;
       }else if(*expr[i] == ')'){
            push(&st, expr[i]);
       }else if(*expr[i] == '('){
           while(empty_stack(st) == 0 && strcmp(peep(st), ")")){
               prefix[z] = peep(st);
               z--;
               popStack(&st);
           }
           popStack(&st);
       }else{
           while(empty_stack(st) == 0 && op_prec(*expr[i]) <= op_prec(*peep(st))){
               prefix[z] = peep(st);
               z--;
               popStack(&st);
           }
           push(&st, expr[i]);

       }
       
    }
    while(strcmp(peep(st), "STACK_END")){
        prefix[z] = peep(st);
        z--;
        popStack(&st);
    }
    printf("start ");
    for(int i = count; i < iter; i++){
        printf("%s ", prefix[i]);
    }
    printf("\n");
    return prefix;
}

static int x = 0;

Node* generate_operation_tree(char** expression_array, int lt){
       if(lt <= x)return NULL;
       Node* node = create_node(expression_array[x], (isalnum(*expression_array[x])) ? INT : OPERATOR);
       x++;
       
       if(node->type == INT){
           return node;
       }

       node->left = generate_operation_tree(expression_array,  lt);

       node->right = generate_operation_tree(expression_array,  lt);
       return node;
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
                    stack* s = create_stack_element("STACK_END");
                    push(&s, "(");
                    Node* oParen = create_node(current_token->value, SEPARATOR);
                    current_node->left = oParen;
                    current_token = tokenArray[++i];
                    if(!strcmp(current_token->value, "(") || current_token->Type == INT){
                            int j = i;
                            char** expr = (char**)malloc((token_index+1)*sizeof(char*));
                            expr[token_index] = NULL;
                            int iter = 0;
                            while(empty_stack(s) == 0 && tokenArray[j]->Type != EOFILE){
                                    
                                    if(strcmp(tokenArray[j] -> value, ")")){
                                        push(&s, tokenArray[j]->value);
                                    }else{
                                        while(strcmp(peep(s), "(")){
                                            popStack(&s);
                                        }
                                        popStack(&s);
                                    }
                                if(empty_stack(s) == 0)
                                    expr[iter++] = tokenArray[j]->value;
                                j++;
                            }
                            printf("bls: \n");
                            for(int p = 0; p<iter; p++){
                                printf("%s ",expr[p]);
                            }
                            printf("\n");
                            //printf("black hole:%s\n", exp_convertor(expr, iter));
                            int mov = 0;
                            char** expression_array = exp_convertor(expr, iter, &mov);
                            printf("bull \n");
                            //memmove(expression_array, expression_array+mov, ;
                            for(int y = 0; y<iter-mov; y++){
                                expression_array[y] = expression_array[y+mov];
                                printf("%s ", expression_array[y]);
                            }
                            expression_array[iter-mov] = NULL;
                            printf("\n");
                            Node* synode = generate_operation_tree(expression_array, iter-mov);
                            print_tree(synode, "center", 0);
                            current_node->left->left = synode;
                            current_token = tokenArray[j-1];
                            i = j-1; 
                             if(!strcmp(current_token->value, ")") && current_token->Type == SEPARATOR){
                                Node* cParen = create_node(current_token->value, SEPARATOR);
                                current_node->left->right = cParen;
                                current_token = tokenArray[++i];
                                if(current_token->value != NULL && !strcmp(current_token->value, ";") && current_token->Type == SEPARATOR){
                                    Node* semi = create_node(current_token->value, SEPARATOR);
                                    current_node->right = semi;
                                    current_node = root;
                                    print_tree(root, "root", 0);
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




