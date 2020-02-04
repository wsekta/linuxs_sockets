//
// Created by student on 04.02.2020.
//

#include "tools.h"
#include "local_server.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>

void create_local_server()
{
    if((local_server_fd=socket(AF_LOCAL,SOCK_STREAM,0))==-1)
        print_error("error in creation local_server socket");
    int rand_fd = open("/dev/urandom",O_RDONLY);
    if(rand_fd==-1)
        print_error("error in open /dev/urandom");
    local_server_address.sun_family = AF_LOCAL;
    local_server_address.sun_path[0]=0;
    if(read(rand_fd,local_server_address.sun_path+1,107)!=107)
        print_error("error in read from /dev/urandom");
    close(rand_fd);
    if(bind(local_server_fd,(struct sockaddr *)&local_server_address, sizeof(local_server_address))==-1)
        print_error("error while binding local_server socket");
    make_nonblock(local_server_fd);
    add_to_epoll(local_server_fd, EPOLLIN | EPOLLET);
    listen(local_server_fd,S_arg);
}