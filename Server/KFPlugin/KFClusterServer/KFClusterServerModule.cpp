﻿#include "KFClusterServerModule.h"
#include "KFClusterServerManage.h"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
	KFClusterServerModule::KFClusterServerModule()
	{

	}

	KFClusterServerModule::~KFClusterServerModule()
	{
	}

	void KFClusterServerModule::InitModule()
	{
		///////////////////////////////////////////////////////////////////////////////
	}

	void KFClusterServerModule::BeforeRun()
	{
		_kf_tcp_server->RegisterLostFunction( this, &KFClusterServerModule::OnServerLostClusterProxy );

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__REGISTER_MESSAGE__( KFMsg::S2S_CLUSTER_REGISTER_REQ, &KFClusterServerModule::HandleClusterRegisterReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_CLUSTER_UPDATE_REQ, &KFClusterServerModule::HandleClusterUpdateReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_CLUSTER_AUTH_REQ, &KFClusterServerModule::HandleClusterAuthReq );
	}

	void KFClusterServerModule::BeforeShut()
	{
		_kf_tcp_server->UnRegisterLostFunction( this );

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__UNREGISTER_MESSAGE__( KFMsg::S2S_CLUSTER_REGISTER_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_CLUSTER_UPDATE_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_CLUSTER_AUTH_REQ );

	}

	void KFClusterServerModule::OnceRun()
	{
		auto* mastersetting = _kf_connection->FindMasterConnection( KFGlobal::Instance()->_app_name );
		if ( mastersetting == nullptr )
		{
			return;
		}

		_cluster_key = mastersetting->_key;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////	
	__KF_SERVER_LOST_FUNCTION__( KFClusterServerModule::OnServerLostClusterProxy )
	{
		KFProxyData* kfgate = _kf_proxy_manage->FindProxyServer( handleid );
		if ( kfgate == nullptr )
		{
			return;
		}

		KFLogger::LogSystem( KFLogger::Info, "[%s:%s:%u|%s:%u] Lost!",
			kfgate->_name.c_str(), kfgate->_type.c_str(), kfgate->_id, kfgate->_ip.c_str(), kfgate->_port );

		_kf_proxy_manage->RemoveProxyServer( handleid );
	}

	__KF_MESSAGE_FUNCTION__( KFClusterServerModule::HandleClusterRegisterReq )
	{
		__PROTO_PARSE__( KFMsg::S2SClusterRegisterReq );

		_kf_proxy_manage->AddProxyServer( kfmsg.type(), kfmsg.id(), kfmsg.name(), kfmsg.ip(), kfmsg.port() );

		KFLogger::LogSystem( KFLogger::Info, "[%s:%s:%u|%s:%u] Discover!",
			kfmsg.name().c_str(), kfmsg.type().c_str(), kfmsg.id(), kfmsg.ip().c_str(), kfmsg.port() );
	}

	__KF_MESSAGE_FUNCTION__( KFClusterServerModule::HandleClusterUpdateReq )
	{
		__PROTO_PARSE__( KFMsg::S2SClusterUpdateReq );

		_kf_proxy_manage->UpdateProxyServer( kfmsg.gateid(), kfmsg.count() );
	}

	__KF_MESSAGE_FUNCTION__( KFClusterServerModule::HandleClusterAuthReq )
	{
		__PROTO_PARSE__( KFMsg::S2SClusterAuthReq );
		uint32 handleid = __KF_HEAD_ID__( kfguid );

		KFLogger::LogLogic( KFLogger::Info, "[%s] cluster[%u] key req!",
			kfmsg.clusterkey().c_str(), handleid );

		if ( kfmsg.clusterkey() != _cluster_key )
		{
			KFLogger::LogLogic( KFLogger::Error, "[%s!=%s] cluster[%u] key error!",
				kfmsg.clusterkey().c_str(), _cluster_key.c_str(), handleid );
			return;
		}

		KFMsg::S2SClusterAuthAck ack;
		ack.set_clustertype( kfmsg.clustertype() );
		auto kfproxy = _kf_proxy_manage->HashProxyServer( handleid );
		if ( kfproxy == nullptr )
		{
			ack.set_token( "" );
			ack.set_ip( "" );
			ack.set_port( 0 );
			ack.set_name( "" );
			ack.set_type( "" );
			ack.set_id( 0 );

			KFLogger::LogLogic( KFLogger::Error, "[%s] cluster[%u] key failed!",
				kfmsg.clusterkey().c_str(), handleid );
		}
		else
		{
			// 创建token
			std::string token = MakeAuthToken( kfguid );
			ack.set_token( token );
			ack.set_ip( kfproxy->_ip );
			ack.set_port( kfproxy->_port );
			ack.set_name( kfproxy->_name );
			ack.set_type( kfproxy->_type );
			ack.set_id( kfproxy->_id );

			// Token发送给GateServer 
			KFMsg::S2SClusterTokenReq tokenreq;
			tokenreq.set_token( token );
			tokenreq.set_gateid( handleid );
			_kf_tcp_server->SendNetMessage( kfproxy->_id, KFMsg::S2S_CLUSTER_TOKEN_REQ, &tokenreq );

			KFLogger::LogLogic( KFLogger::Info, "[%s] cluster[%u] key ok!",
				kfmsg.clusterkey().c_str(), handleid );
		}

		_kf_tcp_server->SendNetMessage( handleid, KFMsg::S2S_CLUSTER_AUTH_ACK, &ack );
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	std::string KFClusterServerModule::MakeAuthToken( const KFGuid& guid )
	{
		std::string headid = KFUtility::ToString( guid._head_id );
		std::string dataid = KFUtility::ToString( guid._data_id );
		std::string date = KFUtility::ToString( KFDate::GetTimeEx() );

		std::string temp = KFUtility::Format( "%s:%s-%s:%u", headid.c_str(), dataid.c_str(), date.c_str(), KFRand::STRand32() );
		return KFCrypto::Md5Encode( temp );
	}
}