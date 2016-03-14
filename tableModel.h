//tableModel.h
#pragma once
#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include<QAbstractTableModel>
#include <QModelIndex>
#include <QList>
#include "global.h"

class TableModel :public QAbstractTableModel
{
	Q_OBJECT
public:
	TableModel(QObject *parent = NULL);
	~TableModel();
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role  /* = Qt::DisplayRole */ )const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
public slots:
	void clearTableData();
	void addItem(TableData tableItem);
private:
	QList<TableData> tableItems;
};
#endif
//tableModel.h