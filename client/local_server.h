//
// Created by student on 04.02.2020.
//

#ifndef MULTIWRITER_LOCAL_SERVER_H
#define MULTIWRITER_LOCAL_SERVER_H

#include <sys/un.h>

int local_server_fd;
struct sockaddr_un local_server_address;

void create_local_server();

#endif //MULTIWRITER_LOCAL_SERVER_H
