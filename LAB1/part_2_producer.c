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
#define ERR_CODE 1

int main (int argc, char *argv[]){
    if (argc != 2) {
        printf("Please enter 1 argument: the path to the text file!\n");
        return ERR_CODE;
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
        return ERR_CODE;
    }

    bytes_read = read(fd, read_buffer, BUFFER_SIZE);
    if (bytes_read == -1){
        perror("read() failed");
        return ERR_CODE;
    }
    close(fd);

    mqd = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0660, NULL);
    if (mqd == -1){
        perror("mq_open() failed");
        return ERR_CODE;
    }
    if(mq_send(mqd, read_buffer, strlen(read_buffer) , 0) == -1){
        perror("mq_send() failed");
        return ERR_CODE;
    }
    mq_close(mqd);

    return 0;
} 