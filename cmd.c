#include "cmd.h"

Command_List* parse(Tokens* tokens) {
    Command_List* table = NULL;
    table = malloc(sizeof(Command_List));
    table->size = 1;
    table->list = malloc(sizeof(Command));

    table->list[0].size = 0;
    table->list[0].vector = malloc(sizeof(char*));
    table->list[0].vector[0] = NULL;

    // command_list->infd[0] = command_list->appfd[0] = command_list->outfd[0] = '\0';

    for(int i = 0; i < tokens->size; ++i){
        Command* c = &(table->list[0]);
        ++(c->size);
        c->vector = realloc(c->vector, (c->size + 1) * sizeof(char*));
        c->vector[c->size - 1] = malloc(ARG_MAX_LEN * sizeof(char));
        strcpy(c->vector[c->size - 1], tokens->vector[i]);
        c->vector[c->size] = NULL;
    }

    return table;
}

void execute(Command_List* table){
	int pid;
	pid = fork();
	if(pid == 0){
		execvp(table->list[0].vector[0], table->list[0].vector);
	}

	// wait for all child to finish
	while(wait(NULL) > 0);
}

void clean_command_list(Command_List* table){
    for(int i = 0; i < table->size; ++i){
        for(int j = 0; j < table->list[i].size; ++j){
            free(table->list[i].vector[j]);
        }
        free(table->list[i].vector);
    }
    free(table->list);
    free(table);
}
