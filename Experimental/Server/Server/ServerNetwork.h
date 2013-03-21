
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881" 

using namespace std; 
#pragma comment (lib, "Ws2_32.lib")





class ServerNetwork
{

public:

    ServerNetwork();
    ~ServerNetwork();

    // Socket to listen for new connections
    SOCKET ListenSocket;

    // Socket to give to the clients
    SOCKET ClientSocket;

    // for error checking return values
    int iResult;

    // table to keep track of each client's socket
    std::map<unsigned int, SOCKET> sessions; 

	bool acceptNewClient(unsigned int & id);

	// send data to all clients
    void sendToAll(char * packets, int totalSize, int client_id);

	// receive incoming data
    int receiveData(unsigned int client_id, char * recvbuf);

};

