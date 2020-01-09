#pragma once

#include <QDialog>
#include <qdebug.h>
#include <Qstring>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <map>
#include "ui_param_setting.h"
#include "connection_cfg.h"

class Param_setting : public QDialog
{
	Q_OBJECT

public:
	Param_setting(QWidget *parent = Q_NULLPTR);
	~Param_setting();

private slots:
	void on_start_connect_button_clicked();
	void on_read_DBC_file_button_clicked();
	void error_msg_display(int i);
	void baud_rate_changed(const QString&);


signals:
	void sig_DBC_init_failed(int);
	void sig_start_new_connection(QVariant);

private:
	Ui::Param_setting* ui_setting;
	Connection_cfg* cfg_tmp;
	void analyze_DBC_file();
	static void OnSendFunc(void* ctx, void* pObj);
};
