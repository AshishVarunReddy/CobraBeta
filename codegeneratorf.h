#include"parserf.h"
#include"lexerf.h"


#ifndef _GENERATOR_H_
#define _GENERATOR_H_

typedef struct calls{
    int number;
    char* call;
    struct calls* next;
}calls;


int generate_code(Node* root);

#endif
