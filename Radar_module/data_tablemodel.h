#pragma once

#include <QTableView>
#include <QStringList>
#include <QPainter>
#include <QHeaderView>
#include <QAbstractTableModel>
#include "can_thread.h"
#include "IDBCManager.h"

//QAbstractTableModel为模型提供了一个标准接口，这些模型将其数据表示为一个二维数组。它必须子类化。
//在子类化QAbstractTableModel时，必须实现rowCount()、columnCount()和data()。
//index()和parent()函数的默认实现由QAbstractTableModel提供。行为良好的模型还需要实现headerData()。
class Data_TableModel : public QAbstractTableModel
{
	Q_OBJECT

public:

	Data_TableModel(QObject*);
	~Data_TableModel(void);
	void setHorizontalHeaderList(QStringList horizontalHeaderList);
	void setVerticalHeaderList(QStringList verticalHeaderList);
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	void refresh_with_new_frame();
	//void setModalData(QList< QStringList > *rowlist);
	void refrushModel();

signals:

	void updateCount(int count);

private:
	QStringList horizontal_header_list;
	QStringList vertical_header_list;
	QList< QList<QString>> arr_row_list;
};
