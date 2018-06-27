﻿#include "KFActivityModule.h"
#include "KFActivityConfig.h"
#include "KFProtocol/KFProtocol.h"


namespace KFrame
{
	KFActivityModule::KFActivityModule()
	{
		_kf_componnet = nullptr;
	}

	KFActivityModule::~KFActivityModule()
	{
	}

	void KFActivityModule::InitModule()
	{
		_kf_config->AddConfig( _kf_activity_config, _kf_plugin->_plugin_name, _kf_plugin->_config, true );
	}

	void KFActivityModule::BeforeRun()
	{
		_kf_componnet = _kf_kernel->FindComponent( KFField::_player );

		_kf_componnet->RegisterUpdateDataModule( this, &KFActivityModule::OnUpdateDataCallBack );
		_kf_componnet->RegisterAddDataModule( this, &KFActivityModule::OnAddDataCallBack );
		_kf_componnet->RegisterRemoveDataModule( this, &KFActivityModule::OnRemoveDataCallBack );

		_kf_componnet->RegisterUpdateDataFunction( KFField::_activity, KFField::_value, this, &KFActivityModule::OnActivityValueUpdateCallBack );

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//_kf_player->RegisterCheckDailyDataFunction( KFField::_activity, this, &KFActivityModule::CheckResetDailyActivity );
		//////////////////////////////////////////////////////////////////////////////////////////////////
		__REGISTER_MESSAGE__( KFMsg::MSG_RECEIVE_ACTIVITY_REWARD_REQ, &KFActivityModule::HandleReceiveActivityRewardReq );
	}

	void KFActivityModule::BeforeShut()
	{
		_kf_config->RemoveConfig( _kf_plugin->_plugin_name );

		_kf_componnet->UnRegisterAddDataModule( this );
		_kf_componnet->UnRegisterRemoveDataModule( this );
		_kf_componnet->UnRegisterUpdateDataModule( this );
		_kf_componnet->UnRegisterUpdateDataFunction( KFField::_activity, KFField::_value );
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//_kf_player->UnRegisterCheckDailyDataFunction( KFField::_activity );
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		__UNREGISTER_MESSAGE__( KFMsg::MSG_RECEIVE_ACTIVITY_REWARD_REQ );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////	
	__KF_MESSAGE_FUNCTION__( KFActivityModule::HandleReceiveActivityRewardReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgReceiveActivityRewardReq );

		auto result = ReceiveActivityReward( player, kfmsg.type(), kfmsg.id() );
		_kf_display->SendToClient( player, result );
	}

	uint32 KFActivityModule::ReceiveActivityReward( KFEntity* player, uint32 type, uint32 activityid )
	{
		auto kfsetting = _kf_activity_config->FindActivityData( type, activityid );
		if ( kfsetting == nullptr )
		{
			return KFMsg::ActivityIdCanNotFind;
		}

		auto kfobject = player->GetData();
		auto kfactivity = kfobject->FindData( KFField::_activity, type );
		if ( kfactivity == nullptr )
		{
			return KFMsg::CanNotFindActivityType;
		}

		auto received = kfactivity->GetValue( KFField::_received );
		if ( KFUtility::HaveBitMask<uint64>( received, activityid ) )
		{
			return KFMsg::ActivityAlreadyReceived;
		}

		auto value = kfactivity->GetValue( KFField::_value );
		if ( value < kfsetting->_done_value )
		{
			return KFMsg::ActivityNotDone;
		}

		player->UpdateData( kfactivity, KFField::_received, KFOperateEnum::ABit, activityid );

		player->AddAgentData( &kfsetting->_rewards, 1.0f, true, __FUNCTION_LINE__ );
		return KFMsg::ActivityReceiveRewardOK;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void KFActivityModule::OnActivityValueUpdateCallBack( KFEntity* player, uint64 key, KFData* kfdata, uint32 operate, uint64 value, uint64 oldvalue, uint64 newvalue )
	{
		auto activitysetting = _kf_activity_config->FindActivitySetting( key );
		if ( activitysetting == nullptr )
		{
			return;
		}

		auto nowvalue = kfdata->GetValue();
		if ( nowvalue <= activitysetting->_max_value )
		{
			return;
		}

		kfdata->OperateValue<uint64>( KFOperateEnum::Set, activitysetting->_max_value );
	}

	void KFActivityModule::UpdateDataActivityValue( KFEntity* player, uint64 key, KFData* kfdata, uint32 operate, uint64 value, uint64 newvalue )
	{
		if ( value == _invalid_int )
		{
			return;
		}

		auto kfparent = kfdata->GetParent();
		auto kfactivitytypesetting = _kf_activity_config->FindActivityType( kfparent->GetName(), kfdata->GetName() );
		if ( kfactivitytypesetting == nullptr )
		{
			return;
		}

		auto kfobject = player->GetData();
		auto kfactivityrecord = kfobject->FindData( KFField::_activity );
		if ( kfactivityrecord == nullptr )
		{
			return;
		}

		auto level = kfobject->GetValue( KFField::_level );
		auto birthday = kfobject->GetValue( KFField::_birthday );
		auto realtime = KFGlobal::Instance()->_real_time;

		for ( auto activitysetting : kfactivitytypesetting->_activity_type )
		{
			// 判断活动时间
			if ( !activitysetting->CheckInTime( realtime, birthday ) )
			{
				return;
			}

			// 是否能更新
			if ( !activitysetting->CheckCanUpdate( key, level, operate ) )
			{
				continue;
			}

			// 检查出发值
			auto operatevalue = activitysetting->CheckTriggerValue( value, newvalue );
			if ( operatevalue == _invalid_int )
			{
				continue;
			}

			// 检查是否需要重置
			auto kfactivity = kfactivityrecord->FindData( activitysetting->_type );
			if ( kfactivity != nullptr )
			{
				auto activitytime = kfactivity->GetValue( KFField::_time );
				if ( activitysetting->CheckResetActivity( realtime, birthday, activitytime ) )
				{
					player->UpdateData( kfactivityrecord, activitysetting->_type, KFField::_value, KFOperateEnum::Set, _invalid_int );
					player->UpdateData( kfactivityrecord, activitysetting->_type, KFField::_received, KFOperateEnum::Set, _invalid_int );
					player->UpdateData( kfactivityrecord, activitysetting->_type, KFField::_time, KFOperateEnum::Set, realtime );
				}

				auto donevalue = kfactivity->GetValue( KFField::_value );
				if ( donevalue >= activitysetting->_max_value )
				{
					continue;
				}
			}

			auto usevalue = activitysetting->GetUseValue( operatevalue );
			player->UpdateData( kfactivityrecord, activitysetting->_type, KFField::_value, activitysetting->_operate, usevalue );
		}
	}

	void KFActivityModule::OnUpdateDataCallBack( KFEntity* player, uint64 key, KFData* kfdata, uint32 operate, uint64 value, uint64 oldvalue, uint64 newvalue )
	{
		UpdateDataActivityValue( player, key, kfdata, operate, value, newvalue );
	}

	void KFActivityModule::UpdateObjectActivityValue( KFEntity* player, uint64 key, KFData* kfdata, uint32 operate )
	{
		auto child = kfdata->FirstData();
		while ( child != nullptr )
		{
			auto value = child->GetValue();
			if ( value != _invalid_int )
			{
				UpdateDataActivityValue( player, key, child, operate, value, value );
			}

			child = kfdata->NextData();
		}
	}

	void KFActivityModule::OnAddDataCallBack( KFEntity* player, KFData* kfparent, uint64 key, KFData* kfdata )
	{
		UpdateObjectActivityValue( player, key, kfdata, KFOperateEnum::Add );
	}

	void KFActivityModule::OnRemoveDataCallBack( KFEntity* player, KFData* kfparent, uint64 key, KFData* kfdata )
	{
		UpdateObjectActivityValue( player, key, kfdata, KFOperateEnum::Dec );
	}

	void KFActivityModule::CheckResetDailyActivity( KFEntity* player )
	{
		auto kfobject = player->GetData();
		auto kfactivityrecord = kfobject->FindData( KFField::_activity );
		if ( kfactivityrecord == nullptr )
		{
			return;
		}

		std::list< uint64 > removes;

		auto birthday = kfobject->GetValue( KFField::_birthday );
		auto realtime = KFGlobal::Instance()->_real_time;

		auto kfactivity = kfactivityrecord->FirstData();
		while ( kfactivity != nullptr )
		{
			auto type = kfactivity->GetKeyID();
			auto activitytime = kfactivity->GetValue( KFField::_time );

			auto activitysetting = _kf_activity_config->FindActivitySetting( type );
			if ( activitysetting == nullptr ||
				!activitysetting->CheckInTime( realtime, birthday ) ||
				activitysetting->CheckResetActivity( realtime, birthday, activitytime ) )
			{
				removes.push_back( type );
			}

			kfactivity = kfactivityrecord->NextData();
		}

		for ( auto type : removes )
		{
			player->RemoveData( kfactivityrecord, type );
		}
	}
}