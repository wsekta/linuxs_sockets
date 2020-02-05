#ifndef MULTIWRITER_INET_SOCK_H
#define MULTIWRITER_INET_SOCK_H

int inet_sock_fd;
int error_no,accepted_no;

void create_inet_socket();

void send_local_arddress();

void recv_from_server();

void close_inet_socket();

#endif //MULTIWRITER_INET_SOCK_H
