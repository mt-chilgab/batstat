#include "Client.hpp"
#include <iostream>

Sock::Sock(){
	initSock();
	isConnected = false;
	isLastTransmissionOK = false;
}

Sock::~Sock(){
	close(sock);
}

void Sock::initSock(){	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) perror("Opening Stream Socket");

	opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));

	hp = gethostbyname(strdup(host.c_str())); 
	if(hp == 0) cerr << host << ": unknown host" << endl;

	server.sin_family = AF_INET;
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = port;
}

void Sock::reInitSock(){
	close(sock);
	initSock();
}

string Sock::getMessage(){
	return this->message;
}

void Sock::setMessage(string message){
	this->message = message;
}

void Sock::clearMessage(){
	this->message = "\0";
}

void Sock::writeMessage(){
	isLastTransmissionOK = send(sock, strdup(message.c_str()), message.length()*sizeof(char), MSG_NOSIGNAL) >= 0;
	if(!isLastTransmissionOK) perror("Writing Message");
}

void Sock::estabConnection(){
	isConnected = connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0;
	if(!isConnected) perror("Connecting to Server");
}
