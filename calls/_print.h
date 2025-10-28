#ifndef _PRINT_H_
#define _PRINT_H_

#include<unistd.h>
#include"../lexerf.h"
#include"../Tree.h"
#include"../parserf.h"
#include<string.h>
#include<stdlib.h>

void handle_print(Node** current_node, Token** tokenArray, int* ip);
#endif
