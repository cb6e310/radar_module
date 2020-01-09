#include "can_thread.h"
#include "global_buffer.h"

Can_thread::Can_thread(QObject *parent)
	: QThread(parent)
{
	stop_flag = false;
	qRegisterMetaType<received_info>("received_info");
	qRegisterMetaType<DWORD>("DWORD");
}

Can_thread::~Can_thread() {

}

void Can_thread::stop() {
	stop_flag = true;
}

//use channel_2 to receive, and use channel_1 to send
void Can_thread::open_CAN_device() {
	qDebug() << "open_Can_device()";
	if (!VCI_OpenDevice(4, 0, 0)) {
		emit sig_init_CAN_failed(0);
	}
	else {

		qDebug() << "Open CAN device successfully";
	}

	//clear the buffer of two channel
	if (!VCI_ClearBuffer(4, 0, 0) || !VCI_ClearBuffer(4, 0, 1)) {
		emit sig_init_CAN_failed(1);
	}
	else {
		qDebug() << "buffer cleared";
	}
	VCI_INIT_CONFIG vic;
	vic.AccCode = 0x80000008;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 1;
	vic.Mode = 0;

	std::map<int, std::pair<int, int>> baudRate_list;
	baudRate_list[10] = std::make_pair(0x31, 0x1c);
	baudRate_list[20] = std::make_pair(0x18, 0x1c);
	baudRate_list[40] = std::make_pair(0x87, 0xFF);
	baudRate_list[50] = std::make_pair(0x09, 0x1C);
	baudRate_list[80] = std::make_pair(0x83, 0xFF);
	baudRate_list[100] = std::make_pair(0x04, 0x1C);
	baudRate_list[125] = std::make_pair(0x03, 0x1c);
	baudRate_list[200] = std::make_pair(0x81, 0xfa);
	baudRate_list[250] = std::make_pair(0x01, 0x1c);
	baudRate_list[400] = std::make_pair(0x80, 0xfa);
	baudRate_list[500] = std::make_pair(0x00, 0x1c);
	baudRate_list[666] = std::make_pair(0x80, 0xb6);
	baudRate_list[800] = std::make_pair(0x00, 0x16);
	baudRate_list[1000] = std::make_pair(0x00, 0x14);
	baudRate_list[33] = std::make_pair(0x09, 0x6f);
	baudRate_list[66] = std::make_pair(0x04, 0x6f);
	baudRate_list[83] = std::make_pair(0x03, 0x6f);

	vic.Timing0 = baudRate_list[baud_rate].first;
	vic.Timing1 = baudRate_list[baud_rate].second;
	qDebug() << "baud_rate:" << vic.Timing0 << " " << vic.Timing1;

	if (!VCI_InitCAN(4, 0, 0, &vic) || !VCI_InitCAN(4, 0, 1, &vic)) {
		emit sig_init_CAN_failed(2);
	}
	else {
		qDebug() << "CAN init successfully";
	}
	VCI_BOARD_INFO vbi;
	if (!VCI_ReadBoardInfo(4, 0, &vbi)) {
		emit sig_init_CAN_failed(3);
	}
	else {
		qDebug() << QString::fromLocal8Bit("CAN通道数：") << vbi.can_Num;
		qDebug() << QString::fromLocal8Bit("硬件版本号:") << vbi.hw_Version;
		qDebug() << QString::fromLocal8Bit("接口库版本号：") << vbi.in_Version;
		qDebug() << QString::fromLocal8Bit("中断号:") << vbi.irq_Num;
	}
	if (!VCI_StartCAN(4, 0, 1) || !VCI_StartCAN(4, 0, 0)) {
		emit sig_init_CAN_failed(4);
	}
	else {
		qDebug() << "CAN started";
	}
}

void Can_thread::transmit_cfg() {

}

void Can_thread::close_connection() {
	VCI_CloseDevice(4, 0);
	//todo
}

void Can_thread::check_connection_status() {

}

void Can_thread::run() {
	qDebug() << "second thread running...";
	while (!stop_flag) {
		received_info info;
		mtx.lock();
		qDebug() << "mtx locked";
		if (info.noframe = VCI_Receive(4, 0, 1, info.vco, 2500, 0)) {
			global_buffer.append(info);
			emit sig_received_frame();
			qDebug() << "aha, a frame received.";
			/*
			QByteArray ba;
			ba.resize(info.noframe * sizeof(VCI_CAN_OBJ));
			*/
		}
		mtx.unlock();
		sleep(1000);
	}
	stop_flag = false;
}

void Can_thread::sleep(unsigned int msec) {
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
