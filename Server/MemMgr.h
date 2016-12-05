#pragma once
#include "qmap.h"
#include "ClientSocket.h"
#include "qmutex.h"

typedef QMap<QString,ClientSocket*> MAP_CLIENTS_INFO;

class CMemMgr
{
public:
	CMemMgr(void);
	~CMemMgr(void);

	static CMemMgr* GetInstance();

	//����
	bool MemInsertClient(  ClientSocket* client );
	//ɾ��
	bool MemRemoveClient( const QString strGUID );
	bool MemRemoveClient( const ClientSocket* client );
	//����
	bool MemUpdateClient( ClientSocket* client );
	//��ѯ
	bool MemQueryClientExist( const QString& strGUID );
	ClientSocket* MemQueryClientObject(const QString& strGUID );
	//��ȡ�����ն���Ϣ
	bool MemGetAllClient( MAP_CLIENTS_INFO& mapClients );

	int MemCountPlayersInRoom(int nRoom);
private:
	MAP_CLIENTS_INFO m_mapClients;

	QBasicMutex m_mutex;
};

