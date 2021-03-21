CC=gcc

all: Server

OTHER_OBSS = DieWithError.o HandleTCPClient.o

Server: Server.o $(OTHER_OBSS)

DieWithError.o: DieWithError.c 
	$(CC) -c DieWithError.c

HandleTCPClient.o: HandleTCPClient.c packet.h
	$(CC) -c HandleTCPClient.c

Server.o: Server.c
	$(CC) -c Server.c

clean:
	rm -f *.o Server *~
