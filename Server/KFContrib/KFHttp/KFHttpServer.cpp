﻿#include "KFHttpServer.h"
#include "KFHttpFactory.h"
#include "KFHttp/KFHttpCommon.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SecureServerSocket.h"

namespace KFrame
{
	KFHttpServer::KFHttpServer()
	{
		_http_server = nullptr;
	}

	KFHttpServer::~KFHttpServer()
	{
		__KF_DELETE__( _http_server );
	}

	void KFHttpServer::ShutDown()
	{
		_http_server->stopAll();

		KFLocker< KFMutex > kflocker( _kf_mutex );
		for ( auto kfdata : _kf_function_data )
		{
			__KF_DESTROY__( KFFunctionData, kfdata );
		}
		_kf_function_data.clear();
	}
	
	void KFHttpServer::Start( const std::string& ip, uint32 port, uint32 maxthread, uint32 maxqueue, uint32 idletime, bool keeplive )
	{	
		Poco::Net::HTTPServerParams* params = __KF_NEW__( Poco::Net::HTTPServerParams );
		params->setKeepAlive( keeplive );
		params->setMaxThreads( maxthread );
		params->setMaxQueued( maxqueue );
		params->setThreadIdleTime( idletime );

		Poco::Net::SocketAddress address( ip, port );
		Poco::Net::ServerSocket socket( address );

		auto httpfactory = __KF_NEW__( KFHttpFactory, this );
		_http_server = __KF_NEW__( Poco::Net::HTTPServer, httpfactory, socket, params );
		_http_server->start();
	}
	
	void KFHttpServer::RegisterMethonFunction( const std::string& url, bool sync, KFHttpMethodFunction& function )
	{
		auto kfhttpfunction = _functions.Create( url );
		kfhttpfunction->_sync = sync;
		kfhttpfunction->_function = function;
	}

	void KFHttpServer::UnRegisterMethonFunction( const std::string& url )
	{
		_functions.Remove( url );
	}
		
	std::string KFHttpServer::ProcessHttpRequest( const std::string& url, const std::string& ip, const std::string& data )
	{
		auto kffunction = _functions.Find( url );
		if ( kffunction == nullptr )
		{
			return _invalid_str;
		}

		if ( !kffunction->_sync )
		{
			return kffunction->_function( ip, data );
		}

		auto kfdata = __KF_CREATE__( KFFunctionData );
		kfdata->_ip = ip;
		kfdata->_data = data;
		kfdata->_kf_function = kffunction;
		{
			// 保存数据
			KFLocker< KFMutex > kflocker( _kf_mutex );
			_kf_function_data.push_back( kfdata );
		}

		return KFHttpCommon::SendResponseCode( KFCommonEnum::OK );
	}

	void KFHttpServer::Run()
	{
		std::list< KFFunctionData* > templist;
		{
			KFLocker< KFMutex > kflocker( _kf_mutex );
			templist.swap( _kf_function_data );
		}

		for ( auto kfdata : templist )
		{
			kfdata->_kf_function->_function( kfdata->_ip, kfdata->_data );
			__KF_DESTROY__( KFFunctionData, kfdata );
		}
	}
}