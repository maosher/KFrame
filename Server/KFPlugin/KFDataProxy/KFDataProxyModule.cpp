﻿#include "KFDataProxyModule.h"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
	KFDataProxyModule::KFDataProxyModule()
	{
	}

	KFDataProxyModule::~KFDataProxyModule()
	{

	}

	void KFDataProxyModule::InitModule()
	{

	}

	void KFDataProxyModule::BeforeRun()
	{
		_kf_tcp_client->RegisterLostFunction( this, &KFDataProxyModule::OnClientLostServer );
		////////////////////////////////////////////////////////////////////////////////////////
		__REGISTER_MESSAGE__( KFMsg::S2S_UPDATE_ZONE_TO_PROXY_REQ, &KFDataProxyModule::HandleUpdateZoneToProxyReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_TRANSMIT_TO_DATA_SHARD_REQ, &KFDataProxyModule::HandleTransmitToDataShardReq );
	}

	void KFDataProxyModule::BeforeShut()
	{
		_kf_tcp_client->UnRegisterLostFunction( this );
		////////////////////////////////////////////////////////////////////////////////////////
		__UNREGISTER_MESSAGE__( KFMsg::S2S_UPDATE_ZONE_TO_PROXY_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_TRANSMIT_TO_DATA_SHARD_REQ );

	}

	/////////////////////////////////////////////////////////////////////////////
	__KF_CLIENT_LOST_FUNCTION__( KFDataProxyModule::OnClientLostServer )
	{
		for ( auto iter : _zone_hash._objects )
		{
			auto kfhash = iter.second;
			auto ok = kfhash->RemoveHashNode( serverid );
			if ( ok )
			{
				KFLogger::LogSystem( KFLogger::Error, "[%s] remove zone[%u] server[%u]!",
					__FUNCTION__, iter.first, serverid );
			}
		}
	}
	
	uint32 KFDataProxyModule::FindZoneShardId( uint32 zoneid, uint32 clientid )
	{
		auto kfhash = _zone_hash.Find( zoneid );
		if ( kfhash == nullptr )
		{
			return  _invalid_int;
		}

		return kfhash->FindHashNode( clientid, true );
	}

	__KF_MESSAGE_FUNCTION__( KFDataProxyModule::HandleUpdateZoneToProxyReq )
	{
		__PROTO_PARSE__( KFMsg::S2SUpdateZoneToProxyReq );

		auto shardid = __KF_HEAD_ID__( kfguid );
		for ( auto i = 0; i < kfmsg.zoneid_size(); ++i )
		{
			auto zoneid = kfmsg.zoneid( i );
			
			auto* kfhash = _zone_hash.Create( zoneid );
			kfhash->AddHashNode( "zone", shardid, 100 );
			
			KFLogger::LogSystem( KFLogger::Info, "[%s] add zone[%u] server[%u]!",
				__FUNCTION__, zoneid, shardid );
		}
	}

	__KF_MESSAGE_FUNCTION__( KFDataProxyModule::HandleTransmitToDataShardReq )
	{
		__PROTO_PARSE__( KFMsg::S2STransmitToDataShardReq );
		auto clientid = __KF_HEAD_ID__( kfguid );

		auto shardid = FindZoneShardId( kfmsg.zoneid(), clientid );
		if ( shardid == _invalid_int )
		{
			return KFLogger::LogLogic( KFLogger::Error, "[%s] can't find zone[%u] shardid!",
				__FUNCTION__, kfmsg.zoneid() );
		}
		
		auto ok = _kf_tcp_client->SendNetMessage( shardid, clientid, kfmsg.msgid(), kfmsg.msgdata().data(), kfmsg.msgdata().length() );
		if ( !ok )
		{
			KFLogger::LogLogic( KFLogger::Error, "[%s] zone[%u] shardid[%s] send msgid[%u] failed!",
				__FUNCTION__, kfmsg.zoneid(), shardid, kfmsg.msgid() );
		}
	}
}