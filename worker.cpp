#include "worker.h"

#include <global.h>
#include <QDir>
#include <QtDebug>
#include <QFileInfo>
#include <QDateTime>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

Worker::Worker(QObject *parent /* = NULL */)
{
	m_sFromDir = "";
	m_sToDir = "";
}

Worker::~Worker()
{

}
void Worker::dirTraverse( QString fromDir,QString toDir )
{
	m_sFromDir = fromDir;
	m_sToDir = toDir;
	emit clearTableData();
	//�������
	producerVector.clear();
	consumerVector.clear();
	maxCopySpeed.clear();

	QDateTime time = QDateTime::currentDateTime();
	QString str = time.toString("yyyy-mm-dd hh:mm:ss ddd");
	createLog(str);
	_dirTraverse(fromDir);
}
void Worker::_dirTraverse(QString fromDir)
{
	QDir dir(fromDir);
	foreach(QFileInfo fileInfo,dir.entryInfoList())
	{
		if (fileInfo.isFile())
		{
			copyFile(fileInfo.absoluteFilePath());
		}
		else 
		{
			if (fileInfo.fileName()=="."||fileInfo.fileName()=="..")
			{
				continue;
			}
			createFolder(fileInfo.absoluteFilePath());
			_dirTraverse(fileInfo.absoluteFilePath());
		}
	}
}
void Worker::createFolder(QString folderDir)
{
	QDir dirTmp(m_sFromDir);
	QString relativePath = dirTmp.relativeFilePath(folderDir);
	QString toFilePath = QDir::fromNativeSeparators(m_sToDir);
	if (!QDir(m_sToDir).isRoot())
	{
		toFilePath += "/";
	}
	toFilePath += relativePath;
	QDir().mkpath(toFilePath);
}

void Worker::copyFile(QString fromFilePath)
{
	//����Դ·���ļ�
	QFile inFile(fromFilePath);
	if (!inFile.open(QIODevice::ReadOnly))
	{
		return;
	}
	//��ʼ������
	dataSize = inFile.size();
	numUsedBytes = 0;
	maxCopySpeed.push_back(0.0);
	inFile.close();

	//����Ŀ��·��
	QString fileName = QFileInfo(fromFilePath).fileName();//��ȡ�ļ���
	QString relativePath = QDir(m_sFromDir).relativeFilePath(fromFilePath);//��ȡ���·��
	QString toFilePath = QDir::fromNativeSeparators(m_sToDir);//ת��·��'\'Ϊ��/��
	if (!QDir(m_sToDir).isRoot())
	{
		toFilePath += "/";
	}
	toFilePath += relativePath;//���մ��·��
	QFile outFile(toFilePath);
	if (!outFile.open(QIODevice::WriteOnly))
	{
		return;
	}
	outFile.close();

	__int64 perfreq;
	__int64 startpercounts;
	__int64 curpercounts;
	
	Producer producer(fromFilePath);
	Consumer consumer(toFilePath);
	producer.setFileName(fromFilePath);
	consumer.setFileName(toFilePath);

	QueryPerformanceFrequency((LARGE_INTEGER*)&perfreq);
	QueryPerformanceCounter((LARGE_INTEGER*)&startpercounts);
	producer.start();
	consumer.start();
	producer.wait();
	consumer.wait();
	QueryPerformanceCounter((LARGE_INTEGER*)&curpercounts);
	double sec = (double)(curpercounts - startpercounts) / perfreq;

	qRegisterMetaType<TableData>("TableData");
	TableData tableItem;
	tableItem.fileName = fileName;
	tableItem.fileSize = QFileInfo(fromFilePath).size();
	tableItem.cpoyTime = sec;
	if (tableItem.fileSize != 0)
		tableItem.speedPerMB = tableItem.cpoyTime/tableItem.fileSize;
	else
		tableItem.speedPerMB = 0;
	createLog(fromFilePath,toFilePath);

	emit resultReady(tableItem);

}