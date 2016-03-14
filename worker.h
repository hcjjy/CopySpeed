#pragma once
#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include "global.h"
#include "concreteCopy.h"

class Worker :public QObject
{
	Q_OBJECT
public:
	Worker(QObject *parent = NULL);
	virtual ~Worker();
	void createFolder(QString folderDir);
	void copyFile(QString fromFilePath);
	void _dirTraverse(QString fromDir);
signals:
	void clearTableData();
	void resultReady(TableData tableItem);
public slots:
	void dirTraverse(QString fromDir,QString toDir);
private:
	QString m_sFromDir;
	QString m_sToDir;
};
#endif