#pragma once


//�ͻ��˷���������ʱ����
#define HEART_BEATS_INTERVAL (5 * 1000)

#pragma pack(1)

enum MSG_TYPE
{
	T_QUERY_LOGON = 2016,		//�ͻ��˵�¼
	T_ACK_LOGON,				//��¼�ظ�

	T_ACK_START,

    T_QUERY_MOVE,				//�ͻ��˷�����һ������
    T_ACK_MOVE,					//������·�

	T_QUERY_SUCCESS,			//ʤ����Ϣ
};//������

//��Ϣͷ
typedef struct _MSG_HEADER 
{
	MSG_TYPE msgType;		//��Ϣ����
	int nBodyLen;			//��Ϣ����(������ͷ)����
	char szGUID[40];		//�ͻ���guid			
	bool bResult;			//��������:TRUE-�����ɹ� ��0:����ʧ�� ����ʱĬ��ΪTRUE
	int nErrCode;			//�������: 0-���� ��0:�쳣 ����ʱĬ��Ϊ0
	int nReserved0;			//�����ֶ�
	int nReserved1;

	_MSG_HEADER()
	{
		msgType = T_QUERY_LOGON;
		strcpy_s(szGUID,40,"");
		nBodyLen = nErrCode = nReserved0 = nReserved1 = 0;
		bResult = false;
	}
}MSG_HEADER,*PMSG_HEADER ;

//��¼ʱ�շ��Ŀͻ�����Ϣ
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