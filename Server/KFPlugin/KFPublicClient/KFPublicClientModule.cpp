﻿#include "KFPublicClientModule.h"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
	KFPublicClientModule::KFPublicClientModule()
	{
		_kf_component = nullptr;
		_kf_basic = nullptr;
	}

	KFPublicClientModule::~KFPublicClientModule()
	{

	}

	void KFPublicClientModule::InitModule()
	{
	}

	void KFPublicClientModule::BeforeRun()
	{
		_kf_component = _kf_kernel->FindComponent( KFField::_player );
		_kf_component->RegisterUpdateDataModule( this, &KFPublicClientModule::OnUpdateDataCallBack );
		_kf_component->RegisterUpdateStringModule( this, &KFPublicClientModule::OnUpdateStringCallBack );
	
		_kf_player->RegisterEnterFunction( this, &KFPublicClientModule::OnEnterUpdatePublicData );
		_kf_player->RegisterLeaveFunction( this, &KFPublicClientModule::OnLeaveUpdatePublicData );

		///////////////////////////////////////////////////////////////////////////////////////////////
		__REGISTER_MESSAGE__( KFMsg::MSG_QUERY_BASIC_REQ, &KFPublicClientModule::HandleQueryBasicReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_QUERY_BASIC_ACK, &KFPublicClientModule::HandleQueryBasicAck );
	}

	void KFPublicClientModule::BeforeShut()
	{
		_kf_kernel->ReleaseObject( _kf_basic );
		_kf_component->UnRegisterUpdateDataModule( this );
		_kf_component->UnRegisterUpdateStringModule( this );

		_kf_player->UnRegisterEnterFunction( this );
		_kf_player->UnRegisterLeaveFunction( this );
		///////////////////////////////////////////////////////////////////////////////////////////////
		__UNREGISTER_MESSAGE__( KFMsg::MSG_QUERY_BASIC_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_QUERY_BASIC_ACK );
	}

	void KFPublicClientModule::OnceRun()
	{
		_kf_basic = _kf_kernel->CreateObject( KFField::_basic );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	bool KFPublicClientModule::SendMessageToPublic( uint32 msgid, ::google::protobuf::Message* message )
	{
		return _kf_cluster->SendMessageToShard( KFField::_public, msgid, message );
	}

	bool KFPublicClientModule::UpdatePublicData( KFEntity* player, const MapString& values )
	{
		KFMsg::S2SUpdatePublicDataReq req;
		req.set_playerid( player->GetKeyID() );

		for ( auto& iter : values )
		{
			auto pbstring = req.add_pbdata();
			pbstring->set_name( iter.first );
			pbstring->set_value( iter.second );
		}

		return SendMessageToPublic( KFMsg::S2S_UPDATE_PUBLIC_DATA_REQ, &req );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void KFPublicClientModule::OnUpdateDataToPublic( KFEntity* player, KFData* kfdata )
	{
		// 还没有初始化
		if ( !player->IsInited() )
		{
			return;
		}

		// 只有公共属性才更新
		if ( !kfdata->HaveFlagMask( KFDataDefine::Mask_Public_Data ) ||
			!kfdata->GetParent()->HaveFlagMask( KFDataDefine::Mask_Public_Data ) )
		{
			return;
		}
		
		// 更新到公共属性集群
		MapString values;
		values[ kfdata->GetName() ] = kfdata->ToString();
		UpdatePublicData( player, values );
	}

	void KFPublicClientModule::OnUpdateDataCallBack( KFEntity* player, uint64 key, KFData* kfdata, uint32 operate, uint64 value, uint64 oldvalue, uint64 newvalue )
	{
		OnUpdateDataToPublic( player, kfdata );
	}

	void KFPublicClientModule::OnUpdateStringCallBack( KFEntity* player, KFData* kfdata, const std::string& value )
	{
		OnUpdateDataToPublic( player, kfdata );
	}

	void KFPublicClientModule::OnEnterUpdatePublicData( KFEntity* player )
	{
		MapString values;
		values[ KFField::_server_id ] = __KF_STRING__( KFGlobal::Instance()->_app_id );
		values[ KFField::_id ] = __KF_STRING__( player->GetKeyID() );
		values[ KFField::_status ] = __KF_STRING__( KFMsg::StatusEnum::OnlineStatus );
		values[ KFField::_status_time ] = __KF_STRING__( KFGlobal::Instance()->_real_time );
		UpdatePublicData( player, values );
	}

	void KFPublicClientModule::OnLeaveUpdatePublicData( KFEntity* player )
	{
		MapString values;
		values[ KFField::_server_id ] = "0";
		values[ KFField::_group_id ] = "0";
		values[ KFField::_status ] = __KF_STRING__( KFMsg::StatusEnum::OfflineStatus );
		values[ KFField::_status_time ] = __KF_STRING__( KFGlobal::Instance()->_real_time );
		UpdatePublicData( player, values );
	}

	__KF_MESSAGE_FUNCTION__( KFPublicClientModule::HandleQueryBasicReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgQueryBasicReq );

		KFMsg::S2SQueryBasicReq req;
		req.set_playerid( playerid );
		req.set_name( kfmsg.name() );
		SendMessageToPublic( KFMsg::S2S_QUERY_BASIC_REQ, &req );
	}

	__KF_MESSAGE_FUNCTION__( KFPublicClientModule::HandleQueryBasicAck )
	{
		__SERVER_PROTO_PARSE__( KFMsg::S2SQueryBasicAck );
	
		KFMsg::MsgTellQueryBasic ack;
		auto pbbasic = ack.mutable_player();

		auto pboject = &kfmsg.pbobject();
		if ( pboject->key() == _invalid_int )
		{
			pbbasic->set_key( _invalid_int );
			pbbasic->set_name( _kf_basic->GetName() );
		}
		else
		{
			// 有数据的话 转换一下
			_kf_basic->Reset();
			_kf_basic->SetKeyID( pboject->key() );

			for ( auto i = 0; i < pboject->pbstring_size(); ++i )
			{
				auto pbstring = &pboject->pbstring( i );

				auto kfdata = _kf_basic->FindData( pbstring->name() );
				if ( kfdata != nullptr )
				{
					kfdata->SetValue< std::string >( pbstring->value() );
				}
			}

			_kf_kernel->SerializeToView( _kf_basic, pbbasic );
		}

		_kf_player->SendMessageToClient( player, KFMsg::MSG_TELL_QUERY_BASIC, &ack );
	}
}