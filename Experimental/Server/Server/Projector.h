class Projector
{

public:

    Projector();
    ~Projector();

	void sendActionPackets();

    char network_data[MAX_PACKET_SIZE];

    void update();

    ClientNetwork* network; 
};
