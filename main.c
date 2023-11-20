#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "constant.h"
#include "token.h"

void prompt(){
	printf("mum $ ");
}

int main(){
	do{
		prompt();

		Tokens* tokens = readline();
		print_tokens(tokens);
		if(tokens->size > 0 && strcmp(tokens->vector[0], "exit") == 0){
			clean_token(tokens);
			printf("exit\n");
			break;
		}

		// execute(tokens);
		clean_token(tokens);
	} while (true);

	exit(EXIT_SUCCESS);
}
