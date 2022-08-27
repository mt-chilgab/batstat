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

#define HOST "localhost"
#define PORT 40000

class Sock{

public:
	Sock();
	~Sock();

	void initSock();
	void closeSock();

	bool isConnected;
	void estabConnection();

	bool isLastTransmissionOK;
	string getMessage();
	void setMessage(string message);
	void clearMessage();
	void writeMessage();

private:
	string message;

	const string host = HOST;
	const int port = htons(PORT);

	int sock, opt;
	struct sockaddr_in server;
	struct hostent *hp;
};
