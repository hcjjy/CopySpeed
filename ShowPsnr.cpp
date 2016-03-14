#include "ShowPsnr.h"
#include "global.h"
#include <QtGui/QtGui>

ShowPsnr::ShowPsnr(QWidget* parent /* = NULL */)
	:QWidget(parent)
{

}

ShowPsnr::~ShowPsnr()
{

}

void ShowPsnr::paintEvent(QPaintEvent *event)
{

	QPainter painter(this);
	painter.setPen(QPen(QBrush(QColor(Qt::blue)),1));
	painter.setRenderHint(QPainter::Antialiasing,true);
	int row = showRow;
	if (row >= producerVector.size()||row >= consumerVector.size())
	{
		return;
	}
	QList<double >producerShowList = producerVector.value(row);
	QList<double >consumerShowList = consumerVector.value(row);
	double width = this->width();
	double height = this->height(); 
	double offset = 30;
	double spanXValue = 4;
	double magnifyYValue = 5000;
	double xPre = 0;
	double pyPre = 0;
	double cyPre = 0;
	double x = 0 ;

	//paint X-Y coordinate
	painter.setPen(QPen(QBrush(QColor(Qt::black)),1));
	painter.drawLine(QPoint(offset,height - offset),QPoint(width -50,height - offset));
	painter.drawLine(QPoint(width -50,height - offset),QPoint(width -50-10,height - offset-10));
	painter.drawLine(QPoint(width -50,height - offset),QPoint(width -50-10,height - offset+10));
	painter.drawText(QPoint(width -50-10,height - offset+10+5),QString("单位：M"));

	painter.drawLine(QPoint(offset,height - offset),QPoint(offset,50));
	painter.drawLine(QPoint(offset,50),QPoint(offset -10,50 +10));
	painter.drawLine(QPoint(offset,50),QPoint(offset +10,50 +10));
	painter.drawText(QPoint(offset-5,50-5),QString("单位：S/M"));

	//zero coordinate
	painter.setPen(QPen(QBrush(QColor(Qt::magenta)),3));
	painter.drawText(QPoint(offset-15,height - offset+10),QString::number(0));

	//绘制x轴单位坐标
	for (int m=offset;m<width -50;m++)
	{
		if (m!=offset && m%(int)(spanXValue*10)==offset)
		{
			painter.setPen(QPen(QBrush(QColor(Qt::black)),1));
			painter.drawLine(QPoint(m,height-offset),QPoint(m,height-offset+5));
			painter.setPen(QPen(QBrush(QColor(Qt::magenta)),3));
			painter.drawText(QPoint(m-5,height-offset+15),QString::number((m-offset)*1.0/spanXValue));
		}
	}
	//绘制y轴单位坐标
	for (int n=offset;n<height -50;n++)
	{
		if (n!=offset && (n-30)%10 == 0)
		{
			painter.setPen(QPen(QBrush(QColor(Qt::black)),1));
			painter.drawLine(QPoint(offset,height-n),QPoint(offset-5,height-n));
			painter.setPen(QPen(QBrush(QColor(Qt::magenta)),3));
			painter.drawText(QPoint(offset-30,height-n),QString::number((n-offset)/magnifyYValue));
		}
	}

	for (int i = 0,j = 0; i<producerShowList.size()&&j<consumerShowList.size();i++,j++)
	{
		x += spanXValue;
		double py = producerShowList.value(i)*magnifyYValue;
		double cy = consumerShowList.value(j)*magnifyYValue;
		//translate coordinate 
		double xPreWid = xPre + offset;
		double xWid = x + offset;
		double pyPreWid = height - pyPre - offset;
		double cyPreWid = height - cyPre - offset;
		double pyWid = height - py - offset;
		double cyWid = height - cy - offset;

		painter.setPen(QPen(QBrush(QColor(Qt::blue)),1));
		painter.drawLine(QPoint(xPreWid,pyPreWid),QPoint(xWid,pyWid));
		painter.setPen(QPen(QBrush(QColor(Qt::red)),1));
		painter.drawLine(QPoint(xPreWid,cyPreWid),QPoint(xWid,cyWid));

		xPre = x;
		pyPre = py;
		cyPre = cy;
	}
}
