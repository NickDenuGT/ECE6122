#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <iostream>
#include "client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int SOCKET;

int sockInit(void)
{
    return 0;
}

int sockQuit(void)
{
    return 0;
}

/* Note: For POSIX, typedef SOCKET as an int. */

int sockClose(SOCKET sock)
{

    int status = 0;

    status = shutdown(sock, SHUT_RDWR);
    if (status == 0)
    {
        status = close(sock);
    }

    return status;

}

void error(const char *msg)
{
    perror(msg);

    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    socklen_t fromlen = 0;
    struct sockaddr  from;
    memset((char *)&from, 0, sizeof(sockaddr));


    char buffer[1024];
    udpMessage outGoing;
    
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    sockInit();
    // Convert string to int
    portno = atoi(argv[2]);
    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;

    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    while (true)
    {
        printf("Please enter the message: ");

        memset(outGoing.chMsg, 0, 1000);
        outGoing.nVersion = '1';
        outGoing.lSeqNum = '1';
        fgets(outGoing.chMsg, 1000, stdin);
        
        for(int i = 0; i < 1000; i++)
        {
            if(outGoing.chMsg[i] == 0)
            {
                outGoing.nMsgLen = i;
                break;
            }
        }
        
        outGoing.nType = '3';
        
        printf("out message = %s\n", outGoing.chMsg);
        
        n = sendto(sockfd, (void *)&outGoing, sizeof(outGoing), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        printf("n = %d\n", n);
        
        if (n < 0)
            error("ERROR writing to socket");

        memset(buffer, 0, 1024);

 //       n = recv(sockfd, buffer, 1023, 0);
        fromlen = sizeof(serv_addr);
        n = recvfrom(sockfd, buffer, sizeof(outGoing), 0, (sockaddr *)&from, &fromlen);

        if (n < 0)
            error("ERROR reading from socket");
        else
            buffer[n] = 0;

        printf("%s\n", buffer);
    }

    sockClose(sockfd);
    sockQuit();

    return 0;
}