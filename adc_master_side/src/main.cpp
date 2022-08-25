#include "QVoltageIndicatorApp.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VoltageIndicator w;

	w.setFixedSize(300, 170);
	w.setWindowTitle("Battery Status Indicator");
    w.show();

    return a.exec();
}
