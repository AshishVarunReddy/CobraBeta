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
        fprintf(fp, "mov rax, %d\nmov r9, %d\nimul r9\n", a, b);
        return a*b;
    }
    else if(op == '/'){
        fprintf(fp, "mov rax, %d\nmov r12, %d\ndiv r12\n", a, b);
        return a/b;
    }
    else if(op == '%'){
        fprintf(fp, "mov rax, %d\nmov r12, %d\ndiv r12\nmov rax, rdx\n",a , b);
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


int calculate_node(Node** op_node, FILE* fp){
    
    if(!(*op_node)){
        perror("error op tree\n");
        exit(-1);
    }
    if((*op_node)->type == INT){
        return atoi((*op_node)->value);
    }

    if((*op_node)->left->type == OPERATOR){
        calculate_node(&((*op_node)->left), fp);
    }

    if((*op_node)->right->type == OPERATOR){
        calculate_node(&((*op_node)->right), fp);
    }

    if((*op_node)->left->type == INT && (*op_node)->right->type == INT){
        int a = atoi((*op_node)->left->value);
        int b = atoi((*op_node)->right->value);
        sprintf((*op_node)->value ,"%d",operations_asm(a, b, *((*op_node)->value), fp));
        (*op_node)->type = INT;
    }
   return atoi((*op_node)->value);
}

static int flag = 0;

void traverse(Node* root, FILE* fp, calls* c){
    if(!root){
        return;
    }
    if(!strcmp(root->value, "exit")){
        printf("exits\n");
        //fprintf(fp, "exit:\n");
        //fprintf(fp, "mov rax, %d\n", sysgen("exit"));
        c->call = "exit";
        c->number = 60;
    }

    if(!strcmp(root->value, "(")){
        printf("( appeared\n");
    }

    if(!strcmp(root->value, ")")){
        printf(") appeared\n");
    }
    int t = (root->type);
    if((t == INT || t == OPERATOR) && flag == 0){
        int num = calculate_node(&root, fp);
        printf("black hole: %d\n", num);
        fprintf(fp, "mov r12, rax\n");
        flag = 1;
    }

    if(!strcmp(root->value, ";")){
        printf("; appeared\n");
        fprintf(fp, "%s:\nmov rax, %d\nmov rdi, r12\n", c->call, c->number);
        fprintf(fp, "syscall\n");
    }
    
    traverse(root->left, fp, c);
    traverse(root->right, fp, c);
}

int generate_code(Node* root){
    FILE* f = fopen("./assembly/gencra.asm", "w");
    if(!f){
        perror("File not opened!!\n");
        exit(1);
    }
    
    
    calls* c = (calls*)malloc(sizeof(calls));
    c->call = NULL;
    c->number = -1;
    c->next = NULL;
    int syscode;
    syscode = sysgen(root->right->value);
    fprintf(f, "global _start\n");
    fprintf(f, "section .text\n_start:\n");
    traverse(root, f, c);
    fclose(f);
    
    
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
        exit(-1);
    }else{
        waitpid(id2, NULL, 0);
    }

    pid_t id3 = fork();
    if(id3 == 0){
        execlp("rm", "rm", "./assembly/gencra.o", NULL);
        perror("no rm\n");
        exit(-1);
    }else{
        waitpid(id3, NULL, 0);
    }
    //system("rm ./assembly/gencra.asm");
    free(c);
    return 0;
}


