//
// Created by wojtek on 03.02.2020.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define MAX_EPOLL_EVENTS 20

struct type_of_epoll {
    int fd;
    enum {
        SERVER, INET, LOCAL
    } type;
};

char *path_to_file;
int tcp_server_port;
int server_fd, epoll_fd;

inline void error(char *msg) {
    perror(msg);
    exit(-1);
}

void load_arguments(int argc, char *argv[]);

void create_server(int port);

void create_epoll();

void add_to_epoll(struct type_of_epoll toe, int type);

void make_nonblock(int fd);

void accept_new_connection();

void process_inet_data(int fd);

void process_local_data(int fd);

int main(int argc, char *argv[]) {
    load_arguments(argc, argv);

    create_epoll();
    create_server(tcp_server_port);

    struct epoll_event *events;
    events = calloc(MAX_EPOLL_EVENTS, sizeof(struct epoll_event));
    while (1) {
        int events_size = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        for (int i = 0; i < events_size; ++i) {
            if (events[i].events & EPOLLERR | events[i].events & EPOLLHUP | !(events[i].events & EPOLLIN)) {
                printf("client disconnected\n");
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

void create_epoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
        error("epoll_create1");
}

void create_server(int port) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error("wrong server initialization\n");

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1)
        error("Unable to bind");

    make_nonblock(server_fd);
    struct type_of_epoll toe;
    toe.fd = server_fd;
    toe.type = SERVER;
    add_to_epoll(toe, EPOLLIN | EPOLLET);

    listen(server_fd, 5);
}

void add_to_epoll(struct type_of_epoll toe, int type) {
    struct epoll_event epoll_ev;
    epoll_ev.events = type;
    epoll_ev.data.ptr = &toe;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, toe.fd, &epoll_ev);
}

void make_nonblock(int fd) {
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        error("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
        error("fcntl");
}

void accept_new_connection() {
    printf("accept debug\n");
    struct sockaddr_in cli_addr;
    socklen_t socklen = sizeof(cli_addr);
    int cli_fd = accept(server_fd, (struct sockaddr *) &cli_addr, &socklen);
    if (cli_fd == -1)
        error("accept new client");
    make_nonblock(cli_fd);
    struct type_of_epoll toe;
    toe.fd = cli_fd;
    toe.type = INET;
    add_to_epoll(toe, EPOLLIN | EPOLLET);
}

void process_inet_data(int fd) {
    struct sockaddr_un sockaddr_un;
    read(fd, &sockaddr_un, sizeof(sockaddr_un));
    //TODO check if struct is complete
    int new_fd = socket(AF_LOCAL, SOCK_STREAM, 0); //don't know if SOCK_STREAM is good
    if (connect(new_fd, (struct sockaddr *) &sockaddr_un, sizeof(sockaddr_un))) {
        make_nonblock(new_fd);
        struct type_of_epoll toe;
        toe.fd = new_fd;
        toe.type = LOCAL;
        add_to_epoll(toe, EPOLLIN | EPOLLET);
    } else {
        sockaddr_un.sun_family = -1;
        write(fd, &sockaddr_un, sizeof(sockaddr_un));
    }
}

void process_local_data(int fd) {
    printf("elo\n");
}

void load_arguments(int argc, char *argv[]) {
    if (argc <= 3)
        error("too few arguments\n");
    tcp_server_port = (int) strtol(argv[1], NULL, 10);
    char *p;
    int opt, argr = 0;
    while ((opt = getopt(argc, argv, "O:")) != -1) {
        if (opt == 'O') {
            path_to_file = (char *) malloc(strlen(optarg));
            if (path_to_file == NULL)
                error("alloc\n");
            strcpy(path_to_file, optarg);
            argr++;
        } else
            error("additional named argument\n");
    }
    if (optind >= argc || !argr)
        error("to few arguments\n");
    tcp_server_port = (int) strtol(argv[optind], &p, 10);
    if (*p != '\0')
        error("argument <port> is wrong\n");
    if (optind + 1 < argc)
        error("additional positional argument\n");
}