#include "file_transfer.h"

int recv_protocol_head(const int cfd, file_protocol_t *p_head)
{
	ssize_t rbytes;
	ssize_t total_received = 0;
	char *buf = (char*)p_head;
	while(1)
	{
		rbytes = tcp_recv_pack(cfd, buf + total_received, sizeof(file_protocol_t) - total_received);
		if(-1 == rbytes){
			DEBUG_INFO("[ERROR] : %s\n", strerror(errno));
			return -1;
		}else if(0 == rbytes){
			DEBUG_INFO("[INFO] : The client has been shut down.\n");
			break;
		}else if(rbytes > 0){
			total_received += rbytes;
			if(sizeof(file_protocol_t) == total_received)
				break;
		}
	}
	if(total_received != sizeof(file_protocol_t)){
		DEBUG_INFO("[ERROR] : Failed to receive data.\n");
		return -1;
	}
	return 0;
}

int recv_filedata(const int cfd, const char *filename, size_t targetsize)
{
	int fd;
	ssize_t rbytes = 0, wbytes = 0, file_size = 0;
	char buf[1024] = {0};
	DEBUG_INFO("[INFO] : filename %s\n", filename);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(-1 == fd){
		DEBUG_INFO("[ERROR] : Failed to open filename.\n");
		return -1;
	}
	while(1)
	{
		rbytes = tcp_recv_pack(cfd, buf, sizeof(buf));
		if(-1 == rbytes){
			DEBUG_INFO("[ERROR] : %s\n", strerror(errno));
			return -1;
		}else if(0 == rbytes){
			DEBUG_INFO("[INFO] : The client has been shut down.\n");
			break;
		}else if(rbytes > 0){
			wbytes = write(fd, buf, rbytes);
			if(wbytes != rbytes){
				DEBUG_INFO("[ERROR] : Failed to write data.\n");
				return -1;
			}
			file_size += rbytes;
			if(file_size == targetsize)
				break;
		}	
	}
	close(fd);
	return file_size;
}

int client_upload_file(const int cfd)
{
	int ret;
	char *filename;
	size_t filesize = 0, recvsize = 0;
	file_protocol_t head;
	ret = recv_protocol_head(cfd, &head);
	if(-1 == ret)
		return -1;
	filename = head.filename;
	filesize = head.filesize;
	recvsize = recv_filedata(cfd, filename, filesize);
	printf("recvsize = %ld\n", recvsize);
	return recvsize;
}

int send_protocol_head(const char *filename, const int sfd)
{
	int fd;
	int filesize;
	int ret;
	file_protocol_t head;
	fd = open(filename, O_RDONLY);
	if(-1 == fd){
		DEBUG_INFO("[ERROR] : Failed to open filename.\n");
		return -1;
	}
	filesize = lseek(fd, 0, SEEK_END);
	close(fd);
	head.filesize = filesize;
	strcpy(head.filename, filename);
	ret = tcp_send_pack(sfd, &head, sizeof(head));
	if(ret != sizeof(file_protocol_t)){
		DEBUG_INFO("[ERROR] : Failed to send protocol head.\n");
		return -1;
	}
	return filesize;
}

int upload_file(const char *filename, int sfd)
{
	int file_size, upload_size, fd;
	ssize_t rbytes = 0, sbytes = 0;
	char buf[1024] = {0};
	file_size = send_protocol_head(filename, sfd);
	if(file_size < 0)
		return -1;
	fd = open(filename, O_RDONLY);
	if(-1 == fd){
		DEBUG_INFO("[ERROR] : Failed to open filename.\n");
		return -1;
	}
	while(1)
	{
		rbytes = read(fd, buf, sizeof(buf));
		if(-1 == rbytes){
			DEBUG_INFO("[ERROR] : Failed to read data.\n");
			return -1;
		}
		sbytes = tcp_send_pack(sfd, buf, rbytes);
		if(sbytes != rbytes){
			DEBUG_INFO("[ERROR] : Failed to send data.\n");
			return -1;
		}
		upload_size += rbytes;	
	}
	close(fd);
	return upload_size;
}
