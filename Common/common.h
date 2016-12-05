#pragma once


//客户端发送心跳包时间间隔
#define HEART_BEATS_INTERVAL (5 * 1000)

#pragma pack(1)

enum MSG_TYPE
{
	T_QUERY_LOGON = 2016,		//客户端登录
	T_ACK_LOGON,				//登录回复

	T_ACK_START,

    T_QUERY_MOVE,				//客户端发送走一步数据
    T_ACK_MOVE,					//服务端下发

	T_QUERY_SUCCESS,			//胜利消息
};//包类型

//消息头
typedef struct _MSG_HEADER 
{
	MSG_TYPE msgType;		//消息类型
	int nBodyLen;			//消息数据(不包含头)长度
	char szGUID[40];		//客户端guid			
	bool bResult;			//包处理结果:TRUE-操作成功 非0:操作失败 请求时默认为TRUE
	int nErrCode;			//错误代码: 0-正常 非0:异常 请求时默认为0
	int nReserved0;			//保留字段
	int nReserved1;

	_MSG_HEADER()
	{
		msgType = T_QUERY_LOGON;
		strcpy_s(szGUID,40,"");
		nBodyLen = nErrCode = nReserved0 = nReserved1 = 0;
		bResult = false;
	}
}MSG_HEADER,*PMSG_HEADER ;

//登录时收发的客户端信息
typedef struct _CLIENT_INFO
{
	int nRoom;
	bool bBlack;
	_CLIENT_INFO()
	{
		nRoom = 0;
		bBlack = true;
	}
}CLIENT_INFO;


#pragma pack()