#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include "ClientSocket.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent=0,int port=0);
protected:
	void incomingConnection(int socketDescriptor);

signals:
    //void sigUpdateText(QString);
public slots:
  
private:
	QObject * m_parent;

};

#endif // SERVER_H
