//
// Created by student on 04.02.2020.
//

#include "tools.h"
#include "timer_lib.h"
#include "local_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

void print_error(char *msg) {
    fprintf(stderr,"%s\n",msg);
    exit(-1);
}

void setup(int argc, char **argv) {
    srand(time(NULL));
    char *p;
    int opt;
    int S_flag =0,p_flag =0,d_flag=0,T_flag=0;
    while ((opt = getopt(argc, argv, "S:p:d:T:")) != -1) {
        switch (opt)
        {
            case 'S':
                S_arg=(int)strtol(optarg,&p,10);
                if(S_arg < 0 || *p !='\0')
                    print_error("wrong S argument");
                else
                    S_flag=1;
                break;
            case 'p':
                p_arg=(int)strtol(optarg,&p,10);
                if(p_arg < 0 || p_arg > 65535 || *p !='\0')
                    print_error("wrong p argument");
                else
                    p_flag=1;
                break;
            case 'd':
                d_arg=strtof(optarg,&p);
                if(d_arg < 0 || *p !='\0')
                    print_error("wrong d argument");
                else
                    d_flag=1;
                break;
            case 'T':
                T_arg=strtof(optarg,&p);
                if(T_arg < 0 || *p !='\0')
                    print_error("wrong T argument");
                else
                    T_flag=1;
                break;
            default:
                print_error("additional argument(s)");
                break;
        }
    }
    if(!S_flag|!p_flag|!d_flag|!T_flag)
        print_error("some argument(s) wasn't specified");

    create_epoll();
    preprocess_time();
}

void create_epoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
        print_error("epoll_create1");
}

void add_to_epoll(int fd, int type) {
    struct epoll_event epoll_ev;
    epoll_ev.events = type;
    epoll_ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &epoll_ev);
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

void do_at_end()
{
    printf("%s\n",time_repr(summary_time));
    free(local_sock_fds);
    exit(0);
}