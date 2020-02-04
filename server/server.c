//
// Created by student on 04.02.2020.
//

#include "server.h"
#include "tools.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

void create_server(int port) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        print_error("wrong server initialization");

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1)
        print_error("Unable to bind");

    make_nonblock(server_fd);
    struct type_of_epoll toe;
    toe.fd = server_fd;
    toe.type = SERVER;
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
    struct type_of_epoll toe;
    toe.fd = cli_fd;
    toe.type = INET;
    add_to_epoll(toe, EPOLLIN | EPOLLET);
}

void process_inet_data(int fd) {
    struct sockaddr_un *sockaddr_un = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
    read(fd, sockaddr_un, sizeof(struct sockaddr_un));
    //TODO check if struct is complete
    int new_fd = socket(AF_LOCAL, SOCK_STREAM, 0); //don't know if SOCK_STREAM is good
    if (connect(new_fd, (struct sockaddr *) &sockaddr_un, sizeof(sockaddr_un))) {
        make_nonblock(new_fd);
        struct type_of_epoll toe;
        toe.fd = new_fd;
        toe.type = LOCAL;
        toe.ptr = sockaddr_un;
        add_to_epoll(toe, EPOLLIN | EPOLLET);
    } else {
        sockaddr_un->sun_family = -1;
        write(fd, sockaddr_un, sizeof(struct sockaddr_un));
    }
}