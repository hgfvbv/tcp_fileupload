#include "tcpsocket.h"
#include "debug.h"

int create_tcp_server_socket(const char *ip, const unsigned short port)
{
	int sfd, ret;
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd){
		DEBUG_INFO("[ERROR] : socket() : %s\n", strerror(errno));
		return -1;
	}
	bzero(&addr, len);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	if(bind(sfd, (const struct sockaddr*)&addr, len) == -1){
		DEBUG_INFO("[ERROR] : bind() : %s\n", strerror(errno));
		return -1;
	}
	ret = listen(sfd, BACKLOG);
	if(-1 == ret){
		DEBUG_INFO("[ERROR] : listen() : %s\n", strerror(errno));
		return -1;
	}
	return sfd;
}

int create_tcp_client_socket(const char *ip, const unsigned short port)
{
	int sfd, ret;
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd){
		DEBUG_INFO("[ERROR] : socket() : %s\n", strerror(errno));
		return -1;
	}
	bzero(&addr, len);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	ret = connect(sfd, (const struct sockaddr*)&addr, len);
       	if(-1 == ret){
		DEBUG_INFO("[ERROR] : connect() : %s\n", strerror(errno));
		return -1;
	}
	DEBUG_INFO("[INFO] : Connect %s successded.\n", ip);
	return sfd;	
}

int wait_for_connect(const int sfd, struct sockaddr_in *addr)
{
	int cfd;
	socklen_t len = sizeof(struct sockaddr_in);
	cfd = accept(sfd, (struct sockaddr*)addr, &len);
	if(-1 == cfd){
		DEBUG_INFO("[ERROR] : %s\n", strerror(errno));
		return -1;
	}
	return cfd;
}

void show_tcp_network_address(const struct sockaddr_in *addr)
{
	printf("ip : < %s > port : < %d >\n", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
}

ssize_t tcp_send_pack(const int sfd, const void *buf, size_t len)
{
	return send(sfd, buf, len, 0);
}

ssize_t tcp_recv_pack(const int sfd, void *buf, size_t len)
{
	return recv(sfd, buf, len, 0);
}
