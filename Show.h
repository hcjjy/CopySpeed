#pragma once
#ifndef SHOW_H
#define SHOW_H
#include <QDialog>
class ShowPsnr;
class QScrollArea;
class Show : public QDialog
{
public:
	Show(int showRow,QWidget* parent = NULL);
	virtual ~Show(void);
protected:
	void resizeEvent(QResizeEvent *event);
private:
	ShowPsnr *m_pShowPsnr;
	QScrollArea *m_pScrollArea;
	int showRow_;
};
#endif
