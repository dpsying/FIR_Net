#include "ClientSocket.h"
#include "qdatastream.h"
#include <QtDebug>
#include "qthread.h"
#include "MemMgr.h"

ClientSocket::ClientSocket(QObject *parent)
{
    m_bIsHeader = true;
	m_nRequiredSize = sizeof(MSG_HEADER);
	m_pData = NULL;
}

void ClientSocket::slotReceived()
{
	int nRequired = m_nRequiredSize;
	int nRemain = bytesAvailable();

	while ( nRemain >= nRequired )
	{
		nRemain -= nRequired;

		char* pBuf = new char[nRequired];
		read(pBuf,nRequired);
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
				DoHandle(m_pData,sizeof(MSG_HEADER) + pHeader->nBodyLen);

				nRequired = sizeof(MSG_HEADER);
				m_bIsHeader = true;

				delete [] m_pData;
				m_pData = NULL;
			}
			else
			{
				nRequired = pHeader->nBodyLen;
				m_bIsHeader = false;
			}
		}
		else
		{
			MSG_HEADER* pHeader = (MSG_HEADER*)m_pData;
			memcpy( (char*)m_pData + sizeof(MSG_HEADER), pBuf, pHeader->nBodyLen );

			//do handle msg
			DoHandle(m_pData,sizeof(MSG_HEADER) + pHeader->nBodyLen);

			m_bIsHeader = true;
			nRequired = sizeof(MSG_HEADER);

			delete[] m_pData;
			m_pData = NULL;
		}
		m_nRequiredSize = nRequired;

		delete[] pBuf;
		pBuf = NULL;
	}
}

void ClientSocket::slotDisconnected()
{
	CMemMgr::GetInstance()->MemRemoveClient(m_strGUID);

	QString strMsg = QString(QStringLiteral("客户端(%1)断开连接")).arg(m_strGUID);
	emit sigUpdateText(strMsg);
}

void ClientSocket::DoHandle(char* pData,int nBufLen)
{
	if ( pData == NULL )
	{
		return ;
	}
	MSG_HEADER* pHeader = (MSG_HEADER*)pData;
	switch ( pHeader->msgType )
	{
	case T_QUERY_LOGON:			//客户端登录
		{
			OnQueryLogon( pData,nBufLen );

			break;
		}
	case T_QUERY_MOVE:			//客户端走棋
		{
			OnQueryMove( pData,nBufLen );

			break;
		}
	case T_QUERY_SUCCESS:		//客户端胜利
		{
			OnQuerySuccess( pData,nBufLen );

			break;
		}

	default:
		{
			break;
		}
	}
}

void ClientSocket::OnQueryLogon(char* pData,int nBufLen)
{
	if ( pData == NULL )
	{
		return;
	}

	MSG_HEADER* pHeader = (MSG_HEADER*)pData;
	CLIENT_INFO* pInfo = (CLIENT_INFO*)(pHeader+1);
 	QString strGUID = QString::fromLatin1(pHeader->szGUID);

	int nPlayerInRoom = CMemMgr::GetInstance()->MemCountPlayersInRoom(pInfo->nRoom);
	if (nPlayerInRoom>=2)
	{
		pHeader->bResult = false;
	}
	else
	{
		m_strGUID = strGUID;
		m_nRoom = pInfo->nRoom;
		if (nPlayerInRoom == 0)
		{
			m_bBlack = true;
		}
		else
		{
			m_bBlack = false;
		}
		pInfo->bBlack = m_bBlack;
		pHeader->bResult = true;

		QString str = m_bBlack? QStringLiteral("黑棋"):QStringLiteral("白棋");
		QString strMsg = QString(QStringLiteral("客户端(%1)登录房间%2,执%3")).arg(m_strGUID).arg(m_nRoom).arg(str);
		emit sigUpdateText(strMsg);

		CMemMgr* pMgr = CMemMgr::GetInstance();
		if (pMgr->MemQueryClientExist(strGUID))
		{
			pMgr->MemUpdateClient(this);
		}
		else
		{
			pMgr->MemInsertClient(this);
		}
	}
	pHeader->msgType = T_ACK_LOGON;
	
	write((char*)pData,nBufLen);
}

void ClientSocket::OnQueryMove(char* pData,int nBufLen)
{
	MSG_HEADER* pHeader = (MSG_HEADER*)pData;
	QString strGUID = QString::fromLatin1(pHeader->szGUID);

	ClientSocket* pClient = CMemMgr::GetInstance()->MemQueryClientObject(strGUID);

	QString strMsg = QString(QStringLiteral("客户端(%1)走棋：落点(%2,%3)")).arg(strGUID).arg(pHeader->nReserved0).arg(pHeader->nReserved1);
	emit sigUpdateText(strMsg);

	MAP_CLIENTS_INFO mapClients;
	CMemMgr::GetInstance()->MemGetAllClient(mapClients);
	MAP_CLIENTS_INFO::const_iterator i;
	for( i = mapClients.constBegin(); i != mapClients.constEnd(); ++i)
	{
		ClientSocket* item = i.value();
		//给同一个房间的其他用户发送步骤消息
		if (item->m_nRoom == pClient->m_nRoom && item->m_strGUID != pClient->m_strGUID)
		{
			pHeader->msgType = T_ACK_MOVE;
			pHeader->bResult = true;

			item->write(pData,nBufLen);
		}
	}
}

void ClientSocket::OnQuerySuccess(char* pData,int nBufLen)
{
	MSG_HEADER* pHeader = (MSG_HEADER*)pData;
	QString strGUID = QString::fromLatin1(pHeader->szGUID);

	ClientSocket* pClient = CMemMgr::GetInstance()->MemQueryClientObject(strGUID);
	QString str = pClient->m_bBlack? QStringLiteral("黑棋"):QStringLiteral("白棋");

	QString strMsg = QString(QStringLiteral("客户端(%1)胜利! %2胜利.")).arg(strGUID).arg(str);
	emit sigUpdateText(strMsg);

}