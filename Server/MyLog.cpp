#include "MyLog.h"
#include <QtDebug>
#include <QFile>
#include <QTime>
#include "qlocale.h"
#include "qcoreapplication.h"


QMutex g_Mutex;

// 自定义消息处理程序
void LogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	//QString str = time.toString("[yyyy-MM-dd hh:mm:ss ddd] "); 
	QString str = time.toString("[hh:mm:ss]"); 

	switch (type) 
	{
	case QtDebugMsg:
		str += "[Debug] ";
		break;
	case QtInfoMsg:
		str += "[Info] ";
		break;
	case QtWarningMsg:
		str += "[Warning] ";
		break;
	case QtCriticalMsg:
		str += "[Critical] ";
		break;
	case QtFatalMsg:
		str += "[Fatal] ";
		break;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	str = str + msg + context.file + QString(" %1 ").arg(context.line) + context.function;
	//str = str + msg ;		//不输出文件、行号、函数等代码信息，只输出调试内容

	g_Mutex.lock();

	//QFile outFile(QCoreApplication::applicationDirPath() + "\\1.log");
	QFile outFile("F:\\1.log");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << str << endl;

	g_Mutex.unlock();
}

MyLog::MyLog(void)
{
	qInstallMessageHandler(LogHandler);
}


MyLog::~MyLog(void)
{
}



