//
// Created by student on 04.02.2020.
//

#include "tools.h"
#include "local_server.h"
#include "timer_lib.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>

void create_local_server()
{
    local_sock_no=0;
    local_sock_fds = (int*)malloc(S_arg* sizeof(int));
    if((local_server_fd=socket(AF_LOCAL,SOCK_STREAM,0))==-1)
        print_error("error in creation local_server socket");
    int rand_fd = open("/dev/urandom",O_RDONLY);
    if(rand_fd==-1)
        print_error("error in open /dev/urandom");
    local_server_address.sun_family = AF_LOCAL;
    local_server_address.sun_path[0]=0;
    if(read(rand_fd,local_server_address.sun_path+1,107)!=107)
        print_error("error in read from /dev/urandom");
    if(close(rand_fd))
        print_error("urand close error");
    if(bind(local_server_fd,(struct sockaddr *)&local_server_address, sizeof(local_server_address))==-1)
        print_error("error while binding local_server socket");
    make_nonblock(local_server_fd);
    add_to_epoll(local_server_fd, EPOLLIN | EPOLLET);
    listen(local_server_fd,S_arg);
}

void accept_new_local_connection()
{
    if(local_sock_no==S_arg)
        print_error("to many connection in file");
    if((local_sock_fds[local_sock_no]=accept(local_server_fd,NULL,NULL))==-1)
        print_error("accept local connection");
    make_nonblock(local_sock_fds[local_sock_no++]);
}

void close_local_server()
{
    if(close(local_server_fd))
        print_error("error while closing af_local server");
}

void send_local_data() //3.wysyłanie komunikatów
{
    struct timespec ts_start;
    if(clock_gettime(CLOCK_REALTIME,&ts_start))
        print_error("clock_gettime");
    int sock_no = rand()%local_sock_no;
    char *repr = time_repr(ts_start);
    write(local_sock_fds[sock_no],repr, sizeof(repr));
    write(local_sock_fds[sock_no],&local_server_address, sizeof(local_server_address));
    write(local_sock_fds[sock_no],&ts_start, sizeof(ts_start));
    struct timespec ts_end;
    if(clock_gettime(CLOCK_REALTIME,&ts_end))
        print_error("clock_gettime");
    add_to_summary(ts_start,ts_end);
}

void remove_from_working_sockets(int fd)
{
    int i=0;
    while(i<local_sock_no)
        if(local_sock_fds[i++]==fd)
            break;
    if(i==local_sock_no)
        return;
    close(local_sock_fds[--i]);
    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,local_sock_fds[i],NULL);
    local_sock_fds[i]=local_sock_fds[--local_sock_no];
}