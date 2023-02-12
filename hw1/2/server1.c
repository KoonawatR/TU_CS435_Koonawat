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
#define SERV_PORT2  18801

#define MAXLINE	100

int lis_fd;
int conn_fd,conn_fd2;
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
	
	//connect to server2
	int n2, m2, m3;
	char line2[MAXLINE];

	conn_fd2 = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT2);
	
	serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);

		if (connect(conn_fd2, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) { 
			perror("Problem in connecting to the server");
			exit(3);
		}
		while ((n = read(conn_fd, line2, MAXLINE)) != 0){
			n2 = write(conn_fd2, line2, MAXLINE);
			printf("send %s with n = %d characters\n", line2, n2);
			fflush(stdout);
			m2 = read(conn_fd2, line, MAXLINE);
			printf("receive %s with m = %d characters\n", line2, m2);
			fflush(stdout);

			m3 = write(conn_fd, line2, MAXLINE);

			fputs(line, stdout);
		}

		close(conn_fd2);

	printf("Client Close Connection\n");
	close(conn_fd);
	close(lis_fd);

}