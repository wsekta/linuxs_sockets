//
// Created by wojtek on 03.02.2020.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
    if(argc <= 3)
    {
        printf("too few arguments\n");
        return -1;
    }

    int tcp_server_port;
    tcp_server_port = strtol(argv[1],NULL,10);

    int server_fd;
    if((server_fd=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        printf("wrong server initialization\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(tcp_server_port);
    server_addr.sin_addr.s_addr=INADDR_ANY;
    bind(server_fd,&server_addr, sizeof(struct sockaddr_in));

    while(1)
    {
        pause();
    }
    return 0;
}