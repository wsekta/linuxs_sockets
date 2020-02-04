//
// Created by wojtek on 03.02.2020.
//

#include "tools.h"
#include "server.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <string.h>

#define MAX_EPOLL_EVENTS 20

void process_local_data(int fd);

int main(int argc, char *argv[]) {
    setup(argc, argv);

    create_epoll();
    create_server(tcp_server_port);

    struct epoll_event *events;
    events = calloc(MAX_EPOLL_EVENTS, sizeof(struct epoll_event));
    while (1) {
        int events_size = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        for (int i = 0; i < events_size; ++i) {
            if ((events[i].events & EPOLLERR) | (events[i].events & EPOLLHUP) | !(events[i].events & EPOLLIN)) {
                close(((struct type_of_epoll *) (events[i].data.ptr))->fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ((struct type_of_epoll *) (events[i].data.ptr))->fd, NULL);
            } else {
                switch (((struct type_of_epoll *) (events[i].data.ptr))->type) {
                    case SERVER:
                        accept_new_connection();
                        break;
                    case INET:
                        process_inet_data(((struct type_of_epoll *) (events[i].data.ptr))->fd);
                        break;
                    case LOCAL:
                        process_local_data(((struct type_of_epoll *) (events[i].data.ptr))->fd);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void process_local_data(int fd) {
    printf("elo\n");
}