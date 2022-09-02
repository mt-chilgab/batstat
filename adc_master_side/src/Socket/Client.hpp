#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>
using namespace std;


class Sock{

public:
	Sock(int p);
	Sock(string h, int p);
	~Sock();

	void initSock();
	void closeSock();

	int getPort();
	bool isConnected;
	void estabConnection();

	bool isLastTransmissionOK;
	string getMessage();
	void setMessage(string message);
	void clearMessage();
	void writeMessage();

private:
	string message;

	string host;
	int port;

	int sock, opt;
	struct sockaddr_in server;
	struct hostent *hp;
};
