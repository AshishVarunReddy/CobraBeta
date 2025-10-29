#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include"lexerf.h"
#include"parserf.h"
#include<unistd.h>
#include<string.h>
#include<limits.h>
#include"codegeneratorf.h"
#include<unistd.h>
#include<wait.h>
#include"hashtable.h"
#include<stdint.h>

extern int var_num;

int operations_int(int a, int b, char op){
    if(op == '+')return a+b;
    if(op == '-')return a-b;
    if(op == '*')return a*b;
    if(op == '/')return a/b;
    if(op == '%')return a%b;

    exit(-1);
}
int operations_asm(int a, int b, char op, FILE* fp){
    if(op == '+'){
        fprintf(fp, "mov rax, %d\nadd rax, %d\n", a, b);
        return a+b;
    }
    else if(op == '-'){
        fprintf(fp, "mov rax, %d\nsub rax, %d\n", a, b);
        return a-b;
    }
    else if(op == '*'){
        fprintf(fp, "mov rax, %d\nimul rax, %d\n", a, b);
        return a*b;
    }
    else if(op == '/'){
        fprintf(fp, "mov rax, %d\nmov r10, %d\nidiv r10\n", a, b);
        return a/b;
    }
    else if(op == '%'){
        fprintf(fp, "mov rax, %d\nmov r10, %d\nidiv r10\nmov rax, rdx\n",a , b);
        return a%b;
    }
    
    exit(-1);
}



int sysgen(char* call){
    int result;
    FILE* ff = fopen("checkout", "r");
    if(!ff){
        perror("Impossible checkout\n");
    }
    long length;
    fseek(ff, 0, SEEK_END);
    length = ftell(ff);
    fseek(ff, 0, SEEK_SET);
    char* buf = malloc(length*sizeof(char));
    fread(buf, 1, length, ff);
    buf[length*sizeof(char)-1] = '\0';
    int i = 0;
    char* current = strdup(buf);
    char* rem_str = current;
    for(char* p = strtok(current, ";"); p; p = strtok(rem_str, ";")){
            char* p2 = strdup(p);
            char* s = strtok(p2, " ");
            if(!strcmp(s, call)){
                result = atoi(p2+strlen(s)+1);
                break;
            }
            i++;
            rem_str = rem_str+strlen(p)+1;
            free(p2);
    }
    fclose(ff);
    return result;
}


int num_vars = 0;

int calculate_node(Node** op_node, FILE* fp, item** variable_s){
    
    if(!(*op_node)){
        perror("error op tree\n");
        exit(-1);
    }
    if((*op_node)->type == INT){
        return atoi((*op_node)->value);
    }

    if((*op_node)->left->type == OPERATOR){
        calculate_node(&((*op_node)->left), fp, variable_s);
    }

    if((*op_node)->right->type == OPERATOR){
        calculate_node(&((*op_node)->right), fp, variable_s);
    }

    if(((*op_node)->left->type == INT || (*op_node)->left->type == IDENTIFIER) && ((*op_node)->right->type == INT || (*op_node)->right->type == IDENTIFIER)){
        if((*op_node)->left->type == IDENTIFIER){
      item* changers = search_var(variable_s, var_num, (*op_node)->left->value);
      if(changers->value == NULL){
        perror("Variable not initialized\n");
        exit(1);
      }else{
        (*op_node)->left->value = changers->value;
        (*op_node)->left->type = INT;
        printf("code114:%s\n", changers->value);
      }
    }

       if((*op_node)->right->type == IDENTIFIER){
      printf("Hello right motherfukcers\n");
            item* changers = search_var(variable_s, var_num, (*op_node)->right->value);
            if(changers->value == NULL){
              perror("Variable not initialized\n");
              exit(1);
            }else{
              (*op_node)->right->value = changers->value;
              (*op_node)->right->type = INT;
              printf("code126: %s\n", changers->value);
            }
          }

        int a = atoi((*op_node)->left->value);
        int b = atoi((*op_node)->right->value);
        int digit = operations_int(a, b, *((*op_node)->value));

        if(digit < 0){
            char* newbf = (char*)malloc(sizeof(char) * strlen((*op_node)->value)+2);
            newbf[sizeof(char)*strlen((*op_node)->value) + 1] = '\0';
            sprintf(newbf, "%d", digit);
            (*op_node)->value = newbf;
        }
        else
            sprintf((*op_node)->value ,"%d",digit);

        (*op_node)->type = INT;
    }
   return atoi((*op_node)->value);
}
int16_t nbuf = 1;
int str_num = 0;
void traverse(Node* root, FILE* fp, calls* c, item** variable_s){
    if(!root){
        return;
    }
    if(!strcmp(root->value, "exit")){
        printf("exits\n");
        c->func_call = "exit";
        c->var_name = NULL;
        c->number = sysgen("exit");
    }
    if(!strcmp(root->value, "print")){
        printf("prints\n");
        c->func_call = "print";
        c->var_name = NULL;
        c->number = sysgen("write");
    }

    if(!strcmp(root->value, "(")){
        printf("( appeared\n");
    }

    if(root->type == IDENTIFIER){
        item* i;
        if(root->left && *(root->left->value) == '='){
          c->var_name = root->value;
          c->func_call = NULL;
        }
        if((i = search_var(variable_s, num_vars, root->value))){
            fprintf(fp, "mov r10, [rbp - %d]\n", i->depth);
            root->value = i->value;
            printf("173code: %s\n", root->value);
        }
    }

    if(!strcmp(root->value, ")")){
        printf(") appeared\n");
    }
    int t = (root->type);
    if((t == INT || t == OPERATOR) && *root->value != '='){
        int num;
        if(t == OPERATOR)
             num = calculate_node(&root, fp, variable_s);
        else num = atoi(root->value);
       // if(t == INT){
            fprintf(fp, "mov rax, %s\n", root->value);
        //}

        free(root->right);
        free(root->left);
        root->right = NULL;
        root->left = NULL;
        if(c->var_name){
            item* sResult = search_var(variable_s, var_num, c->var_name);
            printf("202code: %d\n", var_num);
            c->number = atoi(root->value);
            sResult -> value = root->value;
            printf("206code: %s, %d, %s\n", root->value, num_vars, sResult->value);
        }
        fprintf(fp, "mov r10, rax\n");
    }

    if(!strcmp(root->value, "int")){
        printf("variable initialised\n");
    }

    if((c->func_call || c->var_name) && !strcmp(root->value, ";")){
        printf("; appeared 211 code\n");
        if(c->func_call && !strcmp(c->func_call, "exit")){
        printf("213code\n");
        fprintf(fp, "%s:\nmov rax, %d\nmov rdi, r10\n", c->func_call, c->number);
        fprintf(fp, "syscall\n");
        }else if(c->func_call && !strcmp(c->func_call, "print")){
            printf("222code\n");

        }else if(c->var_name){
            printf("216 code: sagittarius\n");
            item* searchResult = search_var(variable_s, var_num, c->var_name);
            if(searchResult->depth == -1){
                searchResult->depth = 4*(num_vars+nbuf);
                fprintf(fp,"sub rsp, 4\nmov dword [rbp-%d], %s\n", 4*(num_vars+nbuf), searchResult->value);
                printf("220code: %s -> %d\n", searchResult->value, searchResult->depth);
                num_vars++;
            }else{
                fprintf(fp, "mov dword [rbp-%d], %s\n", searchResult->depth, searchResult->value);
                printf("224code: %s -> %d\n", searchResult->value, searchResult->depth);
            }
        }
    }
    if(root->left)
        traverse(root->left, fp, c, variable_s);
    if(root->right)
        traverse(root->right, fp, c, variable_s);
}

int generate_code(Node* root, item*** variable_s){
    FILE* f = fopen("./assembly/gencra.asm", "w");
    if(!f){
        perror("File not opened!!\n");
        exit(1);
    }
    printf("240code\n");
   
    calls* c = (calls*)malloc(sizeof(calls));
    c->func_call = NULL;
    c->var_name = NULL;
    c->number = INT_MIN;
    int syscode;
    if(root->right){
    syscode = sysgen(root->right->value);
    }
    fprintf(f, "global _start\n");
    fprintf(f, "section .text\n_start:\n");
    if(var_num > 0){
        fprintf(f, "push rbp\nmov rbp, rsp\n");
    }
    traverse(root, f, c, *variable_s);
    fclose(f);
    
    for(int i = 0; i<var_num; i++){
        printf("259code: %s -> %s -> %d\n", (*variable_s)[i]->key, (*variable_s)[i]->value, (*variable_s)[i]->depth);
    }
    
    
    pid_t id = fork();
    if(id == 0){
        execlp("nasm", "nasm", "-f", "elf64", "./assembly/gencra.asm", "-o", "./assembly/gencra.o", NULL);
        perror("nasm error\n");
        exit(-1);
    }else{
        waitpid(id, NULL, 0);
    }

    pid_t id2 = fork();
    if(id2 == 0){
        execlp("ld", "ld", "./assembly/gencra.o", "-o", "./assembly/gencra", NULL);
        perror("ld error\n");
        exit(1);
    }else{
        waitpid(id2, NULL, 0);
    }

    pid_t id3 = fork();
    if(id3 == 0){
        execlp("rm", "rm", "./assembly/gencra.o", NULL);
        perror("no rm\n");
        exit(1);
    }else{
        waitpid(id3, NULL, 0);
    }
    //system("rm ./assembly/gencra.asm");
    return 0;
}


