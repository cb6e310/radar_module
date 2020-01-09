#pragma once

#include <Qthread>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QMetaType>
#include <QSemaphore>
#include <QMutex>
#include "ControlCAN.h"
#include <map>
/*
QSemaphore semFree(8);
QSemaphore semAvail(0);
QMutex mtx;
*/
class Can_thread : public QThread
{
	Q_OBJECT

public:
	Can_thread(QObject *parent);
	~Can_thread();
	void close_connection();
	void open_CAN_device();
	void transmit_cfg();
	void check_connection_status();
	void stop();
	int baud_rate;


signals:
	void sig_received_frame();
	void sig_init_CAN_failed(int);

private:
	bool stop_flag;

	void run()	override;
	void sleep(unsigned int msec);

};

struct received_info {
	VCI_CAN_OBJ vco[2500];
	int noframe;
};

Q_DECLARE_METATYPE(received_info);