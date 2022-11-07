/**
 * Author: Ali Sahibi
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>

#define BUFFER_SIZE (2 << 14)
#define MAX_MSG_Q 10
#define MSG_QUEUE_NAME "/msg"

/*void init_mq_attr (struct mq_attr *attr, long maxmsg, long maxsize){
    attr->mq_maxmsg = maxmsg;
    attr->mq_msgsize = maxsize;
}*/

int main (int argc, char *argv[]){
    if (argc != 2) {
        printf("Please enter 1 argument: the path to the text file!\n");
        return -1;
    }
    char read_buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int fd;
    char *file_path = argv[1];
    struct mq_attr attr;

    fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("open() failed ");
        return -1;
    }

    bytes_read = read(fd, read_buffer, BUFFER_SIZE);

    init_mq_attr(&attr, 10, bytes_read);
    mq_open(MSG_QUEUE_NAME, O_WRONLY | O_CREAT, &attr);
    
    printf("endofcode\n");
    return 0;
} 