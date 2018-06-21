﻿#include "KFGroupClientModule.h"
#include "KFGroupClientConfig.h"

namespace KFrame
{
	KFGroupClientModule::KFGroupClientModule()
	{
		_kf_group_member = nullptr;
	}

	KFGroupClientModule::~KFGroupClientModule()
	{

	}

	void KFGroupClientModule::InitModule()
	{
		_kf_config->AddConfig( _kf_group_config, _kf_plugin->_plugin_name, _kf_plugin->_config, true );
		///////////////////////////////////////////////////////////////////////////////
	}

	void KFGroupClientModule::BeforeRun()
	{
		_kf_component = _kf_kernel->FindComponent( KFField::_player );
		_kf_component->RegisterUpdateDataModule( this, &KFGroupClientModule::OnUpdateDataCallBack );
		_kf_component->RegisterUpdateStringModule( this, &KFGroupClientModule::OnUpdateStringCallBack );

		_kf_player->RegisterEnterFunction( this, &KFGroupClientModule::OnEnterQueryMatchGroup );
		_kf_player->RegisterLeaveFunction( this, &KFGroupClientModule::OnLeaveUpdateMatchGroup );
		///////////////////////////////////////////////////////////////////////////////
		__REGISTER_MESSAGE__( KFMsg::MSG_INVITE_MATCH_GROUP_REQ, &KFGroupClientModule::HandleInviteMatchGroupReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_RECEIVE_INVITE_MATCH_GROUP_REQ, &KFGroupClientModule::HandleReceiveInviteMatchGroupReq );
		__REGISTER_MESSAGE__( KFMsg::MSG_REPLY_INVITE_MATCH_GROUP_REQ, &KFGroupClientModule::HandleReplyInviteMatchGroupReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_ADD_MATCH_GROUP_MEMBER_ACK, &KFGroupClientModule::HandleAddMatchGroupMemberAck );
		__REGISTER_MESSAGE__( KFMsg::S2S_REMOVE_MATCH_GROUP_MEMBER_ACK, &KFGroupClientModule::HandleRemoveMatchGroupMemberAck );
		__REGISTER_MESSAGE__( KFMsg::MSG_LEAVE_MATCH_GROUP_REQ, &KFGroupClientModule::HandleLeaveMatchGroupReq );
		__REGISTER_MESSAGE__( KFMsg::MSG_KICK_MATCH_GROUP_REQ, &KFGroupClientModule::HandleKickMatchGroupReq );
		__REGISTER_MESSAGE__( KFMsg::MSG_APPLY_MATCH_GROUP_REQ, &KFGroupClientModule::HandleApplyMatchGroupReq );
		__REGISTER_MESSAGE__( KFMsg::MSG_REPLY_APPLY_MATCH_GROUP_REQ, &KFGroupClientModule::HandleReplyApplyMatchGroupReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_LEAVE_MATCH_GROUP_ACK, &KFGroupClientModule::HandleLeaveMatchGroupAck );
		__REGISTER_MESSAGE__( KFMsg::S2S_CONSENT_APPLY_MATCH_GROUP_ACK, &KFGroupClientModule::HandleConsentApplyMatchGroupAck );
		__REGISTER_MESSAGE__( KFMsg::S2S_UPDATE_GROUP_MEMBER_ACK, &KFGroupClientModule::HandleUpdateGroupMemberAck );
		__REGISTER_MESSAGE__( KFMsg::MSG_MATCH_GROUP_PREPARE_REQ, &KFGroupClientModule::HandleMatchGroupPrepareReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_UPDATE_GROUP_CAPTAIN_ACK, &KFGroupClientModule::HandleUpdateGroupCaptainAck );
		__REGISTER_MESSAGE__( KFMsg::S2S_APPLY_MATCH_GROUP_ACK, &KFGroupClientModule::HandleApplyMatchGroupAck );
		__REGISTER_MESSAGE__( KFMsg::S2S_UPDATE_GROUP_MATCH_ACK, &KFGroupClientModule::HandleUpdateGroupMatchAck );
		__REGISTER_MESSAGE__( KFMsg::MSG_UPDATE_GROUP_MATCH_REQ, &KFGroupClientModule::HandleUpdateGroupMatchReq );
		__REGISTER_MESSAGE__( KFMsg::S2S_CONSENT_INVITE_MATCH_GROUP_REQ, &KFGroupClientModule::HandleConsentInviteMatchGroupReq );
		
	}

	void KFGroupClientModule::OnceRun()
	{
		_kf_group_member = _kf_kernel->CreateObject( "Group", KFField::_group_member );
	
	}

	void KFGroupClientModule::BeforeShut()
	{
		_kf_kernel->ReleaseObject( _kf_group_member );
		_kf_config->RemoveConfig( _kf_plugin->_plugin_name );
		
		_kf_component->UnRegisterUpdateDataModule( this );
		_kf_component->UnRegisterUpdateStringModule( this );

		_kf_player->UnRegisterEnterFunction( this );
		_kf_player->UnRegisterLeaveFunction( this );
		///////////////////////////////////////////////////////////////////////////////
		__UNREGISTER_MESSAGE__( KFMsg::MSG_INVITE_MATCH_GROUP_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_RECEIVE_INVITE_MATCH_GROUP_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::MSG_REPLY_INVITE_MATCH_GROUP_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_ADD_MATCH_GROUP_MEMBER_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_REMOVE_MATCH_GROUP_MEMBER_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::MSG_LEAVE_MATCH_GROUP_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::MSG_KICK_MATCH_GROUP_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::MSG_APPLY_MATCH_GROUP_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::MSG_REPLY_APPLY_MATCH_GROUP_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_LEAVE_MATCH_GROUP_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_CONSENT_APPLY_MATCH_GROUP_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_UPDATE_GROUP_MEMBER_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::MSG_MATCH_GROUP_PREPARE_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_UPDATE_GROUP_CAPTAIN_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_APPLY_MATCH_GROUP_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_UPDATE_GROUP_MATCH_ACK );
		__UNREGISTER_MESSAGE__( KFMsg::MSG_UPDATE_GROUP_MATCH_REQ );
		__UNREGISTER_MESSAGE__( KFMsg::S2S_CONSENT_INVITE_MATCH_GROUP_REQ );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool KFGroupClientModule::SendMessageToGroup( uint32 msgid, ::google::protobuf::Message* message )
	{
		return _kf_cluster->SendMessageToShard( KFField::_group, msgid, message );
	}

	bool KFGroupClientModule::SendMessageToGroup( uint64 groupid, uint32 msgid, ::google::protobuf::Message* message )
	{
		return _kf_cluster->SendMessageToObject( KFField::_group, groupid, msgid, message );
	}

	uint64 KFGroupClientModule::PrepareMatchGroup( KFEntity* player, uint32 matchid, uint32 maxcount )
	{
		auto kfobject = player->GetData();
		auto kfbasic = kfobject->FindData( KFField::_basic );

		auto groupid = kfbasic->GetValue< uint64 >( KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			groupid = kfobject->GetValue< uint64 >( KFField::_group_id );
			if ( groupid == _invalid_int )
			{
				// 没有创建小组
				groupid = KFUtility::Make64Guid( player->GetKeyID() );
				kfobject->SetValue< uint32 >( KFField::_group_match_id, matchid );
				kfobject->SetValue< uint64 >( KFField::_group_id, groupid );
			}
		}
		else
		{
			// 判断最大数量
			auto maxgroupcount = kfbasic->GetValue< uint32 >( KFField::_max_group_count );
			if ( maxcount > maxgroupcount )
			{
				UpdateMatchToGroup( player, groupid, matchid, maxcount );
			}
		}

		return groupid;
	}
	
	bool KFGroupClientModule::CreateMatchGroup( KFEntity* player, uint64 groupid )
	{
		auto kfobject = player->GetData();

		auto matchid = kfobject->GetValue< uint32 >( KFField::_group_match_id );
		auto maxcount = _kf_match->GetMatchMaxCount( matchid );
		
		// 先更新状态
		UpdateMatchGroupStatus( player, groupid, 1, matchid, maxcount );

		// 发送到队伍集群
		KFMsg::S2SCreateMatchGroupReq req;
		req.set_groupid( groupid );
		req.set_matchid( matchid );
		req.set_maxcount( maxcount );
		FormatMatchGroupMember( player, req.mutable_pbmember() );
		auto ok =  SendMessageToGroup( KFMsg::S2S_CREATE_MATCH_GROUP_REQ, &req );
		if ( !ok )
		{
			UpdateMatchGroupStatus( player, _invalid_int, _invalid_int, _invalid_int, _invalid_int );
		}

		return ok;
	}

	void KFGroupClientModule::UpdateMatchGroupStatus( KFEntity* player, uint64 groupid, uint32 iscaptain, uint32 matchid, uint32 maxcount )
	{
		auto kfobject = player->GetData();
		auto kfbasic = kfobject->FindData( KFField::_basic );

		player->UpdateData( KFField::_group_match_id, KFOperateEnum::Set, matchid );

		// 更新队长
		auto selfcaptain = kfobject->GetValue< uint32 >( KFField::_is_captain );
		if ( selfcaptain != iscaptain )
		{
			player->UpdateData( KFField::_is_captain, KFOperateEnum::Set, iscaptain );
		}

		if ( groupid != _invalid_int )
		{
			player->UpdateData( kfbasic, KFField::_status, KFOperateEnum::Set, KFMsg::StatusEnum::GroupStatus );
			player->UpdateData( kfbasic, KFField::_max_group_count, KFOperateEnum::Set, maxcount );
			player->UpdateData( kfbasic, KFField::_group_id, KFOperateEnum::Set, groupid );
		}
		else
		{
			player->UpdateData( kfbasic, KFField::_group_id, KFOperateEnum::Set, groupid );
			player->UpdateData( kfbasic, KFField::_status, KFOperateEnum::Set, KFMsg::StatusEnum::OnlineStatus );
		}
	}

	void KFGroupClientModule::FormatMatchGroupMember( KFEntity* player, KFMsg::PBObject* pbobject )
	{
		// 基础信息
		auto kfobject = player->GetData();
		auto kfplayerbasic = kfobject->FindData( KFField::_basic );
		auto kfmemberbasic = _kf_group_member->FindData( KFField::_basic );
		kfmemberbasic->CopyFrom( kfplayerbasic );

		// 队员id
		_kf_group_member->SetKeyID( player->GetKeyID() );

		// 模型id
		auto modelid = kfobject->GetValue< uint32 >( KFField::_model_id );
		_kf_group_member->SetValue< uint32 >( KFField::_model_id, modelid );

		// 时装id
		auto clothesid = kfobject->GetValue< uint32 >( KFField::_clothes_id );
		_kf_group_member->SetValue< uint32 >( KFField::_clothes_id, clothesid );

		// 序列化
		_kf_kernel->SerializeToClient( _kf_group_member, pbobject );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleUpdateGroupMatchReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgUpdateGroupMatchReq );
		
		auto kfobject = player->GetData();
		auto kfbasic = kfobject->FindData( KFField::_basic );

		auto groupid = kfbasic->GetValue< uint64 >( KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupNotInGroup );
		}

		auto maxcount = _kf_match->GetMatchMaxCount( kfmsg.matchid() );
		auto nowgroupcount = kfbasic->GetValue< uint32 >( KFField::_now_group_count );
		if ( maxcount < nowgroupcount )
		{
			return _kf_display->SendToClient( player, KFMsg::MatchGroupPlayerLimit );
		}

		UpdateMatchToGroup( player, groupid, kfmsg.matchid(), maxcount );
	}

	void KFGroupClientModule::UpdateMatchToGroup( KFEntity* player, uint64 groupid, uint32 matchid, uint32 maxcount )
	{
		player->UpdateData( KFField::_group_match_id, KFOperateEnum::Set, matchid );
		player->UpdateData( KFField::_basic, KFField::_max_group_count, KFOperateEnum::Set, maxcount );

		KFMsg::S2SUpdateGroupMatchReq req;
		req.set_groupid( groupid );
		req.set_matchid( matchid );
		req.set_maxcount( maxcount );
		req.set_playerid( player->GetKeyID() );
		SendMessageToGroup( groupid, KFMsg::S2S_UPDATE_GROUP_MATCH_REQ, &req );
	}
	
	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleInviteMatchGroupReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgInviteMatchGroupReq );

		// 已经在匹配, 不能邀请
		auto kfobject = player->GetData();
		auto matchid = kfobject->GetValue< uint32 >( KFField::_match_id );
		if ( matchid != _invalid_int )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupInviteInMatch );
		}

		auto maxcount = _kf_match->GetMatchMaxCount( kfmsg.matchid() );
		if ( maxcount == 1 )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupInviteInMatch );
		}
		
		// 判断队伍人数
		auto kfgroupmember = kfobject->FindData( KFField::_group_member );
		if ( ( kfgroupmember->Size() + 1 ) >= maxcount )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupMemberIsFull );
		}

		// 先不要创建队伍
		auto groupid = PrepareMatchGroup( player, kfmsg.matchid(), maxcount );

		// 转发给被邀请者
		KFMsg::S2SReceiveInviteMatchGroupReq req;
		req.set_groupid( groupid );
		req.set_source( kfmsg.source() );
		req.set_matchid( kfmsg.matchid() );
		req.set_playerid( kfmsg.playerid() );
		req.set_playername( kfmsg.playername() );
		req.set_inviterplayerid( playerid );
		req.set_inviterserverid( KFGlobal::Instance()->_app_id );
		auto kfbasic = kfobject->FindData( KFField::_basic );
		_kf_kernel->SerializeToClient( kfbasic, req.mutable_inviter() );
		auto ok = _kf_route->SendMessageToRoute( kfmsg.serverid(), kfmsg.playerid(), KFMsg::S2S_RECEIVE_INVITE_MATCH_GROUP_REQ, &req );
		if ( !ok )
		{
			_kf_display->SendToClient( player, KFMsg::GroupServerBusy );
		}
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleReceiveInviteMatchGroupReq )
	{
		__PROTO_PARSE__( KFMsg::S2SReceiveInviteMatchGroupReq );

		auto player = _kf_player->FindPlayer( kfmsg.playerid(), __FUNCTION_LINE__ );
		if ( player == nullptr )
		{
			return _kf_display->SendToPlayer( kfmsg.inviterserverid(), kfmsg.inviterplayerid(),
				KFMsg::GroupPlayerOffline, kfmsg.playername() );
		}

		auto kfobject = player->GetData();
		auto groupid = kfobject->GetValue< uint32 >( KFField::_basic, KFField::_group_id );
		if ( groupid != _invalid_int )
		{
			return _kf_display->SendToPlayer( kfmsg.inviterserverid(), kfmsg.inviterplayerid(),
				KFMsg::GroupAlreadyInGroup, kfmsg.playername() );
		}

		// 如果有设置5分钟内部接受邀请
		auto refusegrouptime = kfobject->GetValue< uint64 >( KFField::_refuse_group_invite );
		if ( refusegrouptime > KFGlobal::Instance()->_real_time )
		{
			return _kf_display->SendToPlayer( kfmsg.inviterserverid(), kfmsg.inviterplayerid(),
				KFMsg::GroupRefuseInvite, kfmsg.playername() );
		}

		_kf_display->SendToPlayer( kfmsg.inviterserverid(), kfmsg.inviterplayerid(),
			KFMsg::InviteMatchGroupOK, kfmsg.playername() );

		// 添加到邀请列表
		auto kfinviterecord = kfobject->FindData( KFField::_group_invite );
		auto kfinvite = _kf_kernel->CreateObject( kfinviterecord->GetDataSetting() );
		kfinvite->SetValue< uint64 >( KFField::_id, kfmsg.groupid() );
		kfinvite->SetValue< uint32 >( KFField::_match_id, kfmsg.matchid() );
		kfinvite->SetValue< std::string >( KFField::_source, kfmsg.source() );
		kfinvite->SetValue< uint64 >( KFField::_time, KFGlobal::Instance()->_real_time );
		
		auto kfbasic = kfinvite->FindData( KFField::_basic );
		_kf_kernel->ParseFromProto( kfbasic, &kfmsg.inviter() );

		player->RemoveData( kfinviterecord, kfmsg.groupid() );
		player->AddData( kfinviterecord, kfmsg.groupid(), kfinvite );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleReplyInviteMatchGroupReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgReplyInviteMatchGroupReq );

		auto kfobject = player->GetData();
		auto kfinviterecord = kfobject->FindData( KFField::_group_invite );
		auto kfinvite = kfinviterecord->FindData( kfmsg.groupid() );
		if ( kfinvite == nullptr )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupInviteNotExist );
		}
		
		if ( kfmsg.operate() == KFMsg::InviteEnum::Refuse || kfmsg.operate() == KFMsg::InviteEnum::RefuseMinute )
		{
			if ( kfmsg.operate() == KFMsg::InviteEnum::RefuseMinute )
			{
				auto refusetime = KFGlobal::Instance()->_real_time + _kf_group_config->_refuse_time;
				kfobject->SetValue< uint64 >( KFField::_refuse_group_invite, refusetime );
			}

			auto name = kfobject->GetValue< std::string >( KFField::_basic, KFField::_name );

			// 转发消息, 拒绝了邀请
			auto kfbasic = kfinvite->FindData( KFField::_basic );
			_kf_display->SendToPlayer( kfbasic, KFMsg::GroupRefuseYourInvite, name );
		}
		else if ( kfmsg.operate() == KFMsg::InviteEnum::Consent )
		{
			auto groupid = kfobject->GetValue< uint64 >( KFField::_basic, KFField::_group_id );
			if ( groupid != _invalid_int )
			{
				return _kf_display->SendToClient( player, KFMsg::GroupInGroup );
			}

			// 防止操作太快, 加入两个队伍
			auto operatetime = kfobject->GetValue< uint64 >( KFField::_operate_time );
			if ( KFGlobal::Instance()->_game_time < operatetime + 2000 )
			{
				return _kf_display->SendToClient( player, KFMsg::OperateFrequently );
			}

			// 判断邀请时间
			auto invitetime = kfinvite->GetValue< uint64 >( KFField::_time );
			if ( KFGlobal::Instance()->_real_time > invitetime + _kf_group_config->_invite_valid_time )
			{
				// 超时
				_kf_display->SendToClient( player, KFMsg::GroupInviteTimeOut );
			}
			else
			{
				// 转发消息给邀请者, 同意了邀请
				KFMsg::S2SConsentInviteMatchGroupReq req;
				req.set_groupid( kfmsg.groupid() );
				req.set_playerid( player->GetKeyID() );
				req.set_serverid( KFGlobal::Instance()->_app_id );
				FormatMatchGroupMember( player, req.mutable_pbmember() );

				auto kfbasic = kfinvite->FindData( KFField::_basic );
				auto ok = _kf_player->SendMessageToClient( kfbasic, KFMsg::S2S_CONSENT_INVITE_MATCH_GROUP_REQ, &req );
				if ( ok )
				{
					kfobject->SetValue< uint64 >( KFField::_operate_time, KFGlobal::Instance()->_game_time );
				}
				else
				{
					_kf_display->SendToClient( player, KFMsg::GroupServerBusy );
				}
			}
		}

		// 删除邀请信息
		player->RemoveData( kfinviterecord, kfmsg.groupid() );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleConsentInviteMatchGroupReq )
	{
		__PROTO_PARSE__( KFMsg::S2SConsentInviteMatchGroupReq );

		auto playerid = __KF_DATA_ID__( kfguid );
		auto player = _kf_player->FindPlayer( playerid, __FUNCTION_LINE__ );
		if ( player == nullptr )
		{
			return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.playerid(), KFMsg::GroupNotExist );
		}

		auto kfobject = player->GetData();
		auto kfbasic = kfobject->FindData( KFField::_basic );
		auto groupid = kfbasic->GetValue< uint64 >( KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			// 没有队伍, 创建一个新的队伍
			groupid = kfobject->GetValue< uint64 >( KFField::_group_id );
			if ( groupid == _invalid_int )
			{
				groupid = kfmsg.groupid();
			}

			// 创建队伍
			auto ok = CreateMatchGroup( player, groupid );
			if ( !ok )
			{
				return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.playerid(), KFMsg::GroupServerBusy );
			}
		}
		else
		{
			// 队长已经在其他队伍中, 不能加入队伍
			if ( groupid != kfmsg.groupid() )
			{
				return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.playerid(), KFMsg::GroupNotExist );
			}

			// 判断队员数量
			auto maxgroupcount = kfbasic->GetValue< uint32 >( KFField::_max_group_count );
			auto kfmemberrecord = kfobject->FindData( KFField::_group_member );
			if ( kfmemberrecord->Size() + 1 >= maxgroupcount )
			{
				return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.playerid(), KFMsg::GroupMemberIsFull );
			}
		}
		
		// 发送到集群
		// 加入队员
		KFMsg::S2SAddMatchGroupMemberReq req;
		req.set_groupid( groupid );
		req.set_playerid( kfmsg.playerid() );
		req.set_serverid( kfmsg.serverid() );
		req.mutable_pbmember()->CopyFrom( kfmsg.pbmember() );
		auto ok = SendMessageToGroup( groupid, KFMsg::S2S_ADD_MATCH_GROUP_MEMBER_REQ, &req );
		if ( !ok )
		{
			return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.playerid(), KFMsg::GroupServerBusy );
		}
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleAddMatchGroupMemberAck )
	{
		__ROUTE_PROTO_PARSE__( KFMsg::S2SAddMatchGroupMemberAck );

		auto kfobject = player->GetData();
		auto kfmemberrecord = kfobject->FindData( KFField::_group_member );

		for ( auto i = 0; i < kfmsg.pbmember_size(); ++i )
		{
			auto pbmember = &kfmsg.pbmember( i );

			auto kfmember = _kf_kernel->CreateObject( kfmemberrecord->GetDataSetting(), pbmember );
			player->AddData( kfmemberrecord, kfmember );

			if ( !kfmsg.has_groupid() )
			{
				auto name = kfmember->GetValue< std::string >( KFField::_basic, KFField::_name );
				_kf_display->SendToClient( player, KFMsg::GroupMemberJoin, name );
			}
		}

		// 如果需要设置队伍id
		if ( kfmsg.has_groupid() )
		{
			UpdateMatchGroupStatus( player, kfmsg.groupid(), kfmsg.iscaptain(), kfmsg.matchid(), kfmsg.maxcount() );

			// 删除邀请列表
			player->RemoveData( KFField::_group_invite );

			if ( kfmsg.newadd() )
			{
				_kf_display->SendToClient( player, KFMsg::GroupJoinOK );
			}
		}

		// 更新当前数量
		auto nowcount = kfmemberrecord->Size() + 1;
		player->UpdateData( KFField::_basic, KFField::_now_group_count, KFOperateEnum::Set, nowcount );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleRemoveMatchGroupMemberAck )
	{
		__ROUTE_PROTO_PARSE__( KFMsg::S2SRemoveMatchGroupMemberAck );

		auto kfobject = player->GetData();
		auto kfmemberrecord = kfobject->FindData( KFField::_group_member );

		auto kfmember = kfmemberrecord->FindData( kfmsg.memberid() );
		if ( kfmember != nullptr )
		{
			auto name = kfmember->GetValue< std::string >( KFField::_basic, KFField::_name );
			_kf_display->SendToClient( player, KFMsg::GroupMemberLeave, name );
		}

		player->RemoveData( kfmemberrecord, kfmsg.memberid() );

		// 更新当前数量
		auto nowcount = kfmemberrecord->Size() + 1;
		player->UpdateData( KFField::_basic, KFField::_now_group_count, KFOperateEnum::Set, nowcount );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleLeaveMatchGroupReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgLeaveMatchGroupReq );

		auto kfobject = player->GetData();
		auto groupid = kfobject->GetValue< uint64 >( KFField::_basic, KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupNotInGroup );
		}

		// 离开队伍
		ProcessLeaveMatchGroup( player );

		// 通知组队服务器
		KFMsg::S2SLeaveMatchGroupReq req;
		req.set_groupid( groupid );
		req.set_playerid( playerid );
		auto ok = SendMessageToGroup( groupid, KFMsg::S2S_LEAVE_MATCH_GROUP_REQ, &req );
		if ( !ok )
		{
			_kf_display->SendToClient( player, KFMsg::GroupServerBusy );
		}
	}

	void KFGroupClientModule::ProcessLeaveMatchGroup( KFEntity* player )
	{
		// 删除队员
		player->RemoveData( KFField::_group_member );
		player->RemoveData( KFField::_group_invite );
		player->RemoveData( KFField::_group_apply );

		// 设置队伍id
		UpdateMatchGroupStatus( player, _invalid_int, _invalid_int, _invalid_int, _invalid_int );

		// 通知离开队伍
		_kf_display->SendToClient( player, KFMsg::GroupLeaveOK );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleKickMatchGroupReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgKickMatchGroupReq );

		auto kfobject = player->GetData();
		auto groupid = kfobject->GetValue< uint64 >( KFField::_basic, KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupNotInGroup );
		}

		if ( kfmsg.memberid() == playerid )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupCanNotKickSelf );
		}

		// 通知组队服务器
		KFMsg::S2SKickMatchGroupReq req;
		req.set_groupid( groupid );
		req.set_captainid( playerid );
		req.set_memberid( kfmsg.memberid() );
		req.set_serverid( KFGlobal::Instance()->_app_id );
		auto ok = SendMessageToGroup( groupid, KFMsg::S2S_KICK_MATCH_GROUP_REQ, &req );
		if ( !ok )
		{
			_kf_display->SendToClient( player, KFMsg::GroupServerBusy );
		}
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleLeaveMatchGroupAck )
	{
		__SERVER_PROTO_PARSE__( KFMsg::S2SLeaveMatchGroupAck );

		// 离开队伍
		ProcessLeaveMatchGroup( player );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleApplyMatchGroupReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgApplyMatchGroupReq );

		auto kfobject = player->GetData();
		auto groupid = kfobject->GetValue< uint64 >( KFField::_basic, KFField::_group_id );
		if ( groupid != _invalid_int )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupInGroup );
		}

		KFMsg::S2SApplyMatchGroupReq req;
		req.set_groupid( kfmsg.groupid() );
		req.set_playerid( playerid );
		req.set_serverid( KFGlobal::Instance()->_app_id );

		auto kfbasic = kfobject->FindData( KFField::_basic );
		_kf_kernel->SerializeToClient( kfbasic, req.mutable_pbmember() );

		auto ok = SendMessageToGroup( kfmsg.groupid(), KFMsg::S2S_APPLY_MATCH_GROUP_REQ, &req );
		if ( !ok )
		{
			_kf_display->SendToClient( player, KFMsg::GroupServerBusy );
		}
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleApplyMatchGroupAck )
	{
		__ROUTE_PROTO_PARSE__( KFMsg::S2SApplyMatchGroupAck );

		// 添加到申请列表
		auto kfobject = player->GetData();
		auto matchid = kfobject->GetValue< uint32 >( KFField::_match_id );
		if ( matchid != _invalid_int )
		{
			return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.playerid(), KFMsg::GroupApplyInMatch );
		}

		auto kfapplyrecord = kfobject->FindData( KFField::_group_apply );
		auto kfapply = kfapplyrecord->FindData( kfmsg.playerid() );
		if ( kfapply != nullptr )
		{
			player->RemoveData( kfapplyrecord, kfmsg.playerid() );
		}

		kfapply = _kf_kernel->CreateObject( kfapplyrecord->GetDataSetting() );
		kfapply->SetValue< uint32 >( KFField::_id, kfmsg.playerid() );
		kfapply->SetValue< uint64 >( KFField::_time, KFGlobal::Instance()->_real_time );

		auto kfbasic = kfapply->FindData( KFField::_basic );
		_kf_kernel->ParseFromProto( kfbasic, &kfmsg.pbmember() );

		player->AddData( kfapplyrecord, kfapply );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleReplyApplyMatchGroupReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgReplyApplyMatchGroupReq );

		auto kfobject = player->GetData();
		auto kfbasic = kfobject->FindData( KFField::_basic );
		auto groupid = kfbasic->GetValue< uint64 >( KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupNotInGroup );
		}

		// 是否存在邀请
		auto kfapply = kfobject->FindData( KFField::_group_apply, kfmsg.applyid(), KFField::_basic );
		if ( kfapply == nullptr )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupApplyNotExist );
		}

		if ( kfmsg.operate() == KFMsg::InviteEnum::Refuse )
		{
			// 拒绝
			auto name = kfbasic->GetValue< std::string >( KFField::_name );
			_kf_display->SendToPlayer( kfapply, KFMsg::GroupRefuseApply, name );
		}
		else if ( kfmsg.operate() == KFMsg::InviteEnum::Consent )
		{
			auto applytime = kfapply->GetValue< uint64 >( KFField::_time );
			if ( KFGlobal::Instance()->_real_time > ( applytime + _kf_group_config->_invite_valid_time ) )
			{
				_kf_display->SendToClient( player, KFMsg::GroupApplyTimeOut );
			}
			else
			{
				// 判断数量
				auto kfgroupmember = kfobject->FindData( KFField::_group_member );
				auto maxcount = kfbasic->GetValue< uint32 >( KFField::_max_group_count );
				if ( ( kfgroupmember->Size() + 1 ) >= maxcount )
				{
					_kf_display->SendToClient( player, KFMsg::GroupMemberIsFull );
				}
				else
				{
					// 转发给玩家, 同意了申请
					KFMsg::S2SConsentApplyMatchGroupAck ack;
					ack.set_groupid( groupid );
					ack.set_captainid( playerid );
					ack.set_serverid( KFGlobal::Instance()->_app_id );
					ack.set_playerid( kfmsg.applyid() );
					ack.set_playername( kfapply->GetValue< std::string >( KFField::_basic, KFField::_name ) );
					auto ok = _kf_player->SendMessageToClient( kfapply, KFMsg::S2S_CONSENT_APPLY_MATCH_GROUP_ACK, &ack );
					if ( ok )
					{
						return _kf_display->SendToClient( player, KFMsg::GroupServerBusy );
					}
				}
			}
		}

		// 删除邀请信息
		player->RemoveData( KFField::_group_apply, kfmsg.applyid() );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleConsentApplyMatchGroupAck )
	{
		__PROTO_PARSE__( KFMsg::S2SConsentApplyMatchGroupAck );

		auto player = _kf_player->FindPlayer( kfmsg.playerid(), __FUNCTION_LINE__ );
		if ( player == nullptr )
		{
			return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.captainid(), KFMsg::GroupPlayerOffline, kfmsg.playername() );
		}

		// 判断是否有队伍
		auto kfobject = player->GetData();
		auto kfbasic = kfobject->FindData( KFField::_basic );
		auto groupid = kfbasic->GetValue< uint64 >( KFField::_group_id );
		if ( groupid != _invalid_int )
		{
			return _kf_display->SendToPlayer( kfmsg.serverid(), kfmsg.captainid(), KFMsg::GroupAlreadyInGroup, kfmsg.playername() );
		}

		// 加入队员
		KFMsg::S2SAddMatchGroupMemberReq req;
		req.set_groupid( kfmsg.groupid() );
		req.set_playerid( kfmsg.playerid() );
		req.set_serverid( KFGlobal::Instance()->_app_id );
		FormatMatchGroupMember( player, req.mutable_pbmember() );
		auto ok = SendMessageToGroup( groupid, KFMsg::S2S_ADD_MATCH_GROUP_MEMBER_REQ, &req );
		if ( !ok )
		{
			_kf_display->SendToPlayer( kfmsg.playerid(), kfmsg.captainid(), KFMsg::GroupServerBusy );
		}
	}

	void KFGroupClientModule::OnEnterQueryMatchGroup( KFEntity* player )
	{
		auto kfobject = player->GetData();
		auto kfbasic = kfobject->FindData( KFField::_basic );
		auto groupid = kfbasic->GetValue< uint64 >( KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return;
		}

		// 先设置成没有队伍
		kfbasic->SetValue< uint64 >( KFField::_group_id, _invalid_int );

		// 查询玩家队伍信息
		KFMsg::S2SOnLineQueryMatchGroupReq req;
		req.set_groupid( groupid );
		req.set_playerid( player->GetKeyID() );
		req.set_serverid( KFGlobal::Instance()->_app_id );
		SendMessageToGroup( groupid, KFMsg::S2S_ONLINE_QUERY_MATCH_GROUP_REQ, &req );
	}

	void KFGroupClientModule::OnLeaveUpdateMatchGroup( KFEntity* player )
	{
		auto kfobject = player->GetData();
		auto groupid = kfobject->GetValue< uint64 >( KFField::_basic, KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return;
		}

		KFMsg::S2SOffLineUpdateMatchGroupReq req;
		req.set_groupid( groupid );
		req.set_playerid( player->GetKeyID() );
		SendMessageToGroup( groupid, KFMsg::S2S_OFFLINE_UPDATE_MATCH_GROUP_REQ, &req );
	}

	void KFGroupClientModule::OnUpdateDataCallBack( KFEntity* player, uint64 key, KFData* kfdata, uint32 operate, uint64 value, uint64 oldvalue, uint64 newvalue )
	{
		OnUpdateDataToGroup( player, kfdata );
	}

	void KFGroupClientModule::OnUpdateStringCallBack( KFEntity* player, KFData* kfdata, const std::string& value )
	{
		OnUpdateDataToGroup( player, kfdata );
	}

	void KFGroupClientModule::OnUpdateDataToGroup( KFEntity* player, KFData* kfdata )
	{
		auto kfparent = kfdata->GetParent();
		if ( !kfdata->HaveFlagMask( KFDataDefine::Mask_Group_Data ) ||
			!kfparent->HaveFlagMask( KFDataDefine::Mask_Group_Data ) )
		{
			return;
		}

		// 是否有队伍
		auto kfobject = player->GetData();
		auto groupid = kfobject->GetValue< uint64 >( KFField::_basic, KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return;
		}
						
		// 更新到组队服务器
		KFMsg::PBStrings pbstrings;
		auto pbstring = pbstrings.add_pbstring();
		pbstring->set_name( kfdata->GetName() );
		pbstring->set_value( kfdata->ToString() );
		UpdateMemberToMatchGroup( groupid, player->GetKeyID(), kfparent->GetName(), pbstrings );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleUpdateGroupMemberAck )
	{
		__ROUTE_PROTO_PARSE__( KFMsg::S2SUpdateGroupMemberAck );

		auto kfobject = player->GetData();
		auto kfmember = kfobject->FindData( KFField::_group_member, kfmsg.memberid() );
		if ( kfmember == nullptr )
		{
			return;
		}
		
		auto kfupdate = kfmember->FindData( kfmsg.dataname() );
		if ( kfupdate == nullptr )
		{
			kfupdate = kfmember;
		}
		
		auto pbstrings = &kfmsg.pbstrings();
		for ( auto i = 0; i < pbstrings->pbstring_size(); ++i )
		{
			auto pbstring = &pbstrings->pbstring( i );
			player->UpdateData( kfupdate, pbstring->name(), pbstring->value() );
		}
	}

	bool KFGroupClientModule::UpdateMemberToMatchGroup( uint64 groupid, uint32 playerid, const std::string& dataname, const KFMsg::PBStrings& pbstrings )
	{
		KFMsg::S2SUpdateGroupMemberReq req;
		req.set_groupid( groupid );
		req.set_memberid( playerid );
		req.set_dataname( dataname );
		req.mutable_pbstrings()->CopyFrom( pbstrings );
		return SendMessageToGroup( groupid, KFMsg::S2S_UPDATE_GROUP_MEMBER_REQ, &req );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleMatchGroupPrepareReq )
	{
		__CLIENT_PROTO_PARSE__( KFMsg::MsgMatchGroupPrepareReq );

		auto kfobject = player->GetData();
		auto groupid = kfobject->GetValue< uint64 >( KFField::_basic, KFField::_group_id );
		if ( groupid == _invalid_int )
		{
			return _kf_display->SendToClient( player, KFMsg::GroupNotInGroup );
		}
		
		// 更新状态
		player->UpdateData( KFField::_prepare, KFOperateEnum::Set, kfmsg.prepare() );

		// 更新准备状态
		KFMsg::PBStrings pbstrings;
		auto pbstring = pbstrings.add_pbstring();
		pbstring->set_name( KFField::_prepare );
		pbstring->set_value( KFUtility::ToString< uint32 >( kfmsg.prepare() ) );
		auto ok = UpdateMemberToMatchGroup( groupid, playerid, _invalid_str, pbstrings );
		if ( !ok )
		{
			_kf_display->SendToClient( player, KFMsg::GroupServerBusy );
		}
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleUpdateGroupCaptainAck )
	{
		__ROUTE_PROTO_PARSE__( KFMsg::S2SUpdateGroupCaptainAck );

		player->UpdateData( KFField::_is_captain, KFOperateEnum::Set, kfmsg.iscaptain() );
		if ( kfmsg.iscaptain() == 1 )
		{
			_kf_display->SendToClient( player, KFMsg::GroupIsNewCaption );
		}
	}

	bool KFGroupClientModule::IsGroupMember( KFEntity* player, uint32 playerid )
	{
		if ( player->GetKeyID() == playerid )
		{
			return true;
		}

		auto kfobject = player->GetData();
		auto kfmember = kfobject->FindData( KFField::_group_member, playerid );
		return kfmember != nullptr;
	}

	void KFGroupClientModule::RemoveGroupMember( uint64 groupid, uint32 playerid )
	{
		// 通知组队服务器
		KFMsg::S2SLeaveMatchGroupReq req;
		req.set_groupid( groupid );
		req.set_playerid( playerid );
		SendMessageToGroup( groupid, KFMsg::S2S_LEAVE_MATCH_GROUP_REQ, &req );
	}

	__KF_MESSAGE_FUNCTION__( KFGroupClientModule::HandleUpdateGroupMatchAck )
	{
		__ROUTE_PROTO_PARSE__( KFMsg::S2SUpdateGroupMatchAck );

		player->UpdateData( KFField::_group_match_id, KFOperateEnum::Set, kfmsg.matchid() );
		player->UpdateData( KFField::_basic, KFField::_max_group_count, KFOperateEnum::Set, kfmsg.maxcount() );
	}
}