#include "QVoltageIndicatorApp.hpp"
#include "Socket/Client.hpp"

#include <QApplication>
#include <thread>


void sendVoltsAmps(Sock *socket, VoltageIndicator *window){
	while(1){
		socket->setMessage(window->getVolts() + "V " + window->getAmps() + window->getAmpUnit() + "\n");
		
		while(!(socket->isConnected)){
			printf("Waiting for the server to open...\n");
			sleep(1);
			socket->estabConnection();
		}

		if(window->lineRcvd){
			while(1){
				socket->writeMessage();
				
				if(!(socket->isLastTransmissionOK)){
					socket->closeSock();
					printf("Again waiting for the server...\n");
					sleep(1);
					
					socket->initSock();
					socket->estabConnection();
				}
				else break;
			}
			socket->clearMessage();
		}
		window->lineRcvd = false;
	}
}	

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VoltageIndicator *window = new VoltageIndicator();
    Sock *socket = new Sock();

	window->setWindowTitle("Battery Status Indicator");
	window->setFixedSize(300, 170);
	window->show();
	
	thread send(sendVoltsAmps, socket, window);
	a.exec();

	send.join();
	
	return 0;
}
