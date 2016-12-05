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

	//插入
	bool MemInsertClient(  ClientSocket* client );
	//删除
	bool MemRemoveClient( const QString strGUID );
	bool MemRemoveClient( const ClientSocket* client );
	//更新
	bool MemUpdateClient( ClientSocket* client );
	//查询
	bool MemQueryClientExist( const QString& strGUID );
	ClientSocket* MemQueryClientObject(const QString& strGUID );
	//获取所有终端信息
	bool MemGetAllClient( MAP_CLIENTS_INFO& mapClients );

	int MemCountPlayersInRoom(int nRoom);
private:
	MAP_CLIENTS_INFO m_mapClients;

	QBasicMutex m_mutex;
};

