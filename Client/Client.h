#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include "..\Common\common.h"

class Client : public QObject
{
    Q_OBJECT

public:
    Client(const QString& host, int port, QObject* parent = 0);
    Client(QObject* parent = 0);

	void connectSignalsAndSlots();
    void connectToServer(const QString &host, const int port);
	bool IsBlack();

	bool QueryLogon();
	bool QueryMove(int x,int y);
	bool QuerySuccess();

	void DoHandle(MSG_HEADER* pHeader);
	void OnAckLogon(MSG_HEADER* pHeader);
	void OnAckStart(MSG_HEADER* pHeader);
	void OnAckMove(MSG_HEADER* pHeader);

signals:
	void OtherUserMove(int x,int y);

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError error);
   
private:
	QTcpSocket _socket;

	//客户端
	int m_nID;
	QString m_strGUID;
	int m_nRoom;
	bool m_bBlack;

	//收数据的相关控制变量
	int m_nRequired;
	int m_bIsHeader;
	char* m_pData;
};

#endif // TCPCLIENT_H
