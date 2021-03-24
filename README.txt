List of relevant files:
    Makefile
    packet.h
    DieWithError.c
    HandleTCPClient.c
    Client.c
    Server.c
    sample.txt

Compilation instructions:
    compile the program:    make

No Configuration file(s)

Running instructions:

    Running the server:
        ./Server 9999
        
        (Note: 9999 is the Port used in this program, 
        it is hard coded in client, if you want to 
        use a different Port, change "servPort" at
        line 23 in Client.c)

    Running the client:
        ./Client

        (Note: Server port number"9999" and IP address"127.0.0.1"
        are hard coded in this program, you can find them at
        line 23 and 24 in the file Client.c)

        After the connection between the server and client is
        established, you will be asked to enter the file name 
        you want to transfer from the server. sample.txt is 
        included in the directory,
        type: ./sample.txt

        A new file named out.txt will be created under the same
        directory will be created.
        
