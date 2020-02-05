//
// Created by student on 04.02.2020.
//
#include "tools.h"

#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>

void print_error(char *msg) {
    fprintf(stderr,"%s\n",msg);
    exit(-1);
}

void setup(int argc, char *argv[]) {
    if (argc <= 3)
        print_error("too few arguments");
    char *p;
    int opt, argr = 0;
    while ((opt = getopt(argc, argv, "O:")) != -1) {
        if (opt == 'O') {
            path_to_file = (char *) malloc(strlen(optarg));
            if (path_to_file == NULL)
                print_error("alloc");
            strcpy(path_to_file, optarg);
            argr++;
        } else
            print_error("additional named argument");
    }
    if (optind >= argc || !argr)
        print_error("to few arguments");
    tcp_server_port = (int) strtol(argv[optind], &p, 10);
    if (*p != '\0')
        print_error("argument <port> is wrong");
    if (optind + 1 < argc)
        print_error("additional positional argument");
    file_no=0;
    make_new_log_file();
    struct sigaction sa;
    sa.sa_handler=make_new_log_file;
    sigaction(SIGUSR1,&sa,NULL);
}

void make_new_log_file()
{
    char new_file_path[1024];
    do{
        close(file_fd);
        sprintf(new_file_path,"%s%03d", path_to_file, file_no++);
        file_fd = open(new_file_path,O_WRONLY|O_CREAT|O_TRUNC,0777);
    }while(file_fd == -1);
}

void create_epoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
        print_error("epoll_create1");
}

void add_to_epoll(struct type_of_epoll *toe, int type) {
    struct epoll_event epoll_ev;
    epoll_ev.events = type;
    epoll_ev.data.ptr = toe;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, toe->fd, &epoll_ev);
}

void make_nonblock(int fd) {
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        print_error("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
        print_error("fcntl");
}