#include "concreteCopy.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <QWaitCondition>
#include <QMutex>
#include <QFile>
#include <Windows.h>
#include <QDebug>

const __int64 operateSize = 1024*1024;
const __int64 BufferSize = 64*1024*1024;
char buffer[BufferSize];
QWaitCondition bufferNotEmpty;
QWaitCondition bufferNotFull;
QMutex mutex;


Producer::Producer(QString readFile,QObject *parent /*= NULL*/)
{
	this->readFile = readFile;
}

Producer::~Producer()
{

}

void Producer::run()
{
	QFile inFile(readFile);
	if (!inFile.open(QIODevice::ReadOnly))
	{
		return;
	}
	__int64 perfreq;
	__int64 startpercounts;
	__int64 curpercounts;
	__int64 fileSize = dataSize;
	__int64 operateUnit;
	producerList.clear();
	
	for (__int64 i=0; i<fileSize;)
	{
		mutex.lock();
		if (fileSize - i>operateSize)
			operateUnit = operateSize;
		else
			operateUnit = fileSize - i;
		if (numUsedBytes==BufferSize)
			bufferNotFull.wait(&mutex);
		mutex.unlock();

		QueryPerformanceFrequency((LARGE_INTEGER*)&perfreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&startpercounts);
		int realRead = inFile.read(buffer+i%BufferSize,operateUnit);
		QueryPerformanceCounter((LARGE_INTEGER*)&curpercounts);
		if (realRead == -1)//读取出错
		{
			inFile.close();
			return;
		}
		double sec = (double)(curpercounts - startpercounts) / perfreq;
		producerList.push_back(sec);
		//qDebug()<<"producer"<<sec<<endl;
		mutex.lock();
		if(sec > maxCopySpeed.at(maxCopySpeed.size()-1))
			maxCopySpeed[maxCopySpeed.size()-1] = sec;
		i += operateUnit;
		numUsedBytes += operateUnit;
		bufferNotEmpty.wakeAll();
		mutex.unlock();
	}
	producerVector.push_back(producerList);
	inFile.close();
}

void Producer::setFileName(QString readFile)
{
	this->readFile = readFile;
}

Consumer::Consumer( QString writeFile,QObject *parent/* = NULL*/)
{
	this->writeFile = writeFile;
}

Consumer::~Consumer()
{

}

void Consumer::run()
{
	QFile outFile(writeFile);
	if (!outFile.open(QIODevice::WriteOnly))
	{
		return;
	}
	__int64 perfreq;
	__int64 startpercounts;
	__int64 curpercounts;
	__int64 fileSize = dataSize;
	__int64 operateUnit;
	consumerList.clear();

	for (__int64 i=0;i<fileSize;)
	{
		mutex.lock();
		if (fileSize - i>operateSize)
			operateUnit = operateSize;
		else
			operateUnit = fileSize - i;
		if (numUsedBytes ==0)	
			bufferNotEmpty.wait(&mutex);
		mutex.unlock();

		QueryPerformanceFrequency((LARGE_INTEGER*)&perfreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&startpercounts);
		int realWrite = outFile.write(buffer+i%BufferSize,operateUnit);
		QueryPerformanceCounter((LARGE_INTEGER*)&curpercounts);
		if (realWrite ==-1)//写入出错
		{
			outFile.close();
			return ;
		}
		double sec = (double)(curpercounts - startpercounts) / perfreq;
		consumerList.push_back(sec);
		//qDebug()<<"consumer"<<sec<<endl;
		mutex.lock();
		if(sec > maxCopySpeed.at(maxCopySpeed.size()-1))
			maxCopySpeed[maxCopySpeed.size()-1] = sec;
		i += operateUnit;
		numUsedBytes -=operateUnit;
		bufferNotFull.wakeAll();
		mutex.unlock();	
	}
	consumerVector.push_back(consumerList);
	outFile.close();
}

void Consumer::setFileName(QString writeFile)
{
	this->writeFile = writeFile;
}