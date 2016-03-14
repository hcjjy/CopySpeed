#include "global.h"
#include <iostream>
#include <fstream>
#include<iomanip>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

__int64 dataSize = 0;
int numUsedBytes = 0;
QList<double> producerList;
QList<double> consumerList;
QVector<QList<double> >producerVector;
QVector<QList<double> >consumerVector;
int showRow = 0;
double showFileSize =0.0;
std::vector<double>maxCopySpeed;
bool createLog(QString str)
{
	QFile outFile("copyLog.txt");
	if (!outFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
	{
		return false;
	}
	QTextStream out(&outFile);
	out<<"\n"<<str<<endl;
	outFile.close();
	return true;
}
bool createLog(QString fromFilePath,QString toFilePath)
{
	int i,j;
	QFile outFile("copyLog.txt");
	if (!outFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
	{
		return false;
	}
	QTextStream out(&outFile);
	out<<fromFilePath<<"\t"<<toFilePath<<endl;

	
	for(i=0,j=0;i<producerList.size()&&j<consumerList.size();i++,j++)
	{
		if (i!=0&&i%16==0)
		{
			out.reset();
			out<<endl;
		}
		out<<qSetFieldWidth(10)<<left<<QString::number(producerList[i],'f',6)
		   <<QString::number(consumerList[j],'f',6);
	}
	if (i%16!=0)
	{
		out.reset();//设置字符宽度会影响endl，所以必须先重置标志
		out<<endl;
	}
	outFile.close();
	return true;
}
