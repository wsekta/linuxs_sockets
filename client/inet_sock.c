//
// Created by student on 04.02.2020.
//

#include "inet_sock.h"
#include "tools.h"
#include "local_server.h"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void create_inet_socket()
{
    error_no=0;
    accepted_no=0;
    if ((inet_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        print_error("wrong server initialization");
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(p_arg);
    inet_aton("127.0.0.1", (struct in_addr *) &server_addr.sin_addr.s_addr);
    bzero(&(server_addr.sin_zero), 8);
    if(connect(inet_sock_fd,(struct sockaddr *)&server_addr, sizeof(server_addr)))
        print_error("couldn't connect to server");
    make_nonblock(inet_sock_fd);
    add_to_epoll(inet_sock_fd,EPOLLIN | EPOLLET);
}

void send_local_arddress(){
    int i;
    for(i=0;i<S_arg;i++)
        write(inet_sock_fd,&local_server_address, sizeof(local_server_address));
}

void recv_from_server()
{
    while(1) {
        struct sockaddr_un *local_addres = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
        if (read(inet_sock_fd, local_addres, sizeof(struct sockaddr_un)) != sizeof(struct sockaddr_un))
            break;
        if(local_addres->sun_family == -1)
        {
            error_no++;
            //printf("server couldn't connect to local server\n");
            //do if error occurred
        } else {
            accepted_no++;
            //printf("working!\n");
            //do if error don't occurred
        }
    }
}

void close_inet_socket()
{
    if(close(inet_sock_fd))
        print_error("error in closing inet sock");
}