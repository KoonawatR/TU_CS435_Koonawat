/*
 A simple echo server program 
 Kasidit Chanchio (kasiditchanchio@gmail.com)
*/
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP		"127.0.0.1"
#define SERV_PORT 	18800

#define MAXLINE	100

int lis_fd;
int conn_fd;
struct sockaddr_in serv_addr;

int main(int argc, char *argv[]){

        int m, n;
        char line[MAXLINE];

	lis_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	bind(lis_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 

	listen(lis_fd, 5);

	if((conn_fd = accept(lis_fd, NULL, NULL)) < 0){
		printf("Accept: Error occured\n");
		exit(1);
	}

	while ((n = read(conn_fd, line, MAXLINE)) != 0){
                printf("line = %s with n = %d characters\n", line, n);
                fflush(stdout);
                m = write(conn_fd, line, n);
                printf("write line = %s for m = %d characters\n", line, m);
                fflush(stdout);
        }

	close(conn_fd);
	close(lis_fd);

}