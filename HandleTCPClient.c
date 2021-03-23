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
    if((recvMsgSize = recv(clntSocket, hder_buff, 4, 0)) < 0)
        DieWithError("recv()header failed");
    
    if((recvMsgSize = recv(clntSocket, filename, 81, 0)) < 0)
        DieWithError("recv()filename failed");
        
    // printf("%d\n", ntohs(hder_buff->count));
    // printf("%d\n", ntohs(hder_buff->seq));
    // printf("%s\n", filename);

    FILE *fp;
    char buff[BUFFERSIZE];
    short seq_num = 1;
    struct header hder;

    fp = fopen(filename, "r");
    if(fp == NULL) {
        DieWithError("Error");
    }
    
    // int count;
    while (fgets(buff, BUFFERSIZE, fp) != NULL) {

        hder.seq = htons(seq_num++);
        hder.count = htons(strlen(buff));
        // printf("%s", buff);
        // printf("seq %d\n",ntohs(hder.seq));
        // printf("count %d\n", ntohs(hder.count));
        if (send(clntSocket, &hder, sizeof(hder), 0) != sizeof(hder))
            DieWithError("send()Header failed");
        if (send(clntSocket, buff, strlen(buff), 0) != strlen(buff))
            DieWithError("send()Line failed");
    }
    
    fclose(fp);
}





