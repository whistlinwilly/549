struct Packet {
	char packet_type[12]; //(From Projector: REQUEST, DATA. From Table: READY, DATA) 
	char data_type[12]; //Types of data - AUDIO, VIDEO, IMAGE, ...
	char* data;
	int data_length;
};

