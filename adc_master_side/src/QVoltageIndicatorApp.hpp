#pragma once

#include "QAsyncSerial.hpp"
#include <QLabel>

class VoltageIndicator : public QWidget
{
    Q_OBJECT

public:
    VoltageIndicator(QWidget *parent = 0);
    ~VoltageIndicator();

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
