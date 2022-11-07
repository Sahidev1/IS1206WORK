/**
 * Author: Ali Sahibi
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define MAX_MSG_Q 10
#define MQ_NAME "/derp"

int main (int argc, char *argv[]){
    if (argc != 2) {
        printf("Please enter 1 argument: the path to the text file!\n");
        return -1;
    }
    char read_buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int fd;
    mqd_t mqd;
    char *file_path = argv[1];
    struct mq_attr attr;

    fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("open() failed ");
        return -1;
    }

    bytes_read = read(fd, read_buffer, BUFFER_SIZE);
    if (bytes_read == -1){
        perror("read() failed");
        return -1;
    }
    close(fd);

    mqd = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0660, NULL);
    if (mqd == -1){
        perror("mq_open() failed");
        return -1;
    }
    mq_send(mqd, read_buffer, strlen(read_buffer) , 0);
    mq_close(mqd);

    return 0;
} 