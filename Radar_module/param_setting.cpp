#include "param_setting.h"

Param_setting::Param_setting(QWidget *parent)
	: QDialog(parent), ui_setting(new Ui::Param_setting)
{
	ui_setting->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose, 1);	//delete this when close()

	connect(this, &Param_setting::sig_DBC_init_failed, this, &Param_setting::error_msg_display);

	connect(ui_setting->baud_rate, SIGNAL(currentIndexChanged(const QString&)), 
		this, SLOT(baud_rate_changed(const QString&)));

	cfg->m_hDBC = DBC_Init();

	if (INVALID_DBC_HANDLE == cfg->m_hDBC) {
		qDebug() << 1;
		emit sig_DBC_init_failed(1);
	}

	cfg->callback_ctx.owner = this;
	cfg->callback_ctx.devinfo = { 4, 0, 0 };

	DBC_SetSender(cfg->m_hDBC, OnSendFunc, &cfg->callback_ctx);
}

Param_setting::~Param_setting()
{
	delete ui_setting;
	//todo
}

void Param_setting::OnSendFunc(void* ctx, void* pObj) {
	Ctx* info = static_cast<Ctx*>(ctx);
	VCI_CAN_OBJ* obj = static_cast<VCI_CAN_OBJ*>(pObj);
	VCI_Transmit(info->devinfo.nDevType, info->devinfo.nDevIndex, info->devinfo.nChIndex, obj, 1);
}

void Param_setting::on_start_connect_button_clicked() {
	/*
	QVariant qvar;
	qvar.setValue(*cfg);
	*/
	emit sig_start_new_connection();

	//cfg->clear();
	//start CAN thread
	//send radar_cfg

	this->close();
}

void Param_setting::on_read_DBC_file_button_clicked() {
	//set QSettings file
	QSettings setting("./Setting.ini", QSettings::IniFormat);

	//get the last file directory in setting file
	QString last_path = setting.value("LastFilePath").toString();
	qDebug() << last_path;


	ui_setting->textEdit_DBC_dir->clear();

	//get the DBC file directory
	//getOpenFileName(parent widget, browser header, the entry dir. the file filter)
	QString path = QDir::toNativeSeparators(
		QFileDialog::getOpenFileName(this, "DBC file Browser", last_path, tr("DBC file (*.dbc)")));
	ui_setting->textEdit_DBC_dir->setText(path);

	setting.setValue("LastFilePath", path);  //record the path to the setting.ini

											 //in order not to get the messy code caused by chinese char, 
											 //please make sure the dbc file directory has not included chinese char
	strcpy_s(cfg->file_info.strFilePath, _MAX_FILE_PATH_, path.toStdString().c_str());
	qDebug() << cfg->file_info.strFilePath;
	cfg->file_info.nType = FileInfo::DBC_J1939;
	if (!DBC_LoadFile(cfg->m_hDBC, &cfg->file_info)) {
		qDebug() << "1111";
		emit sig_DBC_init_failed(2);
		DBC_Release(cfg->m_hDBC);
	}
	analyze_DBC_file();
}

void Param_setting::analyze_DBC_file() {
	if (0 == DBC_GetMessageCount(cfg->m_hDBC)) {
		qDebug() << "2222";
		emit sig_DBC_init_failed(3);
	}
	DBCMessage _msg;
	if (DBC_GetFirstMessage(cfg->m_hDBC, &_msg))
	{
		while (DBC_GetNextMessage(cfg->m_hDBC, &_msg))
		{
			cfg->DBC_message_list.push_back(_msg);
		}
	}
}

void Param_setting::baud_rate_changed(const QString& str) {
	qDebug() << str;
	cfg->baud_rate = str.toInt();
	qDebug() << "cfg->baud_rate: " << cfg->baud_rate;
}

//error information box
void Param_setting::error_msg_display(int i) {
	std::map<int, QString> error_type;
	int result;
	error_type[1] = QString::fromLocal8Bit("ºÇºÇ£¬Éú³ÉDBC¾ä±ú´íÎó£¡");
	error_type[2] = QString::fromLocal8Bit("ºÇºÇ£¬´ò¿ªDBCÎÄ¼þÊ§°Ü£¡");
	error_type[3] = QString::fromLocal8Bit("ºÇºÇ£¬DBCÎÄ¼þÎª¿Õ£¡");
	qDebug() << 2;
	//todo
	switch (i) {
	case 1:
		result = QMessageBox::critical(this, QString::fromLocal8Bit("´íÎó£¡"), error_type[1],
			QMessageBox::Ok);
		if (result == QMessageBox::Ok) {
			//todo
		}
		break;
	case 2:
		result = QMessageBox::critical(this, QString::fromLocal8Bit("´íÎó£¡"), error_type[2],
			QMessageBox::Ok);
		if (result == QMessageBox::Ok) {
			//this->close();
		}
		break;
	case 3:
		result = QMessageBox::critical(this, QString::fromLocal8Bit("´íÎó£¡"), error_type[3],
			QMessageBox::Ok);
		if (result == QMessageBox::Ok) {
			//this->close();
		}
		break;
	}
}
