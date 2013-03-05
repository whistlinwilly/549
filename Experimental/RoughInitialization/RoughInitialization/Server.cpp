#include "Server.h"

Server::~Server(){
	closesocket(ClientSocket);
}

Server::Server(){

	WSADATA wsaData;
	typedef unsigned long IPNumber;    // IP number typedef for IPv4
	const int SERVER_PORT = 6882;

    // our sockets for the server
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

	HOSTENT* hostent;
	SOCKADDR_IN sockAddr = {0};

    
    

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

	// Get IP Address of website by the domain name, we do this by contacting(??) the Domain Name Server
    if ((hostent = gethostbyname("10.0.1.3")) == NULL)  // "localhost"  www.google.com
    {
        printf("Failed to resolve website name to an ip address\n");
        WSACleanup();
        exit(1);
    }

    sockAddr.sin_port             = htons(SERVER_PORT);
    sockAddr.sin_family           = AF_INET;
    sockAddr.sin_addr.S_un.S_addr = (*reinterpret_cast <IPNumber*> (hostent->h_addr_list[0]));
    


    // Set the mode of the socket to be nonblocking
    u_long iMode = 0;
	printf("ABOUT TO LISTEN\n");
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

	printf("MADE IT PAST LISTENING\n");

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, (SOCKADDR*)(&sockAddr), sizeof(sockAddr));
	printf("Listening Socket Bound to: addr: %s, port: %d\n", hostent->h_name, ntohs(sockAddr.sin_port));

    
	if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }


    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
	

	// if client waiting, accept the connection and save the socket
    ClientSocket = accept(ListenSocket,NULL,NULL);
    if (ClientSocket == INVALID_SOCKET) 
    {
		printf("ERROR ACCEPTING\n");
    }
}
	//printf("Connecting With New Client...\n");
	//
	//const char* words = "0";

	//send(ClientSocket, words, 1, 0);



	//closesocket(ClientSocket);

void Server::sendString(const char* i){
	send(ClientSocket, i, strlen(i), 0);
}

void Server::confirm(){
	char word[1];
	while(recv(ClientSocket, word, 1, 0) ==0);
}