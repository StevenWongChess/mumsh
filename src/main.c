#include "main.h"

void prompt(){
	printf("mum $ ");
}

void sigint_handler(){
	if(isexecuting == 1)
		return;
	else
		siglongjmp(env, 42);
}

int main(){
	// initialize
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);

	do{
		if(sigsetjmp(env, 1) == 42){
			printf("\n");
			continue;
		}

		prompt();

		Tokens* tokens = NULL;
		Command_List* table = NULL;

		tokens = readline();
		// print_tokens(tokens);

		if(tokens->size > 0 && strcmp(tokens->vector[0], "exit") == 0){
			clean_token(tokens);
			printf("exit\n");
			break;
		}

		table = parse(tokens);
		// print_command_list(table);
		isexecuting = 1;
		execute(table);
		isexecuting = 0;
		clean_token(tokens);
		clean_command_list(table);
	} while (true);

	exit(EXIT_SUCCESS);
}
