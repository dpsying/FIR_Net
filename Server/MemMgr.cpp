#include "MemMgr.h"


CMemMgr::CMemMgr(void)
{
}


CMemMgr::~CMemMgr(void)
{
}

CMemMgr* CMemMgr::GetInstance()
{
	static CMemMgr obj;
	return &obj;
}

bool CMemMgr::MemInsertClient( ClientSocket* client)
{
	QMutexLocker locker(&m_mutex); 

	if (client)
	{
		m_mapClients.insert(client->m_strGUID,client);
	}
	return true;
}

bool CMemMgr::MemRemoveClient(const QString strGUID)
{
	QMutexLocker locker(&m_mutex); 
	m_mapClients.remove(strGUID);
	return true;
}

bool CMemMgr::MemRemoveClient(const ClientSocket* client)
{
	QMutexLocker locker(&m_mutex); 
	m_mapClients.remove(client->m_strGUID);
	return true;
}

bool CMemMgr::MemUpdateClient( ClientSocket* client)
{
	QMutexLocker locker(&m_mutex); 
	m_mapClients.remove(client->m_strGUID);
	m_mapClients.insert(client->m_strGUID,client);
	return true;
}

bool CMemMgr::MemQueryClientExist(const QString& strGUID) 
{
	QMutexLocker locker(&m_mutex); 
	if (m_mapClients.find(strGUID) == m_mapClients.end())
	{
		return false;
	}
	return true;
}

ClientSocket* CMemMgr::MemQueryClientObject(const QString& strGUID) 
{
	QMutexLocker locker(&m_mutex); 
	MAP_CLIENTS_INFO::const_iterator it = m_mapClients.find(strGUID);
	if ( it == m_mapClients.end())
	{
		return NULL;
	}
	return it.value();
}

bool CMemMgr::MemGetAllClient(MAP_CLIENTS_INFO& mapClients)
{
	QMutexLocker locker(&m_mutex); 
	mapClients = m_mapClients;
	return true;
}

int CMemMgr::MemCountPlayersInRoom(int nRoom)
{
	int nPlayerInRoom = 0;
	MAP_CLIENTS_INFO::const_iterator i;
	for( i=m_mapClients.constBegin(); i!=m_mapClients.constEnd(); ++i)
	{
		ClientSocket* pClient = i.value();
		if (pClient->m_nRoom == nRoom)
		{
			nPlayerInRoom++;
		}
	}
	return nPlayerInRoom;
}
