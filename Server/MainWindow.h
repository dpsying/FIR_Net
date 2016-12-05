#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "server.h"
#include <QTextEdit>

class MainWindow : public QDialog
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0,Qt::WindowFlags f=0);
    ~MainWindow();    
private:
    QListWidget *m_ListWidget;
    QLabel *PortLabel;
    QLineEdit *PortLineEdit;
    QPushButton *CreateBtn;
    QGridLayout *mainLayout;
    int port;
    Server *server;
public slots:
   // void slotCreateServer();
    void slotUpdateText(QString);
};

#endif // TCPSERVER_H
