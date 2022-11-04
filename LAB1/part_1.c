#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (){
    int status;
    pid_t pid = fork();
    if (pid == -1) printf ("Failed to create child process\n");
    
    if (pid == 0){
        execlp("ls","ls", NULL);
    }
    else if (pid > 0){
        wait(&status);
    }

    return 0;
}


/* These are just notes made during programming*/
/*
    wait() waits untill a child process exits and when it has exited
    it returns the PID of the exited child process. Returns -1 if wait failed.
*/

