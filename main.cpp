#include "copyspeed.h"
#include <QtGui/QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec *codec = QTextCodec::codecForName("GBK");//���1
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	CopySpeed w;
	w.show();
	return a.exec();
}
