#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>

#include <stdlib.h>

#define BUFFER_SIZE 2048
#define MAX_MSG_Q 10

void init_mq_attr (struct mq_attr *attr, long maxmsg, long maxsize){
    attr->mq_maxmsg = maxmsg;
    attr->mq_msgsize = maxsize;
}

int main(){
    struct mq_attr attr;
    mqd_t mqd;
    unsigned int prio = 0;
    char *msgname = "/ders0";

    mqd = mq_open (msgname, O_RDONLY);
    mq_getattr(mqd, &attr);

    char *msg_buffer = calloc(attr.mq_msgsize, 1);

    mq_receive(mqd, msg_buffer, attr.mq_msgsize, &prio);
    printf ("Message: %s\n", msg_buffer);

    mq_close(mqd);

    printf ("endofcode\n");
    return 0;
}