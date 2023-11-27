#include "constant.h"
#include "token.h"

typedef struct _Command{
	int size;
	char **vector;
}Command;

typedef struct _Command_List{
	int size;
	Command *list;
	// char infd[ARG_MAX_LEN];
	// char appfd[ARG_MAX_LEN];
	// char outfd[ARG_MAX_LEN];
}Command_List;

Command_List* parse(Tokens* tokens);
void execute(Command_List* table);

void clean_command_list(Command_List* table);
