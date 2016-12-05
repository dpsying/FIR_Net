#include "MainWindow.h"
#include <QApplication>
#include "MyLog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	MyLog log;

    MainWindow w;
    w.show();
    
    return a.exec();
}
