#include "Client.h"

#include <QTime>
#include <QString>
#include "qdatastream.h"
#include "qglobal.h"
#include <QUuid>
#include <QBuffer>

Client::Client(const QString &host, int port, QObject *parent)
    :QObject(parent)
{
	//_blockSize = 0;
	m_nRequired = sizeof(MSG_HEADER);
	m_bIsHeader = true;
	m_pData = NULL;

	m_strGUID = QUuid::createUuid().toString();
	m_nRoom = 0;
	m_bBlack = true;

	connectSignalsAndSlots();
    connectToServer(host, port);

	QueryLogon();
}


Client::Client(QObject *parent)
	:QObject(parent)
{
	m_nRequired = sizeof(MSG_HEADER);
	m_bIsHeader = true;
	m_pData = NULL;

}

void Client::connectSignalsAndSlots()
{
	connect(&_socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
	connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
}

void Client::connectToServer(const QString &host, const int port)
{
    if( ! _socket.isOpen() )
	{
        _socket.connectToHost(host, port);
    }
	else
	{
        qWarning() << "Connection is already open.";
    }
}

void Client::slotReadyRead()
{
	int nRequired = m_nRequired;
	int nRemain = _socket.bytesAvailable();

	while ( nRemain >= nRequired )
	{
		nRemain -= nRequired;

		char* pBuf = new char[nRequired];
		_socket.read(pBuf,nRequired);

		if (m_bIsHeader)
		{
			MSG_HEADER* pHeader = (MSG_HEADER*)pBuf;
			//申请整个消息需要的大小，然后把头先存进去
			m_pData = new char[sizeof(MSG_HEADER) + pHeader->nBodyLen];		
			memset(m_pData,0,sizeof(MSG_HEADER) + pHeader->nBodyLen);
			memcpy(m_pData, pBuf, sizeof(MSG_HEADER) );

			if (pHeader->nBodyLen == 0)
			{
				//do handle msg
				DoHandle(pHeader);

				m_bIsHeader = true;
				nRequired = sizeof(MSG_HEADER);

				delete [] m_pData;
				m_pData = NULL;
			}
			else
			{
				m_bIsHeader = false;
				nRequired = pHeader->nBodyLen;
			}
		}
		else
		{
			MSG_HEADER* pHeader = (MSG_HEADER*)m_pData;
			memcpy( (char*)m_pData + sizeof(MSG_HEADER), pBuf, pHeader->nBodyLen );

			//do handle msg
			DoHandle(pHeader);

			m_bIsHeader = true;
			nRequired = sizeof(MSG_HEADER);

			delete[] m_pData;
			m_pData = NULL;
		}
		m_nRequired = nRequired;

		delete[] pBuf;
		pBuf = NULL;
	}

}

void Client::slotError(QAbstractSocket::SocketError error)
{
    QString errorMessage;

    if(      error == QAbstractSocket::HostNotFoundError )
        errorMessage = "The host was not found.";
    else if( error == QAbstractSocket::RemoteHostClosedError )
        errorMessage = "The remote host is closed.";
    else if( error == QAbstractSocket::ConnectionRefusedError )
        errorMessage = "The connection was refused.";
    else
        _socket.errorString();

    qCritical() << errorMessage;
}

bool Client::QueryLogon()
{
	if( !_socket.isOpen() )
	{
		return false;
	}

	CLIENT_INFO info;
	info.nRoom = m_nRoom;

	int dwHeaderLen = sizeof(MSG_HEADER);
	int dwInfoLen = sizeof(CLIENT_INFO);

	MSG_HEADER header;
	memset(&header,0,dwHeaderLen);
	header.msgType = T_QUERY_LOGON;
	header.nBodyLen = dwInfoLen;
	strcpy_s(header.szGUID,40,m_strGUID.toStdString().c_str());

	int dwTotalLen = dwHeaderLen + dwInfoLen;
	char* pBuf = new char[dwTotalLen];
	if (NULL == pBuf)
	{
		return false;
	}
	memset(pBuf,0,dwHeaderLen);
	memcpy(pBuf,&header,dwHeaderLen);
	memcpy(pBuf + dwHeaderLen,&info,dwInfoLen);

	_socket.write(pBuf,dwTotalLen);

	delete []pBuf;
	return true;
}

bool Client::QueryMove(int x,int y)
{
	if( !_socket.isOpen() )
	{
		return false;
	}

	int dwHeaderLen = sizeof(MSG_HEADER);

	MSG_HEADER header;
	memset(&header,0,dwHeaderLen);
	header.msgType = T_QUERY_MOVE;
	header.nBodyLen = 0;
	strcpy_s(header.szGUID,40,m_strGUID.toStdString().c_str());
	header.nReserved0 = x;
	header.nReserved1 = y;

	_socket.write((char*)&header,dwHeaderLen);

	return true;
}

bool Client::QuerySuccess()
{
	if( !_socket.isOpen() )
	{
		return false;
	}

	MSG_HEADER header;
	memset(&header,0,sizeof(MSG_HEADER));
	header.msgType = T_QUERY_SUCCESS;
	header.nBodyLen = 0;
	strcpy_s(header.szGUID,40,m_strGUID.toStdString().c_str());
	_socket.write((char*)&header,sizeof(MSG_HEADER));

	return true;
}

void Client::DoHandle(MSG_HEADER* pHeader)
{
	if (pHeader == NULL || pHeader->bResult == false)
	{
		return ;
	}
	switch (pHeader->msgType)
	{
	case T_ACK_LOGON:
		{
			OnAckLogon(pHeader);
			break;
		}
	case T_ACK_START:
		{
			OnAckStart(pHeader);
			break;
		}
	case T_ACK_MOVE:
		{
			OnAckMove(pHeader);
			break;
		}
	default:
		break;
	}
}

void Client::OnAckLogon(MSG_HEADER* pHeader)
{
	CLIENT_INFO* pInfo = (CLIENT_INFO*)(pHeader + 1);
	m_bBlack = pInfo->bBlack;
}

void Client::OnAckStart(MSG_HEADER* pHeader)
{

}

void Client::OnAckMove(MSG_HEADER* pHeader)
{
	emit OtherUserMove(pHeader->nReserved0,pHeader->nReserved1);
}

bool Client::IsBlack()
{
	return m_bBlack;
}






