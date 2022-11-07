/*
    Author: Ali Sahibi
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main (){
    int fd[2]; //File descriptors declaration
    int pipestatus = pipe(fd); //Creating the pipe.
    pid_t pid = fork();
    //printf ("File descriptors: 0: %d, 1: %d \n", fd[0], fd[1]);
    switch (pid)
    {
    case -1:
        printf ("Failed to to fork\n");
        break;
    case 0: //CHILD
        close(fd[0]); //closing read end.

        //what would've gone to stdout goes to fd[1] instead, write end of the pipe.
        dup2(fd[1], 1); 

        execlp ("ls", "ls", "/", NULL);
        break;
    default: //PARENT
        close(fd[1]); // closing write end.
        wait(NULL);

        //what woud've been read from stdin is instead read from fd[0], read end of the pipe.
        dup2(fd[0],0);

        close(fd[0]);
        execlp("wc","wc","-l", NULL);
        break;
    }
    return 0;
}


/* These are just notes made during programming*/
/*
    wait() waits untill a child process exits and when it has exited
    it returns the PID of the exited child process. Returns -1 if wait failed.
*/

