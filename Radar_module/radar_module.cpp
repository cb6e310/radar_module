#include "radar_module.h"

Radar_module::Radar_module(QWidget *parent)
	: QMainWindow(parent), ui_main(new Ui::Radar_moduleClass)//, setting(new Param_setting())
{
	ui_main->setupUi(this);
	can_thread = new Can_thread(Q_NULLPTR);
	//-------------------------create the received frame viewer-----------------------------  

	model1 = new Data_TableModel(this);
	ui_main->table_view_dataDisplay->setModel(model1);
	//----------------------create the received frame info viewer done---------------------------- 

	//----------------------create the DBC info viewer-----------------------------------------
	model2 = new QStandardItemModel(ui_main->tree_view_DBCinfo);
	model2->setHorizontalHeaderLabels(QStringList() << QStringLiteral("消息名") << QStringLiteral("数据") << QStringLiteral("注释"));

	ui_main->tree_view_DBCinfo->setModel(model2);
	//---------------------create the DBC info viewer done------------------------------

	//create a qtAction
	QAction* action_create = new QAction(QString::fromLocal8Bit("新建"), this);

	QAction* action_disconnect = new QAction(QString::fromLocal8Bit("断开"), this);

	//apply the action to the widget on form
	ui_main->menuBar->addAction(action_create);
	ui_main->menuBar->addAction(action_disconnect);

	//connect the action's signal with the corresponding slot 
	qRegisterMetaType<QVariant>("QVariant");
	connect(action_create, &QAction::triggered,
		this, &Radar_module::action_create_triggered);

	connect(can_thread, &Can_thread::sig_received_frame, model1, &Data_TableModel::refresh_with_new_frame);

	//qRegisterMetaType<Connection_cfg>("Connection_cfg");


	/*
	todo
	connect
	*/
}

Radar_module::~Radar_module() {
	delete ui_main;
}

void Radar_module::action_create_triggered() {
	setting = new Param_setting();
	connect(setting, SIGNAL(sig_start_new_connection()), this, SLOT(new_cfg_receive()));
	setting->exec();
}

void Radar_module::action_disconnected_triggered() {
	//todo
	//
}

void Radar_module::action_pause_triggered() {
	//todo
	//
}

void Radar_module::display_connection_status() {
	//todo
	//
}

void Radar_module::new_cfg_receive() {
	//*cfg = qvar.value<Connection_cfg>();
	qDebug() << "cfg->baud_rate: " << cfg->baud_rate;
	auto _msg_vec = cfg->DBC_message_list;

	for (int i = 0; i < _msg_vec.size(); i++) {
		QStandardItem* msg_piece = new QStandardItem(QString(_msg_vec[i].strName));
		QStandardItem* msg_piece_comment = new QStandardItem(QString(_msg_vec[i].strComment));
		model2->setItem(i, 0, msg_piece);
		model2->setItem(i, 2, msg_piece_comment);
		for (int j = 0; j < _msg_vec[i].nSignalCount; j++) {
			QStandardItem* signal_piece_name = new QStandardItem(QString(_msg_vec[i].vSignals[j].strName));
			QStandardItem* signal_piece_value = new QStandardItem(QString::number(_msg_vec[i].vSignals[j].nValue));
			QStandardItem* signal_piece_comment = new QStandardItem(QString(_msg_vec[i].strComment));
			msg_piece->setChild(j, 0, signal_piece_name);
			msg_piece->setChild(j, 1, signal_piece_value);
			msg_piece->setChild(j, 2, signal_piece_comment);
		}
	}
	can_thread->baud_rate = cfg->baud_rate;
	//todo
	can_thread->open_CAN_device();
	can_thread->start();
}

void Radar_module::error_msg_display(int i) {
	//todo
}
