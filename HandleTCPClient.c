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

    struct header * hder_buff; 
    char filename[BUFFERSIZE];
    int recvMsgSize;
    short tempSize;
    if((recvMsgSize = recv(clntSocket, hder_buff, 4, 0)) < 0)
        DieWithError("recv()header failed");
    tempSize = ntohs(hder_buff->count);
    
    if((recvMsgSize = recv(clntSocket, filename, tempSize, 0)) < 0)
        DieWithError("recv()filename failed");

    FILE *fp;
    char buff[BUFFERSIZE];
    short seq_num = 1;
    struct header hder;
    short tempSeq;
    short tempCount;
    int totalPackets = 0;
    int totalBytes = 0;

    fp = fopen(filename, "r");
    if(fp == NULL) {
        DieWithError("Error");
    }
    printf("\n");
    while (fgets(buff, BUFFERSIZE, fp) != NULL) {
        tempSeq = seq_num++;
        tempCount = strlen(buff); 
        hder.seq = htons(tempSeq);
        hder.count = htons(tempCount);

        if (send(clntSocket, &hder, sizeof(hder), 0) != sizeof(hder))
            DieWithError("send()Header failed");

        if (send(clntSocket, buff, strlen(buff)+1, 0) != strlen(buff)+1)
            DieWithError("send()Line failed");
        printf("Packet %d transmitted with %d data bytes\n", tempSeq, tempCount);
        totalPackets++;
        totalBytes += tempCount;
    }

    //EOT
    memset(buff, 0, sizeof(buff));
    tempSeq = seq_num++;
    tempCount = strlen(buff);

    hder.seq = htons(tempSeq);
    hder.count = htons(tempCount);
    if(send(clntSocket, &hder, sizeof(hder), 0) != sizeof(hder))
        DieWithError(("send()EOT Header failed"));
    
    if (send(clntSocket, buff, strlen(buff), 0) != strlen(buff))
            DieWithError("send()EOT Line failed");
    
    printf("End of Transmission Packet with sequence number %d transmitted with %d data bytes\n", tempSeq, tempCount);
    fclose(fp);
    close(clntSocket);
    // END: Total byts and packets 
    printf("------------------SUMMARY------------------\n");
    printf("Number of data packets transmitted: %d\n", totalPackets);
    printf("Total number of data bytes transmitted: %d\n", totalBytes);
    printf("\n");
}





