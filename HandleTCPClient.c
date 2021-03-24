#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "packet.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFFERSIZE 81 //80 printable character and 1 null terminating character 
// #define EOT "\u0004"

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

    fp = fopen(filename, "r");
    if(fp == NULL) {
        DieWithError("Error");
    }
    
    while (fgets(buff, BUFFERSIZE, fp) != NULL) {
        hder.seq = htons(seq_num++);
        hder.count = htons(strlen(buff));
        // printf("%s\n", buff);
        // printf("seq %d\n",ntohs(hder.seq));
        // printf("count %d\n", ntohs(hder.count));
        if (send(clntSocket, &hder, sizeof(hder), 0) != sizeof(hder))
            DieWithError("send()Header failed");

        if (send(clntSocket, buff, strlen(buff)+1, 0) != strlen(buff)+1)
            DieWithError("send()Line failed");
    }

    //EOT
    hder.seq = htons(seq_num++);
    hder.count = htons(0);
    memset(buff, 0, sizeof(buff));
    if(send(clntSocket, &hder, sizeof(hder), 0) != sizeof(hder))
        DieWithError(("send()EOT Header failed"));
    
    if (send(clntSocket, buff, strlen(buff), 0) != strlen(buff))
            DieWithError("send()EOT Line failed");
    fclose(fp);
    close(clntSocket);
}





