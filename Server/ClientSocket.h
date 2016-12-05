#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include "..\Common\common.h"

class ClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    ClientSocket(QObject *parent=0);

	quint16 m_nID;

	bool m_bIsHeader;
	int m_nRequiredSize;
	char* m_pData;

	QString m_strGUID;
	int m_nRoom;
	bool m_bBlack;

signals:
	void sigClientDisconnected(QString );
	void sigUpdateText(QString);
protected slots:
    void slotReceived();
    void slotDisconnected();
	
private:
	void DoHandle(char* pData,int nBufLen);
	void OnQueryLogon(char* pData,int nBufLen);
	void OnQueryMove(char* pData,int nBufLen);
	void OnQuerySuccess(char* pData,int nBufLen);
};

#endif 