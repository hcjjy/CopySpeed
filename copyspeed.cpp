#include "copyspeed.h"
#include "worker.h"
#include "tableModel.h"
#include "Show.h"
#include <QtGui/QtGui>
#include <QFile>
#include <QDir>
#include <QModelIndex>

bool isValidDir(QString fromDir,QString toDir)
{
	if (fromDir.isEmpty()||toDir.isEmpty())
		return false;
	return QDir(fromDir).exists() && QDir(toDir).exists();
}

CopySpeed::CopySpeed(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	createAllElement();
	resize(800,600);
	createAllConnect();
	m_pTableView->setModel(m_pTableModel);
}
	
CopySpeed::~CopySpeed()
{
	workerThread.quit();
	workerThread.wait();
}
void CopySpeed::createAllElement()
{
	m_pBtnLocalDir = new QPushButton(tr("本地目录"),this);
	m_pBtnHDFSDir = new QPushButton(tr("HDFS目录"),this);
	m_pBtnRealDir = new QPushButton(tr("实际目录"),this);

	m_pBtnLocalToHDFS = new QPushButton(tr("本地拷贝到HDFS"),this);
	m_pBtnHDFSToLocal = new QPushButton(tr("HDFS拷贝到本地"),this);
	m_pBtnLocalToReal = new QPushButton(tr("本地拷贝到实际"),this);
	m_pBtnRealToLocal = new QPushButton(tr("实际拷贝到本地"),this);

	m_pLELocalDir = new QLineEdit(this);
	m_pLEHDFSDir = new QLineEdit(this);
	m_pLERealDir = new QLineEdit(this);

	worker = new Worker();
	worker->moveToThread(&workerThread);

	m_pTableModel =  new TableModel;
	m_pTableView = new QTableView(this);
	m_pHeaderView = m_pTableView->horizontalHeader();
}
 void CopySpeed::createAllConnect()
 {
	 connect(m_pBtnLocalDir,SIGNAL(clicked()),this,SLOT(onBtnSlot()));
	 connect(m_pBtnHDFSDir,SIGNAL(clicked()),this,SLOT(onBtnSlot()));
	 connect(m_pBtnRealDir,SIGNAL(clicked()),this,SLOT(onBtnSlot()));

	 connect(m_pBtnLocalToHDFS,SIGNAL(clicked()),this,SLOT(copyBtnSlot()));
	 connect(m_pBtnHDFSToLocal,SIGNAL(clicked()),this,SLOT(copyBtnSlot()));
	 connect(m_pBtnLocalToReal,SIGNAL(clicked()),this,SLOT(copyBtnSlot()));
	 connect(m_pBtnRealToLocal,SIGNAL(clicked()),this,SLOT(copyBtnSlot()));

	 connect(&workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));
	 connect(this,SIGNAL(copyFile(QString,QString)),worker,SLOT(dirTraverse(QString,QString)));
	 connect(worker,SIGNAL(clearTableData()),m_pTableModel,SLOT(clearTableData()));
	 connect(worker,SIGNAL(resultReady(TableData)),m_pTableModel,SLOT(addItem(TableData)));
	 connect(m_pTableView,SIGNAL(doubleClicked(const QModelIndex&)),this,SLOT(clickedSlot(const QModelIndex&)));
	
 }
void CopySpeed::onBtnSlot()
{
	
	if (sender() == m_pBtnLocalDir)
	{
		QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			"C:/本地目录/",
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
		if (!dir.isEmpty())
		{
			m_pLELocalDir->setText(dir);
		}
	}
	else if (sender() == m_pBtnHDFSDir)
	{
		QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			"D:/HDFS目录/",
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
		if (!dir.isEmpty())
		{
			m_pLEHDFSDir->setText(dir);
		}
	}
	else if(sender() == m_pBtnRealDir)
	{
		QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			"E:/实际目录/",
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
		if (!dir.isEmpty())
		{
			m_pLERealDir->setText(dir);
		}
	}
}

void CopySpeed::copyBtnSlot()
{
	
	if (sender() == m_pBtnLocalToHDFS)
	{
		QString formDir = m_pLELocalDir->text();
		QString toDir = m_pLEHDFSDir->text();
		if(bool flag = isValidDir(formDir,toDir))
		{
			 workerThread.start();
			 emit copyFile(formDir,toDir);
		}
		else
		{
			QMessageBox::warning(this,tr("警告"),tr("源目录或者目的目录有误"));
		}
	}
	if (sender() == m_pBtnHDFSToLocal)
	{
		QString formDir = m_pLEHDFSDir->text();
		QString toDir = m_pLELocalDir->text();
		if(isValidDir(formDir,toDir))
		{
			workerThread.start();
			emit copyFile(formDir,toDir);
		}
		else
		{
			QMessageBox::warning(this,tr("警告"),tr("源目录或者目的目录有误"));
		}
	}
	if (sender() == m_pBtnLocalToReal)
	{
		QString formDir = m_pLELocalDir->text();
		QString toDir = m_pLERealDir->text();
		if(isValidDir(formDir,toDir))
		{
			workerThread.start();
			emit copyFile(formDir,toDir);
		}
		else
		{
			QMessageBox::warning(this,tr("警告"),tr("源目录或者目的目录有误"));
		}
	}
	if (sender() == m_pBtnRealToLocal)
	{
		QString formDir = m_pLERealDir->text();
		QString toDir = m_pLELocalDir->text();
		if(isValidDir(formDir,toDir))
		{
			workerThread.start();
			emit copyFile(formDir,toDir);
		}
		else
		{
			QMessageBox::warning(this,tr("警告"),tr("源目录或者目的目录有误"));
		}
	}
}
void CopySpeed::clickedSlot(const QModelIndex &index)
{
	showRow = index.row();
	QModelIndex indexSize = m_pTableModel->index(showRow,1);
	double fileSize = indexSize.data().toDouble();
	showFileSize = fileSize/(1024*1024);
	Show showDialog(showRow,this);
	showDialog.exec();
}
void CopySpeed::resizeEvent( QResizeEvent *event )
{
	QRect MainRect = this->rect();
	QRect tmpRect;
	const int magin = 10;
	const int btnWidth = 100;
	const int leWidth = 520;
	const int widHeight = 25;
	const int tableWidth = 640;
	const int tableHeight = 340;
	m_pBtnLocalDir->setGeometry(8*magin,3*magin,btnWidth,widHeight);
	tmpRect = m_pBtnLocalDir->geometry();
	m_pLELocalDir->setGeometry(tmpRect.right()+2*magin,tmpRect.top(),leWidth,tmpRect.height());
	m_pBtnHDFSDir->setGeometry(tmpRect.left(),tmpRect.bottom()+2*magin,tmpRect.width(),tmpRect.height());
	tmpRect = m_pBtnHDFSDir->geometry();
	m_pLEHDFSDir->setGeometry(tmpRect.right()+2*magin,tmpRect.top(),leWidth,tmpRect.height());
	m_pBtnRealDir->setGeometry(tmpRect.left(),tmpRect.bottom()+2*magin,tmpRect.width(),tmpRect.height());
	tmpRect = m_pBtnRealDir->geometry();
	m_pLERealDir->setGeometry(tmpRect.right()+2*magin,tmpRect.top(),leWidth,tmpRect.height());
	
	m_pBtnLocalToHDFS->setGeometry(tmpRect.left(),tmpRect.bottom()+3*magin,tmpRect.width(),tmpRect.height());
	tmpRect = m_pBtnLocalToHDFS->geometry();
	m_pBtnHDFSToLocal->setGeometry(tmpRect.right()+8*magin,tmpRect.top(),btnWidth,tmpRect.height());
	
	m_pTableView->setGeometry(tmpRect.left(),tmpRect.bottom()+3*magin,tableWidth,tableHeight);

	tmpRect = m_pBtnHDFSToLocal->geometry();
	m_pBtnLocalToReal->setGeometry(tmpRect.right()+8*magin,tmpRect.top(),btnWidth,tmpRect.height());
	tmpRect = m_pBtnLocalToReal->geometry();
	m_pBtnRealToLocal->setGeometry(tmpRect.right()+8*magin,tmpRect.top(),btnWidth,tmpRect.height());
	
	m_pHeaderView->resizeSection(0,160);
	m_pHeaderView->resizeSection(1,160);
	m_pHeaderView->resizeSection(2,160);
	m_pHeaderView->resizeSection(3,160);
}
