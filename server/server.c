//
// Created by student on 04.02.2020.
//

#include "server.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>
#include <arpa/inet.h>

void create_server(int port) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        print_error("wrong server initialization");

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_aton("127.0.0.1", (struct in_addr *) &server_addr.sin_addr.s_addr);
    bzero(&(server_addr.sin_zero), 8);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1)
        print_error("Unable to bind");

    make_nonblock(server_fd);
    struct type_of_epoll* toe = (struct type_of_epoll*)malloc(sizeof(struct type_of_epoll));
    toe->fd = server_fd;
    toe->type = SERVER;
    add_to_epoll(toe, EPOLLIN | EPOLLET);

    listen(server_fd, 5);
}

void accept_new_connection() {
    struct sockaddr_in cli_addr;
    socklen_t socklen = sizeof(cli_addr);
    int cli_fd = accept(server_fd, (struct sockaddr *) &cli_addr, &socklen);
    if (cli_fd == -1)
        print_error("accept new client");
    make_nonblock(cli_fd);
    struct type_of_epoll* toe = (struct type_of_epoll*)malloc(sizeof(struct type_of_epoll));
    toe->fd = cli_fd;
    toe->type = INET;
    add_to_epoll(toe, EPOLLIN | EPOLLET);
    printf("new client\n");
}

void process_inet_data(struct type_of_epoll *ptoe) {
    int fd = ptoe->fd;
    while(1) {
        struct sockaddr_un *local_addres = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
        if(read(fd, local_addres, sizeof(struct sockaddr_un))!=sizeof(struct sockaddr_un)) {
            free(local_addres);
            break;
        }
        int new_fd = socket(AF_LOCAL, SOCK_STREAM, 0); //don't know if SOCK_STREAM is good
        if (!connect(new_fd, (struct sockaddr *) local_addres, sizeof(struct sockaddr_un))) {
            make_nonblock(new_fd);
            struct type_of_epoll *toe = (struct type_of_epoll *) malloc(sizeof(struct type_of_epoll));
            toe->fd = new_fd;
            toe->type = LOCAL;
            toe->ptr = local_addres;
            add_to_epoll(toe, EPOLLIN | EPOLLET);
            write(fd, local_addres, sizeof(struct sockaddr_un));
            printf("connected to local server\n");
        } else {
            local_addres->sun_family = -1;
            write(fd, local_addres, sizeof(struct sockaddr_un));
            printf("couldn't connect to local server \n%s\n", local_addres->sun_path + 1);
        }
    }
}
