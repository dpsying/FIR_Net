#include "server.h"
#include "qdatastream.h"
#include "MemMgr.h"

Server::Server(QObject *parent,int port)
    :QTcpServer(parent)
{
    listen(QHostAddress::Any,port);

	m_parent = parent;
}

void Server::incomingConnection(int socketDescriptor)
{
	ClientSocket *pClient=new ClientSocket(this);
	connect(pClient,SIGNAL(readyRead()),pClient,SLOT(slotReceived()));
	connect(pClient,SIGNAL(disconnected()),pClient,SLOT(slotDisconnected()));
	connect(pClient,SIGNAL(disconnected()),pClient,SLOT(deleteLater())); 

	connect(pClient,SIGNAL(sigUpdateText(QString)),m_parent,SLOT(slotUpdateText(QString)));

	pClient->setSocketDescriptor(socketDescriptor);

}

