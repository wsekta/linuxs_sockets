//
// Created by student on 04.02.2020.
//

#ifndef PWSL03_CLI_TOOLS_H
#define PWSL03_CLI_TOOLS_H

int S_arg,p_arg;
float d_arg,T_arg;
int epoll_fd;

void print_error(char *msg);
void setup(int argc, char **argv);

void create_epoll();
void add_to_epoll(int fd, int type);
void make_nonblock(int fd);

#endif //PWSL03_CLI_TOOLS_H
