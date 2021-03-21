#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "packet.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFFERSIZE 80

void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket, char * filename) {

    // fileTransf("./sample.txt");
    filename = "./sample.txt";
    FILE *fp;
    char buff[BUFFERSIZE];

    fp = fopen(filename, "r");
    if(fp == NULL) {
        DieWithError("Error");
    }
    short seq_num = 1;
    while (fgets(buff, BUFFERSIZE, fp) != NULL) {
        struct pkt packet;
        strcpy(packet.data, buff);
        packet.seq = seq_num++;
        packet.count = strlen(packet.data);
        // printf("%s", packet.message);
        // printf("%d\n",packet.seq);
        
        if (send(clntSocket, &packet, packet.count, 0) != packet.count)
            DieWithError(packet.count);
    }
    
    fclose(fp);
}





