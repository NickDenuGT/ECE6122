
#include "server.h"


 typedef int SOCKET;

 

int sockInit(int sockfd, int portno)
{
    struct sockaddr_in serv_addr;
    // Zero out the variable serv_addr
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    
    // Initialize the serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // Convert port number from host to network
    serv_addr.sin_port = htons(portno);
    // Bind the socket to the port number
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        //error("ERROR on binding");
    }
    return 0;
}

int sockQuit(void)
{
    return 0;
}

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
/////////////////////////////////////////////////
// Output error message and exit
void error(const char *msg)
{
    perror(msg);
    // Make sure any open sockets are closed before calling exit
    exit(1);
}

// Called and ran in a thread to listen for new clients.
void listener(int sockfd)
{
    udpMessage incomingMessageStruct;
    struct sockaddr_in from;
    int n;
    socklen_t fromlen = sizeof(struct sockaddr_in);
    while (true)
    {
        n = recvfrom(sockfd, (char*)&incomingMessageStruct, sizeof(udpMessage), 0, (struct sockaddr *)&from, &fromlen);
        //printf("n = %d, nVersion = %c, nType = %c\n nMsgLen = %d\n", n, incomingMessageStruct.nVersion, incomingMessageStruct.nType, incomingMessageStruct.nMsgLen);
        
        if(incomingMessageStruct.nVersion != 1)
        {
            // Ignore Message
        }
        else if(incomingMessageStruct.nType == 0)
        {
            messageHistoryVector.clear();
        }
        else if(incomingMessageStruct.nType == 1)
        {
            messageHistoryVector.clear();
            udpMessageHistory tmp;
            tmp.from = from;
            tmp.fromlen = fromlen;
            tmp.in_message = incomingMessageStruct;
        
            messageHistoryVector.push_back(tmp);
        }
        else if(incomingMessageStruct.nType == 2)
        {
            udpMessageHistory tmp;
            tmp.from = from;
            tmp.fromlen = fromlen;
            tmp.in_message = incomingMessageStruct;

            messageHistoryVector.push_back(tmp);
        }
        else if(incomingMessageStruct.nType == 3)
        {
            createCompositeMessage();
            sendCompositeToAll(sockfd);
            messageHistoryVector.clear();
        }
        else
        {
            // Do nothing. An incorrect option was taken.
        }
        
    }
}

bool compareByLSeqNum(const udpMessageHistory &a, const udpMessageHistory &b)
{
    return a.in_message.lSeqNum < b.in_message.lSeqNum;
}

void createCompositeMessage()
{
    sort(messageHistoryVector.begin(), messageHistoryVector.end(), compareByLSeqNum);
    
    compositeMessage.nMsgLen = 0;
    strcpy(compositeMessage.chMsg,"");
    
    for(int i = 0; i < messageHistoryVector.size(); i++)
    {
        compositeMessage.nMsgLen += messageHistoryVector[i].in_message.nMsgLen;
        strncat(compositeMessage.chMsg, messageHistoryVector[i].in_message.chMsg, messageHistoryVector[i].in_message.nMsgLen);
    }
    
}

void sendCompositeToAll(int sockfd)
{
    int n = 0;
    for(int i = 0; i < messageHistoryVector.size(); i++)
    {
        n = sendto(sockfd, compositeMessage.chMsg, compositeMessage.nMsgLen, 0, (struct sockaddr *)(&messageHistoryVector[i].from), messageHistoryVector[i].fromlen);
        cout << n << "\n";
    }
}

/////////////////////////////////////////////////
// Main
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    int n;
/*
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
 */
    // Create the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // Make sure the socket was created
    /*if (sockfd < 0)
        error("ERROR opening socket");
    */
    // Convert the port number string to an int
    portno = atoi(argv[1]);
    
    sockInit(sockfd, portno);
    printf("Waiting on messages...\n");

    thread listenerThread(listener, sockfd);
    
    while(true)
    {
        int command;
        printf("\nPlease enter command: ");
        cin >> command;

        if(command == 0)
        {
            createCompositeMessage();
            sendCompositeToAll(sockfd);
            messageHistoryVector.clear();
        }
        else if(command == 1)
        {
            messageHistoryVector.clear();
        }
        else if(command == 2)
        {
            
            createCompositeMessage();
            cout << compositeMessage.chMsg << "\n";
        }
        else
        {
            cout << "Improper command. Try again.\n";
        }
    }
   
    sockClose(newsockfd);
    sockClose(sockfd);

    sockQuit();

    return 0;
}