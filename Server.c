/*
    Server.c 
    This file contains code that initiating the server,
    listening on ports, and establishing connection.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAXPENDING 5 /* Maximum outstanding connection requests */
#define PORT_NUMBER 9999

void DieWithError(char *errorMessage);
void HandleTCPClient(int clntSocket);

int main(int argc, char*argv[]) {
    
    int servSock;                       // server socket
    int clntSock;                       // client socket
    struct sockaddr_in servAddr;        // server address structure 
    struct sockaddr_in clntAddr;        // client address structure
    unsigned short servPort;            // server port number
    unsigned int clntLen;               // client structure length

    printf("Initiating Server...\n");

    //Port No.
    servPort = PORT_NUMBER;

    // Create socket for incoming connection
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    //Construct local address structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    
    // Bind to the local address
    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        DieWithError("bind() failed");
    // Mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    printf("Listening on Port %d...\n", servPort);

    for (;;) {
        // Set the size of the in-out parameter
        clntLen = sizeof(clntAddr);
        // Wait for a clien to connect
        if ((clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");
        //clntSock is connected to a client!
        printf("Handling client %s\n", inet_ntoa(clntAddr.sin_addr));
        HandleTCPClient(clntSock);
        exit(1);
    }
    
}