#ifndef CLASS_H
#define CLASS_H

#include "constant.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// constants related to tokens
#define TOKENS_INITIAL_CAPACITY 4
#define TOKENS_RESIZE_FACTOR 2

typedef struct _Tokens
{
  // array of token, will resize like a std::vector in c++14
  int size;
  int capacity;
  char** vector;
}Tokens;

void token_push_back(Tokens* tokens, char token[ARG_MAX_LEN], int *it);
Tokens* readline();
void clean_token(Tokens* tokens);
void print_tokens(Tokens* tokens);

#endif
