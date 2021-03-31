/*
    Client.c 
    This file contains code that sets the client, reads filename from user, setups connection with server,
    sends and receives packets with the server.
*/
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "packet.h"

#define BUFFERSIZE 81 //80 printable character and 1 null terminating character.
#define IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER 9999

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
    int sock;                               // client socket 
    struct sockaddr_in servAddr;            // server address struture 
    unsigned short servPort;                // server port number
    char *servIP;                           // server IP address
    char filename[BUFFERSIZE];              // user input filename buffer
    short nameLen;                          // filename length
    struct header hder;                     // filename packet header

    
    //Set server IP address and port number
    servIP = IP_ADDRESS;
    servPort = PORT_NUMBER;

    //Create a socket using TCP
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    
    //Construct the server address structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);
    servAddr.sin_port = htons(servPort);

    //Establish connect to echo server
    if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
        DieWithError("connect() failed");
    
    //Get filename and set first packet header
    printf("Enter the filename to get from server:\n");
    scanf("%s", filename);
    hder.count = htons(strlen(filename));
    hder.seq = htons(0);
    
    //Send packet header, then send file name
    if (send(sock, &hder, sizeof(hder), 0) != sizeof(hder))
        DieWithError("send() header sent a different number of bytes than expected");
    if (send(sock, filename, strlen(filename), 0) != strlen(filename))
        DieWithError("send() data sent a different number of bytes than expected");

    //Attributes for reciving packet
    struct header hder_buff;                // header buffer for receiving
    char buff[BUFFERSIZE];                  // data buffer for receiving
    int bytesRcvd;                          // bytes actually received
    short tempSeq;                          // temporary Sequence Number
    short tempCount;                        // temporary Count(data characters)
    FILE *fp;                               // file to write
    int totalPackets = 0;                   // Total packets received
    int totalBytes = 0;                     // Total data bytes received
    fp = fopen("./out.txt", "w");
    printf("\n");
    
    //Reveive packets from server
    do {
        //Recv Header
        if((bytesRcvd = recv(sock, &hder_buff, sizeof(hder_buff), 0)) < 0)
            DieWithError("recv()header failed");
        tempSeq = ntohs(hder_buff.seq);
        tempCount = ntohs(hder_buff.count);

        //Recv Data
        if((bytesRcvd = recv(sock, buff, tempCount > 0 ? tempCount + 1 : tempCount, 0)) < 0)
            DieWithError("recv()buff failed");

        //If the packets are not EOF
        if(tempCount > 0) {
            totalPackets++;
            totalBytes += tempCount;
            printf("Packet %d received with %d data bytes\n", tempSeq, tempCount);
            //Write line in file
            fprintf (fp, "%s", buff);
        }
    
    //While the packet is not EOF
    } while(tempCount > 0);
    printf("------------------SUMMARY------------------\n");
    printf("Number of data packets received: %d\n", totalPackets);
    printf("Total number of data bytes received: %d\n", totalBytes);
    printf("\n");
    fclose(fp);
	close(sock);
	exit(0);
}
