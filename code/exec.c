#include "exec.h"
#include "utils.h"
#include "stdbool.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
// sets the "key" argument with the key part of
// the "arg" argument and null-terminates it
static void get_environ_key(char* arg, char* key) {

	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets the "value" argument with the value part of
// the "arg" argument and null-terminates it
static void get_environ_value(char* arg, char* value, int idx) {

	int i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables passed
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void set_environ_vars(char** eargv, int eargc) {

	// Your code here
} 

// opens the file in which the stdin/stdout or
// stderr flow will be redirected, and returns
// the file descriptor
// 
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int open_redir_fd(char* file) {

	return -1;
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
void exec_cmd(struct cmd* cmd) {

	switch (cmd->type) {

		case EXEC:;
			// spawns a command
			//
			// Your code here
            bool set_env = setEnviromentVariables(cmd->scmd);
            if (set_env){ //Si se seteo la variable de entorno, salgo
                free_command(cmd);
                exit(0);
            }
			struct execcmd* e;
			e = (struct execcmd*)cmd;
			int result = execvp(e->argv[0], e->argv);
            if (result == -1){
                printf("Couldn't execute the program!: %s\n", strerror(errno));
                _exit(1);
            }
			break;

		case BACK: {
			struct backcmd* backcmd = (struct backcmd*) cmd;
			exec_cmd(backcmd->c);
			break;
		}

		case REDIR: {
			// changes the input/output/stderr flow
			//
			// Your code here
            struct execcmd* e;
            int result;
            int fd_out = -1, fd_in = -1, fd_err= -1;
			e = (struct execcmd*)cmd;  
      
            if (strlen(e->out_file) != 0){
               
                int mode = S_IRWXU|S_IRWXG;
                if (e->out_file[0] == '>'){ //operador >>
                    fd_out = open(e->out_file+1, O_CREAT|O_APPEND|O_WRONLY, mode);
                }
                else{
                    fd_out = open(e->out_file, O_CREAT|O_TRUNC|O_WRONLY, mode);
                }
                if (fd_out == -1){
                    printf("Couldn't open the file!: %s\n", strerror(errno));
                    _exit(1);
                }
                dup2(fd_out, 1);
            }

            if (strlen(e->in_file) != 0){
                fd_in = open(e->in_file, O_RDONLY, S_IRWXU|S_IRWXG);
                if (fd_in == -1){
                    printf("Couldn't open the file!: %s\n", strerror(errno));
                    _exit(1);
                }
                dup2(fd_in, 0);                
            }

            if (strlen(e->err_file) != 0){

                if (e->err_file[0] == '&'){
                    //Para que funcione 2>&1
                    fd_err = ((int)e->err_file[1]) - 48;
                    dup2(fd_err, 2);
                }
                else{
                    fd_err = open(e->err_file, O_CREAT|O_TRUNC|O_WRONLY, S_IRWXU|S_IRWXG);
                    if (fd_err == -1){
                        printf("Couldn't open the file!: %s\n", strerror(errno));
                        _exit(1);
                    }
                }
                dup2(fd_err, 2);
            }
            result = execvp(e->argv[0], e->argv);
            if (fd_out != -1){
                close(fd_out);
                close(1);
            }
            if (fd_in != -1){
                close(fd_in);
                close(0);
            }
            if (fd_err != -1){
                close(fd_err);
                close(2);
            }

            if (result == -1){
                printf("Couldn't execute the program!: %s\n", strerror(errno));
                _exit(1);
            }
			exit(0);
			break;
		}
		
		case PIPE: {
			// pipes two commands
			//
			// Your code here
		    int pipefd[2];
            pid_t cpid;
            int status = 0;
            struct pipecmd* cmd_p = (struct pipecmd*)cmd;
            if (pipe(pipefd) == -1) {
               printf("pipe error: %s\n", strerror(errno));
               exit(EXIT_FAILURE);
           }

           cpid = fork();
           if (cpid == -1) {
               printf("fork error:: %s\n", strerror(errno));
               exit(EXIT_FAILURE);
           }

           if (cpid == 0) {    /* Child reads from pipe */
               close(pipefd[0]);
			   if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
					printf("pipe dup2: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
               
               exec_cmd(cmd_p->leftcmd);
               
               _exit(EXIT_SUCCESS);
           } 
           else {            

               //No es necesario un waitpid() ya que la lectura del pipe 
               //bloquea al proceso de manera automatica si intenta leer
               //del pipe y este aun no ha sido escrito

               close(pipefd[1]);          /* Reader will see EOF */
               if (dup2(pipefd[0], STDIN_FILENO) == -1) {
					printf("pipe dup2: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
               
               exec_cmd(cmd_p->rightcmd);

               exit(EXIT_SUCCESS);
           }

			// free the memory allocated
			// for the pipe tree structure
			free_command(parsed_pipe);
			break;
		}
	}
}
