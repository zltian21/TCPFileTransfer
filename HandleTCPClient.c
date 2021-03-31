/*
    HandleTCPClient.c
    This file contains code that reads and sends local file to the client.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "packet.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFFERSIZE 81 //80 printable character and 1 null terminating character 

void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket) {

    struct header * hder_buff;      // header buffer for receiving filename packet header
    char filename[BUFFERSIZE];      // data buffer for receiving filename data
    int recvMsgSize;                // actual reveived packet size
    short filenameSize;             // filename data characters size

    //Reveive filename packet header
    if((recvMsgSize = recv(clntSocket, hder_buff, 4, 0)) < 0)
        DieWithError("recv()header failed");
    filenameSize = ntohs(hder_buff->count);
    
    //receive filename data
    if((recvMsgSize = recv(clntSocket, filename, filenameSize, 0)) < 0)
        DieWithError("recv()filename failed");

    //Attributes for reading and sending file.
    FILE *fp;                       // file to read
    char buff[BUFFERSIZE];          // buffer to save read line
    short seq_num = 1;              // start sequence number
    struct header hder;             // header to send
    short tempSeq;                  // temporary sequence number 
    short tempCount;                // temporary data character count
    int totalPackets = 0;           // total packets send
    int totalBytes = 0;             // total data bytes send

    fp = fopen(filename, "r");
    if(fp == NULL) {
        DieWithError("Error");
    }
    printf("\n");

    //While it is not the end of the file: 
    //read a line to buffer, and send packets to client
    while (fgets(buff, BUFFERSIZE, fp) != NULL) {
        // Set packet header
        tempSeq = seq_num++;
        tempCount = strlen(buff); 
        hder.seq = htons(tempSeq);
        hder.count = htons(tempCount);

        //Send packet header
        if (send(clntSocket, &hder, sizeof(hder), 0) != sizeof(hder))
            DieWithError("send()Header failed");
        //Send packet data
        if (send(clntSocket, buff, strlen(buff)+1, 0) != strlen(buff)+1)
            DieWithError("send()Line failed");
        
        printf("Packet %d transmitted with %d data bytes\n", tempSeq, tempCount);
        totalPackets++;
        totalBytes += tempCount;
    }

    //Send EOT Packet
    memset(buff, 0, sizeof(buff));
    tempSeq = seq_num++;
    tempCount = strlen(buff);
    hder.seq = htons(tempSeq);
    hder.count = htons(tempCount);
    if(send(clntSocket, &hder, sizeof(hder), 0) != sizeof(hder))
        DieWithError(("send()EOT Header failed"));
    
    if (send(clntSocket, buff, strlen(buff), 0) != strlen(buff))
            DieWithError("send()EOT Line failed");
    
    //print summary
    printf("End of Transmission Packet with sequence number %d transmitted with %d data bytes\n", tempSeq, tempCount);
    fclose(fp);
    close(clntSocket);
    // END: Total byts and packets 
    printf("------------------SUMMARY------------------\n");
    printf("Number of data packets transmitted: %d\n", totalPackets);
    printf("Total number of data bytes transmitted: %d\n", totalBytes);
    printf("\n");
}





