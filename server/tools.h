//
// Created by student on 04.02.2020.
//

#ifndef PWSL03_TOOLS_H
#define PWSL03_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>

struct type_of_epoll {
    int fd;
    enum {
        SERVER, INET, LOCAL
    } type;
    struct sockaddr_un* ptr;
};

//globals
char *path_to_file;
int file_no;
int tcp_server_port;
int epoll_fd, file_fd;

void print_error(char *msg);

void setup(int argc, char *argv[]);

void make_new_log_file();

void create_epoll();

void add_to_epoll(struct type_of_epoll *toe, int type);

void make_nonblock(int fd);

#endif //PWSL03_TOOLS_H
