#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#include "constant.h"
#include "token.h"
#include "cmd.h"

static sigjmp_buf env;
static volatile sig_atomic_t isexecuting = 0;

void prompt();
void sigint_handler();

#endif
