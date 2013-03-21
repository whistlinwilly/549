class Table
{

public:

    Table();
    ~Table();

    void update();
	void receiveFromClients();
	void sendActionPackets(int client_id);

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

   // The ServerNetwork object 
    ServerNetwork* network;
};

