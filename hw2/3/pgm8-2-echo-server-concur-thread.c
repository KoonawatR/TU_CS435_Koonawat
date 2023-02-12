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
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define SERV_IP		"127.0.0.1"
#define SERV_PORT 	18800
#define MAXLINE	100
#define MAX_CONNECTIONS	100

int lis_fd;
int conn_fd[MAX_CONNECTIONS];
pthread_t tid[MAX_CONNECTIONS];

int numtid = 0;
struct sockaddr_in serv_addr;

void process_request(void *t);

int main(int argc, char *argv[]){

        int n, cid;
	lis_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	bind(lis_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 

	listen(lis_fd, 5);

        cid = 0; 

        while(1){
          int tmp_fd;

          if(cid == MAX_CONNECTIONS){
            printf("Exceed maximum connections\n"); 
            break;
          }

	  if((conn_fd[cid] = accept(lis_fd, NULL, NULL)) < 0){
		printf("Accept: Error occured\n");
		exit(1);
	  }
          tmp_fd = conn_fd[cid];

          if((n = pthread_create(&tid[cid], NULL, (void *) &process_request, (void *)((long)tmp_fd))) != 0){
		printf("Pthread create error\n");
		exit(1);

          }
          printf("main thread just create cid = %d tid = %ld\n", cid, tid[cid]);
          cid++;
          

        }

	close(lis_fd);

}

void process_request(void *t){
    int n, m;
    char line[MAXLINE];
    FILE *fptr; //3.4 declare a pointer of type file
    int tmp_fd = (int)((long) t); 

    pthread_t mythrid = pthread_self(); 
    pthread_detach(mythrid); 

    printf("tid = %ld:  tmp_fd = %d \n", mythrid, tmp_fd);
    char filename[256];
    sprintf(filename, "%ld.txt", mythrid); //3.4 string concat(filename)
    fptr = fopen(filename, "a"); //3.4 open file (a = append(add content to a file without replacing old content))

    while ((n = read(tmp_fd, line, MAXLINE)) != 0){
        printf("tid = %ld : confd = %d : %s \n", mythrid, tmp_fd, line); //3.3
        fprintf(fptr, "%s", line); //3.4 write in file
        fflush(stdout);
        m = write(tmp_fd, line, n);
        fflush(stdout);
    }
    fclose(fptr);//3.4 close file

    close(tmp_fd);
}
