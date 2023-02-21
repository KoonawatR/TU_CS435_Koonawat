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

#include <sys/select.h>

int lis_fd;
int conn_fd;
struct sockaddr_in serv_addr;

#define MAXCONN 100

int main(int argc, char *argv[]){

  int m, n, i, j, x, y;
  char tP1[5] = "cli-";
  char id[MAXLINE];
  char tP2[10] = " says: ";
  char concat[MAXLINE] = "";
  char line[MAXLINE];
	int conn_fd[MAXCONN]; 
	int cindex = 0; 

	fd_set base_rfds; // base read fd set
	fd_set rfds; // read fd set to be passed as a parameter of select() 
	int fdmax;

	lis_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	bind(lis_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 

	listen(lis_fd, 5);

	FD_ZERO(&base_rfds);
	FD_ZERO(&rfds);

	FD_SET(lis_fd, &base_rfds);
	fdmax = lis_fd;

	for(;;){
	  memcpy(&rfds, &base_rfds, sizeof(fd_set));
	  if(select(fdmax+1, &rfds, NULL, NULL, NULL) < 0){
	    printf("select error!\n");
	    fflush(stdout);
	    exit(1);
	  }
  
	  for(i = 0; i <= fdmax; i++){
	    if(FD_ISSET(i, &rfds)){
        if(i == lis_fd){
          if((conn_fd[cindex] = accept(lis_fd, NULL, NULL)) < 0){
            printf("Accept: Error occured\n");
            exit(1);
          }

          printf("a new connection %d is made!\n", conn_fd[cindex]);
          FD_SET(conn_fd[cindex] , &base_rfds);
          sprintf(id, "%d", (int)conn_fd[cindex]); // <- cast int to string (create id for client)
          x = write(conn_fd[cindex], id, MAXLINE); // <- send id to client
          if(conn_fd[cindex] > fdmax){
            fdmax = conn_fd[cindex];
          }
          cindex++;
        }
        else{
          n = read(i, line, MAXLINE);
          if (n <= 0){
            if(n == 0){
                printf("read: close connection %d\n", i);
          FD_CLR(i, &base_rfds);
          close(i);
          for(j =0; j <= cindex; j++){
            if (conn_fd[j] == i){
              conn_fd[j] = -1; 
            }
          }
            }
            else{
                printf("read: Error occured\n");
                exit(1);
            }
          }
          else{

            printf("on %d line = %s with n = %d characters\n", i, line, n);
            sprintf(id, "%d", i); // <- cast int to string (create id for client)
            strcat(concat,tP1); // <- concat string
            strcat(concat,id); // <- concat string
            strcat(concat,tP2); // <- concat string
            strcat(concat,line); // <- concat string
            fflush(stdout); // <- concat string

            for(j =0; j <= cindex; j++){
              if(conn_fd[j] != -1){
                  if (conn_fd[j] == i){
                    continue; // dont sent to sender
                  }
                  m = write(conn_fd[j], concat, n);
                  fflush(stdout);
              }
            }

            memset(concat, 0, sizeof(concat)); // <- set array to empty
            memset(id, 0, sizeof(id)); // <- set array to empty
          }
        }
	    }
	  }
	}
	close(lis_fd);

}
