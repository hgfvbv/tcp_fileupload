#include "../public/tcpsocket.h"
#include "../public/file_transfer.h"

int main(int argc, const char *argv[])
{
	if(argc != 4){
		fprintf(stderr, "Usage : %s < ip > < port >.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int cfd;
	cfd = create_tcp_client_socket(argv[1], atoi(argv[2]));
	if(-1 == cfd)
		exit(EXIT_FAILURE);
	upload_file(argv[3], cfd);
	close(cfd);
	return 0;
}
