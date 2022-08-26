#pragma once

#include "QAsyncSerial.hpp"
#include <QLabel>
#include <string>

using namespace std;

class VoltageIndicator : public QWidget
{
    Q_OBJECT

public:
    VoltageIndicator(QWidget *parent = nullptr);
    ~VoltageIndicator();
	
	string getVolts();
	string getAmps();
	string getAmpUnit();
	
	bool sendMessage;

private:
    QAsyncSerial serial;
	QLabel *voltLabel;
	QLabel *voltUnit;
	QLabel *ampLabel;
	QLabel *ampUnit;
	QLabel *volts;
	QLabel *amps;
	
private slots:
    void onLineReceived(QString data);
};
