#include "token.h"

// push a string to the back of tokens
void token_push_back(Tokens* tokens, char token[ARG_MAX_LEN], int *it){
	if(tokens->size == tokens->capacity){
		// resize if full capacity
		tokens->capacity *= TOKENS_RESIZE_FACTOR;
		tokens->vector = realloc(tokens->vector, tokens->capacity * sizeof(char*));
	}
	tokens->vector[tokens->size] = malloc(ARG_MAX_LEN * sizeof(char));
	strcpy(tokens->vector[tokens->size], token);
	++(tokens->size);

	// cleanup the token and reset its "iterator"
	token[0] = '\0';
	*it = 0;
}

// read from stdin and return tokens splited by space
Tokens* readline(){
	Tokens* tokens = NULL;
	char buffer[LINE_MAX_LEN];
	char token[ARG_MAX_LEN] = "\0";
	int it = 0; // token_tail_index
	bool prev_blank = true;

	int quote = 0; //01 means in 'xxx', 10 means in "xxx"
	bool complete = false;

	while(!complete && fgets(buffer, LINE_MAX_LEN, stdin)){
		if(!tokens){
			tokens = malloc(sizeof(Tokens));
			tokens->size = 0;
			tokens->capacity = TOKENS_INITIAL_CAPACITY;
			tokens->vector = malloc(tokens->capacity * sizeof(char*));
		}

		int buffer_size = strlen(buffer);
		for(int i = 0; i < buffer_size; ++i){
			if(quote == 01){
				if(buffer[i] == '\''){
					quote = 0;
					complete = true;
				}
				else{
					prev_blank = false;
					token[it] = buffer[i];
					++it;
					token[it] = '\0';
				}
			}
			else if(quote == 10){
				if(buffer[i] == '\"'){
					quote = 0;
					complete = true;
				}
				else{
					prev_blank = false;
					token[it] = buffer[i];
					++it;
					token[it] = '\0';
				}
			}
			else{
				if(buffer[i] == ' '){
					if(!prev_blank){
						// add token to tokens
						token_push_back(tokens, token, &it);
						prev_blank = true;
					}
				}
				else if(buffer[i] == '\n'){
					break;
				}
				else if(buffer[i] == '<'){
					// deal with "word<" to put word in tokens first, 
					// otherwise no " " to trigger token_push_back
					complete = false;
					if(!prev_blank){
						prev_blank = true;
						token_push_back(tokens, token, &it);
					}

					// < case
					token[0] = '<';
					token[1] = '\0';
					it = 1;
					token_push_back(tokens, token, &it);
				}
				else if(buffer[i] == '>'){
					complete = false;
					if(!prev_blank){
						prev_blank = true;
						token_push_back(tokens, token, &it);
					}

					if(i + 1 < buffer_size - 1 && buffer[i + 1] == '>'){
						// >> case
						token[0] = token[1] = '>';
						token[2] = '\0';
						it = 2;
						token_push_back(tokens, token, &it);
						++i;
					}
					else{
						// > case
						token[0] = '>';
						token[1] = '\0';
						it = 1;
						token_push_back(tokens, token, &it);
					}
				}
				else if(buffer[i] == '|'){
					complete = false;
					if(!prev_blank){
						prev_blank = true;
						token_push_back(tokens, token, &it);
					}

					// | case
					token[0] = '|';
					token[1] = '\0';
					it = 1;
					token_push_back(tokens, token, &it);
				}
				else if(buffer[i] == '\''){
					complete = false;
					quote = 01;
				}
				else if(buffer[i] == '\"'){
					complete = false;
					quote = 10;
				}
				else{
				// append char to token
				prev_blank = false;
				token[it] = buffer[i];
				++it;
				token[it] = '\0';
				complete = true;
			}
			}
		}

		if(complete && !prev_blank){
			// add token to tokens
			token_push_back(tokens, token, &it);
			// prev_blank = true;
			// it = 0;
		}

		if(!complete){
			printf("> ");
		}
	}

	/***
	 * unfinished command + ctrl d => nothing
	 * empty command + ctrl d => exit
	 * unfinished command + 2 X ctrl d will still exit, while on linux it will no nothing
	***/
	if(feof(stdin)){
		clean_token(tokens);
		printf("\nexit\n");
		exit(EXIT_SUCCESS);
	}

	return tokens;
}

void clean_token(Tokens* tokens){
	if(!tokens)
		return;
	for(int i = 0; i < tokens->size; i++){
		free(tokens->vector[i]);
	}
	free(tokens->vector);
	free(tokens);
}

void print_tokens(Tokens* tokens){
    printf("----printing %d tokens----\n", tokens->size);
    for(int i=0; i < tokens->size; i++){
		printf("---printing %dth tokens---\n", i);
        printf("%s\n", tokens->vector[i]);
    }
	printf("--------------------------\n");
}
