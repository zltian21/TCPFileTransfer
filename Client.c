#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "packet.h"

#define BUFFERSIZE 81 //80 printable character and 1 null terminating character.
#define IP_ADDRESS "127.0.0.1"
#define PORT_NO 9999

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in servAddr;
    unsigned short servPort;
    char *servIP;
    char filename[BUFFERSIZE];
    short nameLen;
    struct header hder;
    int bytesRcvd;
    
    servIP = IP_ADDRESS;
    servPort = PORT_NO;

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
    
    //Get filename and set packet 0 header
    printf("Enter the filename to get from server:\n");
    scanf("%s", filename);
    hder.count = htons(strlen(filename));
    hder.seq = htons(100);
    
    //Send the filename and packet header
    if (send(sock, &hder, sizeof(hder), 0) != sizeof(hder))
        DieWithError("send() header sent a different number of bytes than expected");
    if (send(sock, filename, strlen(filename), 0) != strlen(filename))
        DieWithError("send() data sent a different number of bytes than expected");

    //Attributes for reciving packet
    struct header hder_buff;
    char buff[BUFFERSIZE];
    int i = 1;
    short tempSeq;
    short tempCount;
    FILE *fp;
    int totalPackets = 0;
    int totalBytes = 0;
    fp = fopen("./out.txt", "w");
    printf("\n");
    do {
        
        if((bytesRcvd = recv(sock, &hder_buff, sizeof(hder_buff), 0)) < 0)
            DieWithError("recv()header failed");
        tempSeq = ntohs(hder_buff.seq);
        tempCount = ntohs(hder_buff.count);

        if((bytesRcvd = recv(sock, buff, tempCount > 0 ? tempCount + 1 : tempCount, 0)) < 0)
            DieWithError("recv()buff failed");

        if(tempCount > 0) {
            totalPackets++;
            totalBytes += tempCount;
            printf("Packet %d received with %d data bytes\n", tempSeq, tempCount);
            fprintf (fp, "%s", buff);
        }

    } while(tempCount > 0);
    printf("------------------SUMMARY------------------\n");
    printf("Number of data packets received: %d\n", totalPackets);
    printf("Total number of data bytes received: %d\n", totalBytes);
    printf("\n");
    fclose(fp);
	close(sock);
	exit(0);
}
