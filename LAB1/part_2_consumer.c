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

void init_mq_attr (struct mq_attr *attr, long maxmsg, long maxsize){
    attr->mq_maxmsg = maxmsg;
    attr->mq_msgsize = maxsize;
}

int main(){
    struct mq_attr attr;
    mqd_t mqd;
    unsigned int prio = 0;
    int fd[2];
    int pipestatus;
    char* msg_buffer;

    pipestatus = pipe(fd);

    switch (fork())
    {
    case -1:
        perror("fork() failed");
        break;
    case 0:
        close(fd[0]);

        mqd = mq_open (MQ_NAME, O_RDONLY);
        mq_getattr(mqd, &attr);
        msg_buffer = calloc(attr.mq_msgsize, 1);
        mq_receive(mqd, msg_buffer, attr.mq_msgsize, &prio);
        mq_close(mqd);
        
        write(fd[1], msg_buffer, strlen(msg_buffer) + 1);
        break;
    default:
        close(fd[1]);
        wait(NULL);

        dup2(fd[0], 0);
        close (fd[0]);
        execlp("wc","wc","-w", NULL);
        break;
    }

    /*
    mqd = mq_open (MQ_NAME, O_RDONLY);
    mq_getattr(mqd, &attr);

    msg_buffer = calloc(attr.mq_msgsize, 1);

    mq_receive(mqd, msg_buffer, attr.mq_msgsize, &prio);
    printf ("Message: %s\n", msg_buffer);

    mq_close(mqd);

    printf ("endofcode\n");

    */
    return 0;
}