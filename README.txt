List of relevant files:
    Makefile
    packet.h
    DieWithError.c
    HandleTCPClient.c
    Client.c
    Server.c
    sample.txt

Compilation instructions:
    compile the program:  make

No Configuration file(s)

Running instructions:

    Running the server:
        ./Server
        
        (Note: 9999 is the Port used in the program, it is hard coded in both server and client.
        If you need to use a different Port, change "PORT_NUMBER" at line 16 in both Server.c and Client.c)

    Running the client:
        ./Client

        (Note: Server port number"9999" and IP address"127.0.0.1" are hard coded in this program. 
        If you need to use a different Port, change "PORT_NUMBER" at line 16 in both Server.c and Client.c.
        If you need to use a different Server IP address, change "IP_ADDRESS" at line 15 in Client.c)


    After the connection is established, you will be asked to enter the filename at the Client side.
    A testing file(sample.txt) is included in this program.
    Enter: ./sample.txt
    then hit Retutn key to start the tranferation.
    
    Once the file is fully transfered, both client and server will be stop running and eixt.
    You will see a new file named "out.txt" under the current directory that has the content fetched from the server.
        
