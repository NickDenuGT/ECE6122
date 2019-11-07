/*
Author: Nicholas Denu
Class ECE 6122
last Date Modified: 10/23/19

Description:

This file has all of the includes needed for the client.cpp file.
This file also has the structure that will be passed through the sockets.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <iostream>
#include <string>
#include <thread>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int SOCKET;


using namespace std; 

#ifndef CLIENT_H
#define CLIENT_H

struct udpMessage{
    unsigned char nVersion;
    unsigned char nType;
    unsigned short nMsgLen;
    unsigned long lSeqNum;
    char chMsg[1000];
};


#endif /* CLIENT_H */

