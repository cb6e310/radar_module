#pragma once

#include <QDebug>
#include <QMetaType>
#include <QMessageBox>
#include <Qstring>
#include <vector>
#include "IDBCManager.h"
#include "ControlCAN.h"


struct DevInfo
{
	DWORD nDevType;
	DWORD nDevIndex;
	DWORD nChIndex; //Í¨µÀºÅ
};

struct Ctx
{
	void* owner;
	DevInfo devinfo;
};

struct Connection_cfg
{

	Connection_cfg();
	Connection_cfg(const Connection_cfg&);
	~Connection_cfg();
	//bool clear();
	DBCHandle m_hDBC;
	FileInfo file_info;
	std::vector<DBCMessage> DBC_message_list;
	int baud_rate;
	Ctx callback_ctx;
};

Q_DECLARE_METATYPE(Connection_cfg)
