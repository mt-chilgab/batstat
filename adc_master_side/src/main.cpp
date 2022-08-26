#include "QVoltageIndicatorApp.hpp"
#include "Socket/Client.hpp"
#include <QApplication>
#include <thread>

void scw(Sock *s, string d){
	while(1){
		s->setData(d);
		s->writeData();
		s->clearData();
	}	
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VoltageIndicator w;

	w.setFixedSize(300, 170);
	w.setWindowTitle("Battery Status Indicator");
    w.show();
	
	Sock *s = new Sock();

	thread send(scw, s, w.getVolts() + " " + w.getAmps() + "\n");
	a.exec();

	send.join();

	return 0;
}
