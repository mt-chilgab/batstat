#include "QVoltageIndicatorApp.hpp"
#include "Socket/Client.hpp"

#include <QApplication>
#include <thread>

void sendVoltsAmps(Sock &socket, VoltageIndicator &window){
	while(1){
		socket.setMessage(window.getVolts() + "V " + window.getAmps() + window.getAmpUnit() + "\n");
		
		while(!(socket.isConnected)){
			printf("(%d) Waiting for the server to open...\n\n", socket.getPort());
			sleep(1);
			socket.estabConnection();
		}

		if(window.lineRcvd){
			while(1){
				socket.writeMessage();
				
				if(!(socket.isLastTransmissionOK)){
					socket.closeSock();
					printf("(%d) Again waiting for the server...\n\n", socket.getPort());
					sleep(1);
					
					socket.initSock();
					socket.estabConnection();
				}
				else break;
			}
			socket.clearMessage();
		}
		window.lineRcvd = false;
	}
}	


int main(int argc, char *argv[]){
	
	QApplication a(argc, argv);
    VoltageIndicator window;

	window.setWindowTitle("Battery Status Indicator");
	window.setFixedSize(300, 170);
	window.show();
    
	Sock socket(40000);	
	Sock socket_(40001);
	Sock socket__(40002);

	thread send(sendVoltsAmps, ref(socket), ref(window));
	thread send_(sendVoltsAmps, ref(socket_), ref(window));
	thread send__(sendVoltsAmps, ref(socket__), ref(window));

	a.exec();

	send.join();
	send_.join();
	send__.join();	

	return 0;
}
