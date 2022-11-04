#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (){
    int status;
    pid_t pid = fork();
    if (pid == -1) printf ("Failed to create child process\n");
    //execlp("ls","ls", NULL);
    
    if (pid == 0){
        printf ("Hello i am child: %d \n", (int) getpid());
        execlp("ls","ls", NULL);
    }
    else if (pid > 0){
        wait(&status);
        printf ("Hello i am parent: %d \n", (int) getpid());
        printf ("parent status: %d \n", status);
        printf ("child process PID: %d \n", (int) pid);
    }

    return 0;
}


/* These are just notes made during programming*/
/*
    wait() waits untill a child process exits and when it has exited
    it returns the PID of the exited child process. Returns -1 if wait failed.
*/

