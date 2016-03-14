#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QList>
#include <QVector>

typedef struct TableData
{
	QString fileName;
	__int64 fileSize;
	double cpoyTime;
	double speedPerMB;
}TableData;
typedef struct Speed
{
	double readSpeed;
	double writeSpeed;
}SPeed;
bool createLog(QString);
bool createLog(QString, QString);
extern __int64 dataSize;
extern int numUsedBytes;
extern QList<double> producerList;
extern QList<double> consumerList;
extern QVector<QList<double> >producerVector;
extern QVector<QList<double> >consumerVector;
extern int showRow;
extern double showFileSize;
extern std::vector<double>maxCopySpeed;
#endif