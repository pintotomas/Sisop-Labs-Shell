#include "utils.h"

// splits a string line in two
// acording to the splitter character
char* split_line(char* buf, char splitter) {

	int i = 0;

	while (buf[i] != splitter &&
		buf[i] != END_STRING)
		i++;
		
	buf[i++] = END_STRING;
	
	while (buf[i] == SPACE)
		i++;
	
	return &buf[i];
}

// looks in a block for the 'c' character
// and returns the index in which it is, or -1
// in other case
int block_contains(char* buf, char c) {

	for (int i = 0; i < strlen(buf); i++)
		if (buf[i] == c)
			return i;
	
	return -1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//splits a string into an array of strings according to a delimiter
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int get_array_of_words_lenght(char **lw){
    //PRE: lw termina en NULL
    int counter = 0;
    while (lw[counter] != NULL){
        counter++;
    }
    return counter;
}

void free_char_array(char **wl){

    int counter = 0;
    while (wl[counter] != NULL){
        free(wl[counter]);
        counter++;
    }
}

bool setEnviromentVariables(char *cmd){
    //returns True if it has set new enviroment variables
    char **splitted_cmd = str_split(cmd, ' ');
    int cantidad_asignaciones = get_array_of_words_lenght(splitted_cmd); //Supongo que son todas asignaciones
    bool set_envs = false;
    for (int i = 0; i < cantidad_asignaciones; i++){
        char *curr_pair = splitted_cmd[i];
        char **value_and_key = str_split(curr_pair, '=');
        int lenght_of_value_and_key = get_array_of_words_lenght(value_and_key);
        if (lenght_of_value_and_key == 2){ //Si No, no es una asignacion
            setenv(value_and_key[0], value_and_key[1], 1); //Siempre reescribo el valor de la variable de entorno si ya existe
            set_envs = true;
        }
        free_char_array(value_and_key); //Libero (key,value), cada valor de la tupla almacenada en memoria
        free(value_and_key); //Libero la memoria reservada para la tupla en si
    }
    
    //Libero memoria
    free_char_array(splitted_cmd);
    free(splitted_cmd);
    return set_envs;

}

