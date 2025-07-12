#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include"lexerf.h"
#include"parserf.h"
#include<unistd.h>
#include<string.h>
#include<ctype.h>

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



void traverse(Node* root, FILE* fp){
    if(!root){
        return;
    }
    if(!strcmp(root->value, "exit")){
        printf("exits\n");
        fprintf(fp, "mov rax, %d\n", sysgen("exit"));
    }

    if(!strcmp(root->value, "(")){
        printf("( appeared\n");
    }

    if(!strcmp(root->value, ")")){
        printf(") appeared\n");
    }
    if(isdigit(*(root->value))){
        printf("%d appeared\n", atoi(root->value));
        fprintf(fp,"mov rdi, %d\n", atoi(root->value));
    }

    if(!strcmp(root->value, ";")){
        printf("; appeared\n");
        fprintf(fp, "syscall\n");
    }
    
    traverse(root->left, fp);
    traverse(root->right, fp);
}

int generate_code(Node* root){
    FILE* f = fopen("./assembly/gencra.asm", "w");
    if(!f){
        perror("File not opened!!\n");
        exit(1);
    }
    int syscode;
    syscode = sysgen(root->right->value);
    fprintf(f, "global _start\n");
    fprintf(f, "section .text\n_start:\n");
    traverse(root, f);
    fclose(f);
    system("nasm -f elf64 ./assembly/gencra.asm -o ./assembly/gencra.o");
    system("ld ./assembly/gencra.o -o ./assembly/gencra");
    system("rm ./assembly/gencra.o");
    system("rm ./assembly/gencra.asm");
    print_tree(root, "root", 0);
    return 0;
}


