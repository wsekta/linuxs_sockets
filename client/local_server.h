//
// Created by student on 04.02.2020.
//

#ifndef MULTIWRITER_LOCAL_SERVER_H
#define MULTIWRITER_LOCAL_SERVER_H

#include <sys/un.h>

int local_server_fd;
struct sockaddr_un local_server_address;
int local_sock_no;
int *local_sock_fds;

void create_local_server();

void accept_new_local_connection();

#endif //MULTIWRITER_LOCAL_SERVER_H
