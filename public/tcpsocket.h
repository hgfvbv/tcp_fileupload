#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define BACKLOG 10
extern int create_tcp_server_socket(const char *ip, const unsigned short port);
extern int create_tcp_client_socket(const char *ip, const unsigned short port);
extern int wait_for_connect(const int sfd, struct sockaddr_in *addr);
extern void show_tcp_network_address(const struct sockaddr_in *addr);
extern ssize_t tcp_send_pack(const int sfd, const void *buf, size_t len);
extern ssize_t tcp_recv_pack(const int sfd, void *buf, size_t len);
#endif
