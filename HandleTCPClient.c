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

    struct pkt * buffer; 
    int recvMsgSize;
    if((recvMsgSize = recv(clntSocket, buffer, 85, 0)) < 0)
        DieWithError("recv() failed");
    

    char * filename = buffer->data;
    FILE *fp;
    char buff[BUFFERSIZE];
    short seq_num = 1;
    

    fp = fopen(filename, "r");
    if(fp == NULL) {
        DieWithError("Error");
    }
    // while (fgets(buff, BUFFERSIZE, fp) != NULL) {
    //     struct pkt packet;
    //     strcpy(packet.data, buff);
    //     packet.seq = seq_num++;
    //     packet.count = strlen(packet.data);
    //     // printf("%s", packet.message);
    //     // printf("%d\n",packet.seq);
        
    //     if (send(clntSocket, &packet, packet.count, 0) != packet.count)
    //         DieWithError(packet.count);
    // }
    
    // fclose(fp);
}





