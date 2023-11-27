#include <unistd.h>

#include "constant.h"
#include "token.h"
#include "cmd.h"

void prompt(){
	printf("mum $ ");
}

int main(){
	do{
		prompt();

		Tokens* tokens = readline();
		Command_List* table = NULL;
		// print_tokens(tokens);
		if(tokens->size > 0 && strcmp(tokens->vector[0], "exit") == 0){
			clean_token(tokens);
			printf("exit\n");
			break;
		}

		table = parse(tokens);
		execute(table);
		clean_token(tokens);
		clean_command_list(table);
		tokens = NULL;
	} while (true);

	exit(EXIT_SUCCESS);
}
