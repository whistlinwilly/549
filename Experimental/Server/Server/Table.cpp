#include "stdafx.h"


unsigned int Table::client_id; 

Table::Table()
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

void Table::update()
{

	
}



void Table::sendActionPackets(int client_id)
{
    // send action packet
	
    unsigned int packet_size;
    char packet_data[20] = "LOTS OF POOP!";
	packet_size = strlen(packet_data);

    network->sendToAll(packet_data,packet_size, client_id);
}
