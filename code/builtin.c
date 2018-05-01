#include "builtin.h"
#include "utils.h"
#include "stdbool.h"
// returns true if the 'exit' call
// should be performed

int compare_first_word(char *line, const char *cmpWord){
	char * copy = malloc(strlen(line) + 1); 
	strcpy(copy, line);
	bool res = false;
	char **line_split = str_split(copy,' ');
	if (strcmp(line_split[0], cmpWord) == 0){
			res = true;
	}
	free_char_array(line_split);
	free(line_split);
	free(copy);
	return res;

}

char* getCurrentDirectory(void){
    char *directory = (char *) malloc(sizeof(char)*PATH_LENGHT);
    getcwd(directory, PATH_LENGHT);
    return directory;
}


int exit_shell(char* cmd) {
	//Your code here
	bool perform_exit = compare_first_word(cmd,"exit");
    if (perform_exit){
        exit(EXIT_SUCCESS);
        return true;
    }
    return false;

}

// returns true if "chdir" was performed
// this means that if 'cmd' contains:
// 	$ cd directory (change to 'directory')
// 	$ cd (change to HOME)
// it has to be executed and then return true
int cd(char* cmd) {
	//Your code here
    bool perform_cd = compare_first_word(cmd, "cd");
    int result_chdir;
    if (perform_cd){
        if (strlen(cmd) == 2){ //Hay que volver a home
            result_chdir = chdir("/home/");
        }
        else{
            char *directory = cmd + 3;
	        result_chdir = chdir(directory); 
        }
	    if (result_chdir == -1){
            printf("Couldn't change directory!!: %s\n", strerror(errno));
        }
        else{
            char *currDir = getCurrentDirectory();
            if (currDir == NULL){ //Hubo un error
                printf("Couldn't change directory!!: %s\n", strerror(errno));
                return true;
            }
            memset(promt, 0, sizeof promt); //limpio el prompt
            strncpy(promt, currDir, strlen(currDir));
            free(currDir);
        }
        return true;
    }
    return false;	
}

// returns true if 'pwd' was invoked
// in the command line
int pwd(char* cmd) {
	//Your code here
    bool perform_pwd = compare_first_word(cmd, "pwd");
    if (perform_pwd){
        char *currDir = getCurrentDirectory();
        if (currDir == NULL){ //Hubo un error
            printf("Couldn't change directory!!: %s\n", strerror(errno));
        }
        else{
            printf("%s\n",currDir);
        }
        return true;
    }
    return false;

}

