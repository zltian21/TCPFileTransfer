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
    struct sockaddr_in ServAddr;
    struct sockaddr_in ClntAddr;
    unsigned short servPort;
    unsigned int clntLen;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    // HandleTCPClient(1);
}