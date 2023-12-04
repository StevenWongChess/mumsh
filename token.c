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

	if(fgets(buffer, LINE_MAX_LEN, stdin)){
		tokens = malloc(sizeof(Tokens));
		tokens->size = 0;
		tokens->capacity = TOKENS_INITIAL_CAPACITY;
		tokens->vector = malloc(tokens->capacity * sizeof(char*));

		int buffer_size = strlen(buffer);
		for(int i = 0; i < buffer_size; ++i){
			if(buffer[i] == ' '){
				if(!prev_blank){
					// add token to tokens
					token_push_back(tokens, token, &it);
					prev_blank = true;
					it = 0;
				}
			}
			else if(buffer[i] == '\n'){
				break;
			}
			else{
				// append char to token
				prev_blank = false;
				token[it] = buffer[i];
				++it;
				token[it] = '\0';
			}
		}

		if(!prev_blank){
			// add token to tokens
			token_push_back(tokens, token, &it);
			// prev_blank = true;
			// it = 0;
		}

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
        printf("%s\n", tokens->vector[i]);
    }
}
