/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   client.h
 * Author: nick
 *
 * Created on October 22, 2019, 8:06 PM
 */

#ifndef CLIENT_H
#define CLIENT_H

typedef struct udpMessage{
    unsigned char nVersion;
    unsigned char nType;
    unsigned short nMsgLen;
    unsigned long lSeqNum;
    char chMsg[1000];
};

#endif /* CLIENT_H */

