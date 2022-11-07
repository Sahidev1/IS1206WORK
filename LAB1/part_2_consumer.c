#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>

#include <stdlib.h>

#define BUFFER_SIZE 100
#define MAX_MSG_Q 1024

void init_mq_attr (struct mq_attr *attr, long maxmsg, long maxsize){
    attr->mq_maxmsg = maxmsg;
    attr->mq_msgsize = maxsize;
}

int main(){
    struct mq_attr attr;
    char *msg_buffer = (char *) malloc (BUFFER_SIZE);
    mqd_t mqd;
    //mode_t RW_MODE = 0666;
    char *msgname = "/ghx";

    init_mq_attr (&attr, MAX_MSG_Q, BUFFER_SIZE);
    mqd = mq_open (msgname, O_RDONLY);

    mq_receive(mqd, msg_buffer, MAX_MSG_Q, NULL);
    printf ("Message: %s\n", msg_buffer);

    mq_close(mqd);

    printf ("endofcode\n");
    return 0;
}