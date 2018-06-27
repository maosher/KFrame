#ifndef __KF_HTTP_DATA_H__
#define __KF_HTTP_DATA_H__

#include "KFDefine.h"

namespace KFrame
{
	class KFHttp;
	class KFHttpClientManage;
	__ST_CLASS__( KFHttpData )
	{
	public:
		KFHttpData();
		~KFHttpData();
		
		// ����Http
		void StartMTHttp( KFHttpClientManage* clientmanage );
		
		// ִ���߼�
		void Run();
		////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////
		// ���ʽ���
		void Finish();
	public:
		// ������
		KFHttpClientManage* _manage;

		// ���ʵ�ַ
		std::string _url;
		
		// ��������
		std::string _data;

		// ��������
		std::string _result;

		// �ص�����
		KFHttpClientFunction _function;

		// httpclient
		KFHttp* _http;
	
	};
}


#endif