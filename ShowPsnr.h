#pragma once
#ifndef SHOWPSNR_H
#define SHOWPSNR_H
#include <QDialog>
class QPaintEvent;

class ShowPsnr:public QWidget
{
	Q_OBJECT
public:
	ShowPsnr(QWidget* parent = NULL);
	virtual ~ShowPsnr();
protected:
	virtual void paintEvent(QPaintEvent *event);
};
#endif