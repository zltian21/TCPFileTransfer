#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> /*for socket(), connect(), send(), and recv()*/
#include <arpa/inet.h> /*for sockaddr_in and inet_addr()*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING 5 /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage);
void HandleTCPClient(int clntSocket);

int main(int argc, char*argv[]) {
    int servSock;
    int clntSock;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    unsigned short servPort;
    unsigned int clntLen;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }
    //Port No.
    servPort = atoi(argv[1]);

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