#include "../public/tcpsocket.h"
#include "../public/file_transfer.h"
#include "../public/debug.h"
#include <pthread.h>

void *do_task(void *arg)
{
	size_t size;
	int cfd = *(int*)arg;
	size = client_upload_file(cfd);
	printf("client upload file size : %ld\n", size);
	pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
	if(argc != 3){
		fprintf(stderr, "Usage : %s < ip > < port >.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int sfd, cfd, ret;
	struct sockaddr_in addr;
	pthread_t tid;
	sfd = create_tcp_server_socket(argv[1], atoi(argv[2]));
	if(-1 == sfd)
		exit(EXIT_FAILURE);	
	while(1)
	{
		bzero(&addr, sizeof(struct sockaddr_in));
		cfd = wait_for_connect(sfd, &addr);
		if(-1 == cfd)
			exit(EXIT_FAILURE); 
		show_tcp_network_address(&addr);
		ret = pthread_create(&tid, NULL, do_task, (void*)&cfd);
		if(ret != 0){
			DEBUG_INFO("[ERROR] : pthread_create() : %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		pthread_detach(tid);	
	}
	close(cfd);
	close(sfd);
	return 0;
}
