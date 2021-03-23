#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "packet.h"

#define BUFFERSIZE 81 //80 printable character and 1 null terminating character 

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in servAddr;
    unsigned short servPort;
    char *servIP;
    char filename[BUFFERSIZE];// for filename
    short nameLen;
    struct header hder;
    // unsigned int filenameLen;
    int bytesRcvd;
    
    servIP = "127.0.0.1";
    servPort = 9999;
    
    //Get filename
    // printf("Enter the filename to get from server:\n");
    // scanf("%s", filename);
    strcpy(filename, "./sample.txt"); // temoline
    hder.count = htons(strlen(filename));
    hder.seq = htons(100);
    // printf("%d\n", sizeof(struct header));

    // filenameLen = strlen(filename);

    //Create a socket using TCP
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    
    //Construct the server address structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);
    servAddr.sin_port = htons(servPort);

    //*Establish connect to echo server*/
    if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
        DieWithError("connect() failed");
    
    //Send the struct
    if (send(sock, &hder, sizeof(hder), 0) != sizeof(hder))
        DieWithError("send() header sent a different number of bytes than expected");
    
    if (send(sock, filename, strlen(filename), 0) != strlen(filename))
        DieWithError("send() data sent a different number of bytes than expected");
}
