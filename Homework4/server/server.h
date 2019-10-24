

#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <iostream>
#include <thread>
#include <pthread.h>
#include <omp.h>
#include <vector>
#include <algorithm>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

#ifndef SERVER_H
#define SERVER_H


using namespace std; 

struct udpMessage{
    unsigned char nVersion;
    unsigned char nType;
    unsigned short nMsgLen;
    unsigned long lSeqNum;
    char chMsg[1000];
};

struct udpMessageHistory{
    struct sockaddr_in from;
    socklen_t fromlen;
    udpMessage in_message;
};

vector<udpMessageHistory> messageHistoryVector;

udpMessage compositeMessage = {'1', '0', 0, 0, ""};

int compositeMessageLength = 0;

#endif /* SERVER_H */

