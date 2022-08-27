#pragma once

#include "QAsyncSerial.hpp"
#include <QLabel>
#include <string>
#include <mutex>

using namespace std;

class VoltageIndicator : public QWidget
{
    Q_OBJECT

	mutable mutex classMutex;

public:
    VoltageIndicator(QWidget *parent = nullptr);
    ~VoltageIndicator();
	
	string getVolts();
	string getAmps();
	string getAmpUnit();
	
	bool lineRcvd;

private:
	
    QAsyncSerial serial;
	QLabel *voltLabel;
	QLabel *voltUnit;
	QLabel *ampLabel;
	QLabel *ampUnit;
	QLabel *volts;
	QLabel *amps;
	
	QStringList splitList;	

private slots:
    void onLineReceived(QString data);
};
