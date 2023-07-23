#ifndef __FILE_TRANSFER_H__
#define __FILE_TRANSFER_H__
#include "tcpsocket.h"
#include "debug.h"
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME_SZ 256
typedef struct file_protocol{
	size_t filesize;
	char filename[FILENAME_SZ];
}file_protocol_t;

extern int recv_protocol_head(const int cfd, file_protocol_t *p_head);
extern int recv_filedata(const int cfd, const char *filename, size_t targetsize);
extern int client_upload_file(const int cfd);
extern int send_protocol_head(const char *filename, const int sfd);
extern int upload_file(const char *filename, const int sfd);
#endif
