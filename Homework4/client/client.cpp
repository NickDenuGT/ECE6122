/*
Author: Nicholas Denu
Class ECE 6122
last Date Modified: 10/23/19

Description:

This file is meant to act as a client into the partnering server program. This 
program sends messages to the server that are stored in a UDP method. 
This program also takes responses from the client and displays them.
*/


#include "client.h"

typedef int SOCKET;


/*
Purpose: close the socket being used.

Inputs: sock - the current socket being used.
Outputs: int - the return status for the function.
*/

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


/*
Purpose: Gather errors throughout the function.

Inputs: msg - the error message.
Outputs: None
*/
void error(const char *msg)
{
    perror(msg);

    exit(0);
}

/*
Purpose: Prints out the message packet in an organized manner.

Inputs: outGoing - the message packet.
Outputs: None
*/
void printMessageStruct(udpMessage outGoing)
{
    cout << "UDPMESSAGE\n\n";
    cout << "chMsg = " << outGoing.chMsg;
    cout << "\nlSeqNum = " << outGoing.lSeqNum;
    cout << "\nnMsgLen = " << outGoing.nMsgLen;
    cout << "\ntype = " << outGoing.nType;
    cout << "\nversion = " << outGoing.nVersion;
    cout << "\n\n";
}

/*
Purpose: This function is called as a thread and is listening for any response
from the server. When it does receive a response from the server it will
display it in an organized manner.

Inputs: sockfd_void - The socket id. Passed as a void * to accomodate for
the pthread calling this function.
Outputs: None
*/
void *listener(void *sockfd_void)
{
    int sockfd = *((int *)(&sockfd_void));
    udpMessage incomingMessageStruct;
    struct sockaddr_in from;
    int n;
    socklen_t fromlen = sizeof(struct sockaddr_in);
    while (true)
    {
        n = recvfrom(sockfd, (char*)&incomingMessageStruct, sizeof(udpMessage), 0, (struct sockaddr *)&from, &fromlen);
        
        //printMessageStruct(incomingMessageStruct);
        
        incomingMessageStruct.chMsg[incomingMessageStruct.nMsgLen] = 0;
        printf("Received Msg Type: 1, Seq: 33, Msg: %s\n", incomingMessageStruct.chMsg);
    }
}

/*
Purpose: This function is used as the command getter from the user. Based on
the command the messages are sent with different messages and types.

Inputs: argc - The number of command line items.
        argv[] - The ip and port numbers will need to be inputs here.

Outputs: int - Returns 0 unless there is an error. That is not built in.
*/
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

    string command_line, message;
    long sequence;
    char command_char, version, type;
    
    pthread_t listenerThread;
    pthread_create(&listenerThread, NULL, listener, (void *)sockfd);
    
    //thread listenerThread(listener, sockfd);
    
    while (true)
    {
        printf("Please enter command: ");
        
        getline(cin, command_line);
        
        command_char = command_line.at(0);
        
        if(command_char == 'v')
        {
            version = command_line.at(2);
        }
        else if(command_char == 't')
        {
            type = command_line.at(2);
            command_line = command_line.substr(4);
            sequence = (long)stoi(command_line.substr(0, command_line.find(" ")));
            message = command_line.substr(command_line.find(" ") + 1);
            
            strcpy(outGoing.chMsg, message.c_str());
            outGoing.lSeqNum = sequence;
            outGoing.nMsgLen = command_line.length() - (command_line.find(" ") + 1);
            outGoing.nType = type;
            outGoing.nVersion = version;
            
            //printMessageStruct(outGoing);
            
            n = sendto(sockfd, (void *)&outGoing, sizeof(outGoing), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            
            
        }
        else if(command_char == 'q')
        {
            pthread_cancel(listenerThread);
            break;
        }
        
        
        


    }

    sockClose(sockfd);

    return 0;
}
