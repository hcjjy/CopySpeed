#ifndef COPYSPEED_H
#define COPYSPEED_H

#include <QtGui/QWidget>
#include <QModelIndex>
#include <QThread>
class QPushButton;
class QLineEdit;
class Worker;
class QTableView;
class TableModel;
class QHeaderView;
class CopySpeed : public QWidget
{
	Q_OBJECT

public:
	CopySpeed(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CopySpeed();
signals:
	void copyFile(QString fromDir,QString toDir);
protected:
	virtual void resizeEvent(QResizeEvent *event);
public slots:
	void onBtnSlot();
	void copyBtnSlot();
	void clickedSlot(const QModelIndex &index);
private:
	void createAllElement();
	void createAllConnect();
private:
	QPushButton *m_pBtnLocalDir;
	QPushButton *m_pBtnHDFSDir;
	QPushButton *m_pBtnRealDir;

	QPushButton *m_pBtnLocalToHDFS;
	QPushButton *m_pBtnHDFSToLocal;
	QPushButton *m_pBtnLocalToReal;
	QPushButton *m_pBtnRealToLocal;

	QLineEdit *m_pLELocalDir;
	QLineEdit *m_pLEHDFSDir;
	QLineEdit *m_pLERealDir;
	QThread workerThread;
	Worker *worker;
	TableModel *m_pTableModel;
	QTableView *m_pTableView;
	QHeaderView *m_pHeaderView;
};

#endif // COPYSPEED_H
