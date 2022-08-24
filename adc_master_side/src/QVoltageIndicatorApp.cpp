#include "QVoltageIndicatorApp.hpp"
#include <QGridLayout>

VoltageIndicator::VoltageIndicator(QWidget *parent)
    : QWidget(parent)
{
	ampUnit = new QLabel("A", this);
	
	volts = new QLabel("Off", this);
	amps = new QLabel("Off", this);
	
	auto *grid = new QGridLayout(this);
	grid->addWidget(voltLabel, 0, 0);
	grid->addWidget(volts, 1, 0);
	grid->addWidget(voltUnit, 1, 1);

	grid->addWidget(ampLabel, 2, 0);
	grid->addWidget(amps, 3, 0);
	grid->addWidget(ampUnit, 3, 1);

	setLayout(grid);	
    connect(&serial,SIGNAL(lineReceived(QString)), this, SLOT(onLineReceived(QString)));

	serial.open("/dev/ttyACM0", 57600);
}

VoltageIndicator::~VoltageIndicator()
{
}

void VoltageIndicator::onLineReceived(QString data)
{
	QStringList tmp = data.split(" "); 
	volts->setText(tmp.at(0));

	double amperage = tmp.at(1).toDouble();
	if(amperage < 1){
		ampUnit->setText("mA");
		amps->setText(QString::number(amperage*1000, 'f', 0));
	}
	else{
		ampUnit->setText("A");
		amps->setText(QString::number(amperage, 'f', 2));
	}
}
