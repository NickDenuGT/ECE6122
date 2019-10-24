
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
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    //if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    //{
        //error("ERROR on binding");
    //}
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

void sendCompositeToAll(int sockfd)
{
    int n = 0;
    for(int i = 0; i < messageHistoryVector.size(); i++)
    {
        n = sendto(sockfd, (void *)&compositeMessage, sizeof(udpMessage), 0, (struct sockaddr *)(&messageHistoryVector[i].from), messageHistoryVector[i].fromlen);
    }
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
        
        //printMessageStruct(incomingMessageStruct);
        
        if(incomingMessageStruct.nVersion != '1')
        {
            // Ignore Message
        }
        else if(incomingMessageStruct.nType == '0')
        {
            messageHistoryVector.clear();
        }
        else if(incomingMessageStruct.nType == '1')
        {
            messageHistoryVector.clear();
            udpMessageHistory tmp;
            tmp.from = from;
            tmp.fromlen = fromlen;
            tmp.in_message = incomingMessageStruct;
        
            messageHistoryVector.push_back(tmp);
        }
        else if(incomingMessageStruct.nType == '2')
        {
            udpMessageHistory tmp;
            
            if(compositeMessageLength + incomingMessageStruct.nMsgLen > 1000)
            {
                int leftover, nMsgLen;
                nMsgLen = (1000 - compositeMessageLength);
                leftover = incomingMessageStruct.nMsgLen - nMsgLen;
                
                char tmp_string_front[1000];
                memset(&tmp_string_front[0], 0, 1000);
                char tmp_string_back[1000];
                memset(&tmp_string_back[0], 0, 1000);
                
                for(int i = 0; i < nMsgLen ; i++)
                {
                    tmp_string_front[i] = incomingMessageStruct.chMsg[i];
                }
                for(int i = 0; i < leftover ; i++)
                {
                    tmp_string_back[i] = incomingMessageStruct.chMsg[i + nMsgLen];
                }
                
                memset(&incomingMessageStruct.chMsg[0], 0, 1000);
                strcpy(incomingMessageStruct.chMsg, tmp_string_front);
               
                incomingMessageStruct.nMsgLen = nMsgLen;
            
                tmp.from = from;
                tmp.fromlen = fromlen;
                tmp.in_message = incomingMessageStruct;

                messageHistoryVector.push_back(tmp);
                
                createCompositeMessage();
                sendCompositeToAll(sockfd);
                messageHistoryVector.clear();
                
                
                compositeMessageLength = leftover;
                incomingMessageStruct.nMsgLen = leftover;
                memset(&incomingMessageStruct.chMsg[0], 0, 1000);
                strcpy(incomingMessageStruct.chMsg, tmp_string_back);
                incomingMessageStruct.lSeqNum = 0;
                
                tmp.in_message = incomingMessageStruct;\
                
                messageHistoryVector.push_back(tmp);
            }
            else
            {
                compositeMessageLength += incomingMessageStruct.nMsgLen;
                tmp.from = from;
                tmp.fromlen = fromlen;
                tmp.in_message = incomingMessageStruct;

                messageHistoryVector.push_back(tmp);
            }
        }
        else if(incomingMessageStruct.nType == '3')
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