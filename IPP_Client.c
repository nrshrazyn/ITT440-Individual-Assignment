//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 4444

void send_file_data(FILE* fp, int sockfd, struct sockaddr_in addr)
{
  int n;
  char buffer[1024];

  // Sending the data
  while (fgets(buffer, 1024, fp) != NULL)
  {
    printf("[SENDING] File Contains: %s", buffer);

    n = sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (n == -1)
    {
      perror("[ERROR] sending file to the server.");
      exit(1);
    }
    bzero(buffer, 1024);
  }

  // Sending the 'END'
  strcpy(buffer, "END");
  sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));

  fclose(fp);
}
void sigint_handler(int sig)
{
    printf("\nOpps, the server is busy right now. Please try again later!\n");
    exit(1);
}


int main()
{
 //declaration
 int clientSocket, ret;
 struct sockaddr_in serverAddr;
 FILE *fp;
 char *filename = "CS2554A_IndividualAssignment.txt";
 char buffer[1024], bufferRespond[1024], password[50];
 
 void sigint_handler(int sig);
 if(signal(SIGINT, sigint_handler) == SIG_ERR)
 {
     perror("signal");
     exit(1);
 }
 
 //create socket
 clientSocket = socket(AF_INET, SOCK_STREAM, 0);
 if(clientSocket < 0)
 {
  printf("[-]Error in connection.\n");
  exit(1);
 }
 printf("[+]Client Socket is created.\n");
 
 memset(&serverAddr, '\0', sizeof(serverAddr));
 serverAddr.sin_family = AF_INET;
 serverAddr.sin_port = htons(PORT);
 serverAddr.sin_addr.s_addr = inet_addr("192.168.121.5");

 //connect
 ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
      if(ret < 0)
      {
     printf("[-]Error in connection.\n");
     exit(1);
      }
      else
      {
          printf("Enter password: \t");
          scanf("%s", &password[0]);
          send(clientSocket, password, strlen(password), 0);
          if(strcmp(password, "aziyan") == 0)
          {
            printf("\n[+]Connected to Server.\n");
            printf("Waiting A Respond From The Server...\n");
            recv(clientSocket, bufferRespond, 1024, 0);
            printf("\nServer: %s\n", bufferRespond);
          }
          else
          {
              printf("Access Denied! Wrong password! Try again!\n");
              close(clientSocket);
              exit(1);
          }
      }
        while(1)
        {
         printf("Please select the option: \t");
         scanf("%s", &buffer[0]);
         send(clientSocket, buffer, strlen(buffer), 0);

         if(strcmp(buffer, "2") == 0)
         {
          close(clientSocket);
          printf("[-]Disconnected from server.\n");
          exit(1);
         }  
         else if(strcmp(buffer, "1") == 0)
         {
          fp = fopen(filename, "r");
          if(fp == NULL)
             {
              perror("[-]Error in reading file. \n");
              printf("Printing is Unsuccessfull! \n");
              exit(1);
             }
          else
            {
            printf("\nTransfering The File...\n");
            sleep(4);
            send_file_data(fp, clientSocket, serverAddr);
            printf("\nFile sent successfully. \n");
            printf("Waiting Server To Start Printing Process... \n");
            printf("\nPrinter Name : HP Printer Server \n");
            printf("Source IP address : 192.168.121.5 \n");
            printf("File name : %s\n", filename);
            printf("\n");
              for(int i = 0; i<10; i++)
              {
                  printf("Printing...\n");
                  sleep(2);
              }
              recv(clientSocket, buffer, 1024, 0);
              printf("\nServer: %s\n", buffer);
              printf("[+]Closing the connection. \n");
              close(clientSocket);
              exit (1);
            }
         }//end else if
       else
        {
         close(clientSocket);
         printf("Wrong option! Try again!\n");
         printf("[-]Disconnected from server.\n");
         exit(1);    
        }

    }//end while connected to server

 return 0;
}
