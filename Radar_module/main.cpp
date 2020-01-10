#include "radar_module.h"
#include <QtWidgets/QApplication>
//#include <QtCharts>

int main(int argc, char *argv[])
{
	cfg = new Connection_cfg();
	QApplication a(argc, argv);
	Radar_module w;
	w.show();
	return a.exec();
}
