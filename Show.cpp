#include "Show.h"
#include "ShowPsnr.h"
#include "global.h"
#include <QScrollArea>
#include <QScrollBar>
Show::Show(int showRow,QWidget* parent /*= NULL*/)
	:QDialog(parent)
{
	resize(800,600);
	showRow_ = showRow;
	m_pScrollArea = new QScrollArea(this);
	m_pShowPsnr = new ShowPsnr;
	m_pScrollArea->setGeometry(30,30,width()-60,height()-60);
	m_pShowPsnr->setGeometry(0,0,4*showFileSize+100,5000*maxCopySpeed[showRow_]+100);
	if (m_pShowPsnr->width()<m_pScrollArea->width())
	{
		m_pShowPsnr->resize(m_pScrollArea->width(),m_pShowPsnr->height());
	}
	if (m_pShowPsnr->height()<m_pScrollArea->height())
	{
		m_pShowPsnr->resize(m_pShowPsnr->width(),m_pScrollArea->height());
	}
	m_pScrollArea->setWidget(m_pShowPsnr);
	QScrollBar *scrollBar = m_pScrollArea->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

Show::~Show(void)
{

}

void Show::resizeEvent( QResizeEvent *event )
{
	m_pScrollArea->setGeometry(30,30,width()-60,height()-60);
	m_pShowPsnr->setGeometry(0,0,4*showFileSize+100,5000*maxCopySpeed[showRow_]+100);
	if (m_pShowPsnr->width()<m_pScrollArea->width())
	{
		m_pShowPsnr->resize(m_pScrollArea->width(),m_pShowPsnr->height());
	}
	if (m_pShowPsnr->height()<m_pScrollArea->height())
	{
		m_pShowPsnr->resize(m_pShowPsnr->width(),m_pScrollArea->height());
	}
}
