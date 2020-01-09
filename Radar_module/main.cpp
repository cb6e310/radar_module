#include "radar_module.h"
#include <QtWidgets/QApplication>
//#include <QtCharts>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Radar_module w;
	w.show();
	return a.exec();
}
