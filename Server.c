//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

void write_file(int sockfd, struct sockaddr_in addr)
{

  char* filename = "CS2554A_IndividualAssignment.txt";
  int n;
  char buffer[1024];
  socklen_t addr_size;

  // Creating a file.
  FILE* fp = fp = fopen(filename, "w");

  // Receiving the data and writing it into the file.
  while (1)
  {
    addr_size = sizeof(addr);
    n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);

    if (strcmp(buffer, "END") == 0)
    {
      break;
    }

    printf("[RECEVING] File Contains: %s", buffer);
    fprintf(fp, "%s", buffer);
    bzero(buffer, 1024);
  }

  fclose(fp);
}

int main(){
//declaration
 int sockfd, ret;
 struct sockaddr_in serverAddr;

 int newSocket;
 struct sockaddr_in newAddr;

 socklen_t addr_size;

 char buffer[1024], bufferRespond[1024], password[50];

 FILE *fp;
 char *filename = "CS2554A_IndividualAssignment.txt";

//create socket
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if(sockfd < 0)
 {
  printf("[-]Error in connection.\n");
  exit(1);
 }
 printf("[+]Server Socket is created.\n");

 memset(&serverAddr, '\0', sizeof(serverAddr));
 serverAddr.sin_family = AF_INET;
 serverAddr.sin_port = htons(PORT);
 serverAddr.sin_addr.s_addr = inet_addr("192.168.121.5");

//bind
 ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
     if(ret < 0)
     {
       printf("[-]Error in binding.\n");
       exit(1);
     }
      printf("[+]Bind to port %d\n", 4444);

//listen
      if(listen(sockfd, 10) == 0)
      {
        printf("[+]Listening....\n");
      }
      else
      {
        printf("[-]Error in binding.\n");
      }
//when listening
 while(1)
 {
  newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
  if(newSocket < 0)
  {
   exit(1);
  }
  else
  {
     recv(newSocket, password, 50, 0);
     if(strcmp(password, "aziyan") == 0)
     {
       
       printf("\nConnection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
       //send respond to client
       strcpy(bufferRespond, "HI, WELCOME TO THE PRINTER SERVER!\n1. Print            2. Exit\n");
       send(newSocket, bufferRespond, strlen(bufferRespond), 0);
       bzero(bufferRespond, 1024);
       printf("\n1. Print      2. Exit \n");
       printf("Waiting for input...\n");
     }
     else
     {
         printf("Someone trying to reach you!\n");
         break;
     }
  }//end else
  
    recv(newSocket, buffer, 1024, 0);
    printf("\nOption from Client: %s\n", buffer);
    if(strcmp(buffer, "2") == 0)
    {
     printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
     exit(1);
    }
    else if(strcmp(buffer, "1") == 0)
    {
     printf("\nWaiting Client Transfer The File...\n");
     sleep(4);
     write_file(newSocket, newAddr);
     printf("\nFile received successfully.\n");
     printf("\nPrinter Name : HP Printer Server \n");
     printf("Source IP address : 192.168.121.5 \n");
     printf("Destination IP address : %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
     printf("File name : %s\n", filename);
     printf("\n");
     for(int i= 0; i < 10; i++)
     {
     printf("Printing...\n");
     sleep(2);
     }
     strcpy(buffer, "PRINTING IS SUCCESSFULL!\n");
     printf("\nPrinting Status: %s\n", buffer);
     send(newSocket, buffer, strlen(buffer), 0);
     bzero(buffer, 1024);
     printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
     exit(1);
    }//end else if
    else
    {
    printf("Wrong option! Try again!\n");
    printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
     exit(1);
    }

 }//end while sockfd, 10
 close(newSocket);
 return 0;
}//end main
