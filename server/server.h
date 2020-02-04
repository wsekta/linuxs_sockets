//
// Created by student on 04.02.2020.
//

#ifndef PWSL03_SERVER_H
#define PWSL03_SERVER_H

#include "tools.h"

void create_server(int port);

void accept_new_connection();

void process_inet_data(struct type_of_epoll *ptoe);

#endif //PWSL03_SERVER_H
