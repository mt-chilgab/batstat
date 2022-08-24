#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
	QLabel* const voltLabel = new QLabel("Voltage", this);
	QLabel* const voltUnit = new QLabel("V", this);
	QLabel* const ampLabel = new QLabel("Amperage", this);
	QLabel *ampUnit;
	QLabel *volts;
	QLabel *amps;

private slots:
    void onLineReceived(QString data);
};

#endif // MAINWINDOW_H
