/**
 * Author: Ali Sahibi
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048
#define MAX_MSG_Q 10
#define MQ_NAME "/derp"
#define ERR_CODE 1

int main(){
    struct mq_attr attr;
    mqd_t mqd;
    unsigned int prio = 0;
    int fd[2];
    int pipestatus;
    char* msg_buffer;

    if (pipe(fd) == -1){
        perror("pipe() failed");
        return ERR_CODE;
    }

    switch (fork())
    {
    case -1:
        perror("fork() failed");
        break;
    case 0:
        close(fd[0]);

        mqd = mq_open (MQ_NAME, O_RDONLY | O_NONBLOCK);
        if (mqd == -1){
            perror("mq_open() failed");
            return ERR_CODE;
        }
        mq_getattr(mqd, &attr);
        msg_buffer = calloc(attr.mq_msgsize, 1);
        if(mq_receive(mqd, msg_buffer, attr.mq_msgsize, &prio) == -1){
            free(msg_buffer);
            mq_close(mqd);
            perror("mq_receive() failed");
            return ERR_CODE;
        }
        mq_close(mqd);
        
        write(fd[1], msg_buffer, strlen(msg_buffer) + 1);
        free(msg_buffer);
        printf("Number of words: ");
        break;
    default:
        close(fd[1]);
        wait(NULL);

        dup2(fd[0], 0);
        close (fd[0]);
        if(execlp("wc","wc","-w", NULL) == -1) {
            perror ("exelcp() failed");
            return ERR_CODE;
        }
        break;
    }
    return 0;
}