#include "cmd.h"

Command_List* parse(Tokens* tokens) {
    Command_List* table = NULL;
    table = malloc(sizeof(Command_List));
    table->size = 1;
    table->list = malloc(sizeof(Command));

    table->list[0].size = 0;
    table->list[0].vector = malloc(sizeof(char*));
    table->list[0].vector[0] = NULL;

    table->infd[0] = table->appfd[0] = table->outfd[0] = '\0';

    for(int i = 0; i < tokens->size; ++i){
        if(strcmp(tokens->vector[i], "<") == 0){
            // WARNING: need to make sure that there exists at least 1 token afterwards
            strcpy(table->infd, tokens->vector[i + 1]);
            ++i;
        }
        else if(strcmp(tokens->vector[i], ">") == 0){
            strcpy(table->outfd,tokens->vector[i + 1]);
            ++i;
        }
        else if(strcmp(tokens->vector[i], ">>") == 0){
            strcpy(table->appfd, tokens->vector[i + 1]);
            ++i;
        }
        else if(strcmp(tokens->vector[i], "|") == 0){
            ++(table->size);
            table->list = realloc(table->list, table->size * sizeof(Command));
            Command* c = &(table->list[table->size - 1]);
            c->size = 0;
            c->vector = malloc(sizeof(char*));
            c->vector[0] = NULL;
        }
        else{
            Command* c = &(table->list[table->size - 1]);
            ++(c->size);
            c->vector = realloc(c->vector, (c->size + 1) * sizeof(char*));
            c->vector[c->size - 1] = malloc(ARG_MAX_LEN * sizeof(char));
            strcpy(c->vector[c->size - 1], tokens->vector[i]);
            c->vector[c->size] = NULL;
        }
    }

    return table;
}

void execute(Command_List* table){
	int pid;
    int backup_in = dup(STDIN_FILENO), backup_out = dup(STDOUT_FILENO);
    int infd, outfd;

    // deal with redirections
    if(strlen(table->infd) > 0){
        infd = open(table->infd, O_RDONLY, 0666);
    }
    else{
        infd = dup(backup_in);
    }

    for(int i = 0; i < table->size; i++){
        dup2(infd, STDIN_FILENO);
        close(infd);
        if(i == table->size - 1){
            // last pipe command, deal with output
            if(strlen(table->outfd) > 0){
                outfd = open(table->outfd, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            }
            else if(strlen(table->appfd) > 0){
                outfd = open(table->appfd, O_WRONLY | O_CREAT | O_APPEND, 0666);
            }
            else{
                outfd = dup(backup_out);
            }
        }
        else{
            int fd[2];
            pipe(fd);
            outfd = fd[1];
            infd = fd[0];
        }

        dup2(outfd, STDOUT_FILENO);
        close(outfd);

        pid = fork();
        if(pid == 0){
            execvp(table->list[i].vector[0], table->list[i].vector);
        }
    }

    dup2(backup_in, STDIN_FILENO);
    close(backup_in);
    dup2(backup_out, STDOUT_FILENO);
    close(backup_out);

	// wait for all child to finish
	while(wait(NULL) > 0);
}

void clean_command_list(Command_List* table){
    if(!table)
        return;
    for(int i = 0; i < table->size; ++i){
        for(int j = 0; j < table->list[i].size; ++j){
            free(table->list[i].vector[j]);
        }
        free(table->list[i].vector);
    }
    free(table->list);
    free(table);
}

void print_command(Command* command){
    for(int i = 0; i < command->size; ++i){
        printf("%s ", command->vector[i]);
    }
    printf("\n");
}

void print_command_list(Command_List* table){
    printf("infd is %s\n", table->infd);
    printf("outfd is %s\n", table->outfd);
    printf("appfd is %s\n", table->appfd);
    for(int i = 0; i < table->size; ++i){
        printf("printing %dth command\n", i);
        print_command(&(table->list[i]));
    }
}
