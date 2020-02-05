//
// Created by wojtek on 03.02.2020.
//

#include "tools.h"
#include "server.h"
#include "time_lib.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <string.h>
#include <time.h>

#define MAX_EPOLL_EVENTS 20

void process_local_data(struct type_of_epoll *ptoe);

int main(int argc, char *argv[]) {
    setup(argc, argv);

    create_epoll();
    create_server(tcp_server_port);

    struct epoll_event *events;
    events = (struct epoll_event *) malloc(MAX_EPOLL_EVENTS * sizeof(struct epoll_event));
    while (1) {
        int events_size = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        for (int i = 0; i < events_size; ++i) {
            if ((events[i].events & EPOLLERR) | (events[i].events & EPOLLHUP) | !(events[i].events & EPOLLIN)) {
                close(((struct type_of_epoll *) (events[i].data.ptr))->fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ((struct type_of_epoll *) (events[i].data.ptr))->fd, NULL);
                free(events[i].data.ptr);
            } else {
                switch (((struct type_of_epoll *) (events[i].data.ptr))->type) {
                    case SERVER:
                        accept_new_connection();
                        break;
                    case INET:
                        process_inet_data((struct type_of_epoll *) events[i].data.ptr);
                        break;
                    case LOCAL:
                        process_local_data((struct type_of_epoll *) events[i].data.ptr);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void process_local_data(struct type_of_epoll *ptoe) {
    struct timespec ts,ts_remote;
    struct sockaddr_un addrun;
    int fd = ptoe->fd;
    int i=0;
    char c='y';
    char statement[26];
    char addr[108];
    char statement_to_file[128]="";
    while(1==read(fd,&c,1)&&i<26&&c!=0)
        statement[i++]=c;
    statement[i]=0;
    if(sizeof(struct sockaddr_un)!=read(fd,&addrun, sizeof(struct sockaddr_un)))
        return;
    while(i<108)
        if(addrun.sun_path[i]!=ptoe->ptr->sun_path[i++])
            return;
    if(sizeof(struct timespec)!=read(fd,&ts_remote, sizeof(struct timespec)))
        return;
    if(clock_gettime(CLOCK_REALTIME,&ts))
        print_error("clock_gettime");
    strcat(statement_to_file,time_repr(ts));
    strcat(statement_to_file," : ");
    strcat(statement_to_file,statement);
    strcat(statement_to_file," : ");
    strcat(statement_to_file,time_repr(ts_sub(ts,ts_remote)));
    i=0;
    while(statement_to_file[i++]!=0);
    i--;
    write(file_fd,statement_to_file,i);
    write(file_fd,"\n",1);
}