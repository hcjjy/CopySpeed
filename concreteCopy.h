#pragma once
#ifndef CONCRETECOPY_H
#define CONCRETECOPY_H
#include <QThread>

class Producer: public QThread
{
public:
	Producer(QString readFile,QObject *parent = NULL);
	~Producer();
	void run();
	void setFileName(QString readFile);
private:
	QString readFile;
};

class Consumer: public QThread
{
public:
	Consumer(QString writeFile,QObject *parent = NULL);
	~Consumer();
	void run();
	void setFileName(QString writeFile);
private:
	QString writeFile;
};

#endif