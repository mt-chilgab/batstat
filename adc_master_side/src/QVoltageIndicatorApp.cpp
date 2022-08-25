#include "QVoltageIndicatorApp.hpp"
#include <QFont>
#include <QGridLayout>

VoltageIndicator::VoltageIndicator(QWidget *parent)
    : QWidget(parent)
{
	voltLabel = new QLabel("Voltage", this);
	voltLabel->setGeometry(120, 380, 1000, 80);
	voltLabel->setFont(QFont("Helveltica", 12, QFont::Black));
	voltLabel->setAlignment(Qt::AlignLeft);

	ampLabel = new QLabel("Amperage", this);
	ampLabel->setGeometry(120, 380, 1000, 80);
	ampLabel->setFont(QFont("Helveltica", 12, QFont::Black));
	ampLabel->setAlignment(Qt::AlignLeft);

	voltUnit = new QLabel("V", this);
	voltUnit->setGeometry(120, 380, 1000, 80);
	voltUnit->setFont(QFont("Helveltica", 11, QFont::Bold));
	voltUnit->setAlignment(Qt::AlignLeft);

	ampUnit = new QLabel("A", this);
	ampUnit->setGeometry(120, 380, 1000, 80);
	ampUnit->setFont(QFont("Helveltica", 11, QFont::Bold));
	ampUnit->setAlignment(Qt::AlignLeft);
	
	volts = new QLabel("Off", this);
	volts->setGeometry(120, 380, 1000, 80);
	volts->setFont(QFont("Helveltica", 11, QFont::Light));
	volts->setAlignment(Qt::AlignRight);
	
	amps = new QLabel("Off", this);
	amps->setGeometry(120, 380, 1000, 80);
	amps->setFont(QFont("Helveltica", 11, QFont::Light));
	amps->setAlignment(Qt::AlignRight);
	
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
