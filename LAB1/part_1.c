#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main (){
    const int BUFFER_SIZE = 2048;
    char write_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];
    int status;
    int fd[2]; //File descriptors declaration

    pipe(fd); //Creating the pipe.
    printf ("File descriptors: 0: %d, 1: %d \n", fd[0], fd[1]);

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        printf ("Failed to to fork\n");
        break;
    case 0: //CHILD
        close(fd[0]);
        dup2(fd[1], 1); //piping stdout to fd[1]
        close(fd[1]);
        execlp ("ls", "ls", "/", NULL);
        break;
    default: //PARENT
        close(fd[1]);

        wait(NULL);
        dup2(fd[0],0);
        close(fd[0]);
        //dprintf(fd[0], "hello");

        execlp("wc","wc","-l", NULL);
        //dprintf(fd[0], "hello");
        
        //read(fd[0], read_buffer, BUFFER_SIZE);

       // read(fd[0], read_buffer, BUFFER_SIZE);
        //printf("%s", read_buffer);

        break;
    }

   /* pid_t pid = fork();
    if (pid == -1) printf ("Failed to create child process\n");
    
    if (pid == 0){
        execlp("ls","ls", NULL);
    }
    else if (pid > 0){
        wait(&status);
    }
    */
    return 0;
}


/* These are just notes made during programming*/
/*
    wait() waits untill a child process exits and when it has exited
    it returns the PID of the exited child process. Returns -1 if wait failed.
*/

