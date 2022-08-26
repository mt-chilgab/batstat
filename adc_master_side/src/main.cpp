#include "QVoltageIndicatorApp.hpp"
#include "Socket/Client.hpp"

#include <QApplication>
#include <iostream>
#include <stdexcept>
#include <thread>

void sendVoltsAmps(Sock *s, VoltageIndicator *w){
	while(1){
		s->setData(w->getVolts() + "V " + w->getAmps() + w->getAmpUnit() + "\n");
		
		while(!(s->isConnected)){
			printf("Waiting...\n");
			sleep(1);
			s->isConnected = s->estabConnection();
		}

		if(w->sendMessage){
			s->writeData();
			s->clearData();
		}

		w->sendMessage = false;
	}
}	

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VoltageIndicator *w = new VoltageIndicator();

	w->setFixedSize(300, 170);
	w->setWindowTitle("Battery Status Indicator");
    w->show();

	Sock *s = new Sock();
	thread send(sendVoltsAmps, s, w);
	a.exec();

	send.join();
	
	return 0;
}
