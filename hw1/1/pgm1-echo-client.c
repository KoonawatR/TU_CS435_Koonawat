/*
 A simple TCP client program 
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

#define	MAXLINE	100

int conn_fd;
struct sockaddr_in serv_addr;

int main(int argc, char *argv[]){

        int n, m;
        char line[MAXLINE];
        char line2[MAXLINE];

	conn_fd = (AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);

        if (connect(conn_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) { 
            perror("Problem in connecting to the server");
            exit(3);
        }

        while (fgets(line, MAXLINE, stdin) != NULL) {

            n = write(conn_fd, line, MAXLINE);
            m = read(conn_fd, line2, MAXLINE);
            printf("%s\n", line2);

        }

	close(conn_fd);

}