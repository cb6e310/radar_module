#pragma once

#include <QtWidgets/QMainWindow>
#include <QstandardItemModel>
#include <Qstring>
#include <QMetaType>
#include "ui_radar_module.h"
#include "param_setting.h"
#include "connection_cfg.h"
#include "can_thread.h"
#include "IDBCManager.h"
#include "global_buffer.h"
#include "data_tablemodel.h"

class Radar_module : public QMainWindow
{
	Q_OBJECT

public:
	Radar_module(QWidget *parent = Q_NULLPTR);
	~Radar_module();

private slots:
	void action_create_triggered();				//new param_setting dialog
	void action_disconnected_triggered();
	void action_pause_triggered();
	//void display_DBC_info();
	//void receive_frame(QVariant);
	void display_connection_status();
	void new_cfg_receive(QVariant);
	void error_msg_display(int);

private:
	Ui::Radar_moduleClass* ui_main;
	Param_setting *setting;
	Can_thread *can_thread;
	Connection_cfg *cfg;
	Data_TableModel *model1;
	QStandardItemModel* model2;
};
