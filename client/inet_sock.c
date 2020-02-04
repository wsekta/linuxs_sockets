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

void create_inet_socket()
{
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