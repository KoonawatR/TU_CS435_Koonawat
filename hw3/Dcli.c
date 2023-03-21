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

int accept_cr(int fd, struct sockaddr *addr, socklen_t *len);
int write_full(int fd, const void *buf, size_t count);
int read_full(int fd, void *buf, size_t count);

int client_shutdown_flag = 0;

int conn_fd;
struct sockaddr_in serv_addr;

//part2 circular linked list
struct hnode{
  int client;
  int length;
  char * line;
  struct hnode * next;
};

//create pointer Head/Tail
struct hnode * head = NULL;
struct hnode * tail = NULL;

//add new node
void addatlast(int id,int len,char * lin)
{
    // Initialize a new node
    struct hnode* temp;
    temp = (struct hnode*)malloc(sizeof(struct hnode));
  
    // If the new node is the only node in the list
    if (tail == NULL) {
        temp->client = id;
        temp->length = len;
        temp->line = malloc(strlen(lin)+1);
        strcpy(temp->line,lin);
        temp->next = temp;
        tail = temp;
        head = temp;
    }
  
    // Else the new node will be last node and will contain
    // the reference of head node
    else {
        temp->client = id;
        temp->length = len;
        temp->line = malloc(strlen(lin)+1);
        strcpy(temp->line,lin);
        temp->next = tail->next;
        tail->next = temp;
        tail = temp;
    }
}

//delete first node
void deletefirst()
{
  struct hnode* temp;

  temp = tail->next;
  tail->next = temp->next;
  free(temp->line);
  free(temp);
}

void printprompt(char id[]){
    char tP1[5] = "cli-";
    char tP2[10] = "> ";
    char concat[MAXLINE] = "";
    strcat(concat,tP1); // <- concat string
    strcat(concat,id); // <- concat string
    strcat(concat,tP2); // <- concat string
    printf("%s",concat);
    fflush(stdout);
}

//print list
void viewList()
{
    // If list is empty
    if (tail == NULL)
        printf("List is empty\n");
  
    // Else print the list
    else {
        struct hnode* temp;
        temp = tail->next;
        printf("----Chat History----\n");
        do {
          if(temp != NULL){
            printf("%s", temp->line);
            temp = temp->next;
          }
        } while (temp != tail->next);
    }
}

int main(int argc, char *argv[]){
    int n, m;
    int chk = 0;
	fd_set base_rfds, rfds; 
    int fdmax = 0; 
    int listchk = 0;
    char line[MAXLINE];
    char id[MAXLINE]; //store id
    char tP1[5] = "cli-";
    char tP2[10] = " says: ";
    char concat[MAXLINE] = "";

	conn_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);

        if (connect(conn_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) { 
            perror("Problem in connecting to the server");
            exit(3);
        }

	FD_ZERO(&base_rfds);
	FD_ZERO(&rfds);
	FD_SET(fileno(stdin), &base_rfds);
	FD_SET(conn_fd, &base_rfds);

	fdmax = conn_fd;

	while(1){
	  memcpy(&rfds, &base_rfds, sizeof(fd_set)); // copy it
          if (select(fdmax+1, &rfds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
          }
	  
	  if(FD_ISSET(fileno(stdin), &rfds)){
        if(fgets(line, MAXLINE, stdin) == NULL){
	      printf("Shutdown writing to TCP connection\n");
	      shutdown(conn_fd, SHUT_WR);
	      client_shutdown_flag = 1;
	    }
	    else{
            if(strcmp(line, "viewlist\n") == 0){
                viewList();
                printprompt(id);
            }else{
                n = write_full(conn_fd, line, MAXLINE);
                strcat(concat,tP1); // <- concat string
                strcat(concat,id); // <- concat string
                strcat(concat,tP2); // <- concat string
                strcat(concat,line); // <- concat string
                int x = atoi(id);
                addatlast(x, MAXLINE, concat); //add node
                listchk++;
                if(listchk > 10){ //if node > 10 delete
                    deletefirst();
                }
                memset(concat, 0, sizeof(concat)); // <- set array to empty
                printprompt(id);
            }
	    }
	  }
	  if(FD_ISSET(conn_fd, &rfds)){
            if(read_full(conn_fd, line, MAXLINE) == 0){
                if(client_shutdown_flag){
                    printf("TCP connection closed after client shutdown\n");
                    shutdown(conn_fd, SHUT_WR); // add shutdown
                    close(conn_fd);
                    break;
                }
                else{
                    printf("Error: TCP connection closed unexpectedly\n");
                    exit(1);
                }
            }
	    else{
            if(chk == 0){ // get id from server
                strcat(id,line);
                chk++;
                printprompt(id);
                continue;
            }
            //printf("\n");
            int x = atoi(id);
            addatlast(x, MAXLINE, line); //add node
            listchk++;
            if(listchk > 10){ //if node > 10 delete
                deletefirst();
            }
            printf("\n%s", line);
            printprompt(id);
	    }
	  }
	}
}

// -----Basic Communication Utilities-----
int accept_cr(int fd, struct sockaddr *addr, socklen_t *len){
	int ret;
repeat_accept:
        ret = accept(fd, addr, len);
        if (ret < 0) {
            if (errno == EINTR){
                goto repeat_accept;
	    }
	    printf("accept error errno=%d fd=%d\n", errno, fd);
        }
	return ret;
}

int write_full(int fd, const void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = write(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){
                continue;
	    }
	    printf("write error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}

int read_full(int fd, void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = read(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){ 
                continue;
	    } 
	    printf("read error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}
