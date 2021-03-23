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
    if((recvMsgSize = recv(clntSocket, buffer, 90, 0)) < 0) //85
        DieWithError("recv() failed");
        
    printf("%s\n", buffer->data);
    printf("%d\n", ntohs(buffer->count));
    printf("%d\n", ntohs(buffer->seq));
    char * filename = buffer->data;
    FILE *fp;
    char buff[BUFFERSIZE];
    short seq_num = 1;
    struct pkt packet;

    fp = fopen(filename, "r");
    if(fp == NULL) {
        DieWithError("Error");
    }
    
    int count;
    while (fgets(buff, BUFFERSIZE, fp) != NULL) {
        strcpy(packet.data, buff);
        packet.seq = htons(seq_num++);
        packet.count = htons(strlen(packet.data));
        printf("%s", packet.data);
        printf("seq %d\n",ntohs(packet.seq));
        printf("count %d\n", ntohs(packet.count));
        if (count = send(clntSocket, &packet, sizeof(packet), 0) != sizeof(packet))
            DieWithError("send() failed");
        printf("SENTTTT: %d\n", count);
    }
    
    // fclose(fp);
}





