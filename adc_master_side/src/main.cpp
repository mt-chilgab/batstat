#include <QApplication>
#include "QVoltageIndicatorApp.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VoltageIndicator w;

	w.setWindowTitle("Battery Status Indicator");
    w.show();

    return a.exec();
}
