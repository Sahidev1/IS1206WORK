/*
    Author: Ali Sahibi
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define ERROR 1

/**
 * This program gives you the number of filed in the "/" directory. 
 * The program creates a parent and child process. The child process executes "ls /" and the output is redirected to the stdin 
 * of the parent process which runs "wc -l". 
*/
int main (){
    int fd[2]; //File descriptors declaration
    if (pipe(fd) == -1){
        perror("pipe() failed");
        return ERROR;
    } //Creating the pipe.

    switch (fork())
    {
    case -1:
        perror("fork() failed");
        break;
    case 0: //CHILD
        close(fd[0]); //closing read end.

        //what would've gone to stdout goes to fd[1] instead, write end of the pipe.
        dup2(fd[1], 1); 

        if(execlp ("ls", "ls", "/", NULL) == -1) perror("child execlp() failed");
        break;
    default: //PARENT
        close(fd[1]); // closing write end.
        wait(NULL);

        //what woud've been read from stdin is instead read from fd[0], read end of the pipe.
        dup2(fd[0],0);

        close(fd[0]);
        if(execlp("wc","wc","-l", NULL) == -1) perror("parent execlp() failed");
        break;
    }
    return ERROR;
}


/* These are just notes made during programming*/
/*
    wait() waits untill a child process exits and when it has exited
    it returns the PID of the exited child process. Returns -1 if wait failed.
*/

