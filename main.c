#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "constant.h"

void prompt()
{
	printf("mum $ ");
}

// int main(int argc, char *argv[]){
int main()
{
	do
	{
		prompt();

		char buffer[LINE_MAX_LEN];
		fgets(buffer, LINE_MAX_LEN, stdin);
		int size = strlen(buffer);
		printf("%d\n", size);
		printf("%s! \n", buffer);

		if (strcmp(buffer, "exit\n") == 0)
		{
			break;
		}
	} while (true);

	exit(EXIT_SUCCESS);
}
