//
// Created by wojtek on 04.02.2020.
//
#include "tools.h"
#include "local_server.h"
#include "inet_sock.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>

#define MAX_EPOLL_EVENTS 20

int main(int argc,char* argv[])
{
    setup(argc, argv);
    create_local_server();
    create_inet_socket();
    send_local_arddress();

    struct epoll_event *events;
    events = (struct epoll_event *)malloc(MAX_EPOLL_EVENTS * sizeof(struct epoll_event));
    while(1)
    {
        int events_size = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        for (int i = 0; i < events_size; ++i) {
            if ((events[i].events & EPOLLERR) | (events[i].events & EPOLLHUP) | !(events[i].events & EPOLLIN)) {
                close(events[i].data.fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
            } else {
                if(events[i].data.fd == local_server_fd){
                    accept_new_local_connection();
                } else if(events[i].data.fd == inet_sock_fd){
                    recv_from_server();
                } else {
                    //TODO
                }
            }
        }
        if(error_no+accepted_no == S_arg)
            break;
    }

}