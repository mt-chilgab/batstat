#include "Client.hpp"
#include <iostream>

Sock::Sock(){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) perror("Opening Stream Socket");
	
	hp = gethostbyname(strdup(host.c_str())); 
	if(hp == 0) cerr << host << ": unknown host" << endl;

	server.sin_family = AF_INET;
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = port;
	
	isConnected = false;
}

Sock::~Sock(){
	close(sock);
}

string Sock::getData(){
	return this->data;
}

void Sock::setData(string d){
	this->data = d;
}

void Sock::clearData(){
	this->data = "\0";
}

void Sock::writeData(){
	if(write(sock, strdup(data.c_str()), data.length()*sizeof(char)) < 0){
		perror("Writing Message");
	}
}

bool Sock::estabConnection(){
	return connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0;
}
