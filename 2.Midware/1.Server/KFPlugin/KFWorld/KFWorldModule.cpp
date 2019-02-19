﻿#include "KFWorldModule.hpp"

namespace KFrame
{
#define __ZONE_TIMER_ID__ 1

    void KFWorldModule::BeforeRun()
    {
        __REGISTER_LOOP_TIMER__( __ZONE_TIMER_ID__, 5000, &KFWorldModule::OnTimerZoneRegister );
        __REGISTER_SERVER_LOST_FUNCTION__( &KFWorldModule::OnServerLostGame );
        __REGISTER_SERVER_DISCOVER_FUNCTION__( &KFWorldModule::OnServerDisCoverGame );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::S2S_UPDATE_ZONE_TO_WORLD_REQ, &KFWorldModule::HandleUpdateZoneToWorldReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_DISCONNECT_ZONE_TO_WORLD_REQ, &KFWorldModule::HandleDisconnectZoneToWorldReq );

        __REGISTER_MESSAGE__( KFMsg::S2S_LOGIN_TO_WORLD_REQ, &KFWorldModule::HandleLoginToWorldReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_SYNC_TO_WORLD_REQ, &KFWorldModule::HandlePlayerSyncToWorldReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_ENTER_TO_WORLD_REQ, &KFWorldModule::HandlePlayerEnterToWorldReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_LEAVE_TO_WORLD_REQ, &KFWorldModule::HandlePlayerLeaveToWorldReq );

        __REGISTER_MESSAGE__( KFMsg::S2S_BROADCAST_TO_GAME, &KFWorldModule::HandleBroadcastToGameReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_TRANSMIT_TO_PLAYER, &KFWorldModule::HandleTransmitToPlayerReq );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_HTTP_FUNCTION__( __KF_STRING__( kickonline ), true, &KFWorldModule::HandleHttpKickOnline );
    }

    void KFWorldModule::BeforeShut()
    {
        __UNREGISTER_TIMER__();
        __UNREGISTER_SERVER_LOST_FUNCTION__();
        __UNREGISTER_SERVER_DISCOVER_FUNCTION__();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __UNREGISTER_MESSAGE__( KFMsg::S2S_UPDATE_ZONE_TO_WORLD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_DISCONNECT_ZONE_TO_WORLD_REQ );

        __UNREGISTER_MESSAGE__( KFMsg::S2S_LOGIN_TO_WORLD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_SYNC_TO_WORLD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_ENTER_TO_WORLD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_LEAVE_TO_WORLD_REQ );

        __UNREGISTER_MESSAGE__( KFMsg::S2S_BROADCAST_TO_GAME );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TRANSMIT_TO_PLAYER );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __UNREGISTER_HTTP_FUNCTION__( __KF_STRING__( kickonline ) );
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFWorldModule::SendToOnline( uint64 playerid, uint32 msgid, ::google::protobuf::Message* message )
    {
        auto kfonline = _kf_online_list.Find( playerid );
        if ( kfonline == nullptr )
        {
            return false;
        }

        kfonline->SendToOnline( msgid, message );
        return true;
    }

    bool KFWorldModule::SendToGame( uint64 gameid, uint32 msgid, ::google::protobuf::Message* message )
    {
        return _kf_tcp_server->SendNetMessage( gameid, msgid, message );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_SERVER_DISCOVER_FUNCTION__( KFWorldModule::OnServerDisCoverGame )
    {
        if ( handletype == __KF_STRING__( game ) )
        {
            _kf_game_conhash.AddHashNode( __KF_STRING__( game ), handleid, 100 );
        }
    }

    __KF_SERVER_LOST_FUNCTION__( KFWorldModule::OnServerLostGame )
    {
        if ( handletype == __KF_STRING__( game ) )
        {
            _kf_game_conhash.RemoveHashNode( handleid );
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    __KF_TIMER_FUNCTION__( KFWorldModule::OnTimerZoneRegister )
    {
        auto zone = _kf_zone->GetZone();
        auto _url = _kf_ip_address->GetAuthUrl() + __KF_STRING__( zoneregister );

        // 注册小区信息
        __JSON_DOCUMENT__( kfjson );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( zoneid ), zone->_id );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( name ), zone->_name );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( zonelogicid ), zone->_logic_id );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( zoneloginid ), zone->_login_id );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( url ), _kf_http_server->GetHttpUrl() );

        auto recvdata = _kf_http_client->STGet( _url, kfjson );
        __JSON_PARSE_STRING__( kfresult, recvdata );
        auto retcode = _kf_http_client->GetCode( kfresult );
        if ( retcode == KFMsg::Ok )
        {
            __UNREGISTER_OBJECT_TIMER__( __ZONE_TIMER_ID__ );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandleUpdateZoneToWorldReq )
    {
        __PROTO_PARSE__( KFMsg::S2SUpdateZoneToWorldReq );

        auto pbdata = kfmsg.zonedata();
        if ( !_zone_data.ip().empty() && _zone_data.count() < pbdata.count() )
        {
            return;
        }

        // 保存小区数据
        _zone_data.CopyFrom( pbdata );

        // 更新给auth服务器
        auto kfzone = _kf_zone->GetZone();
        static auto _url = _kf_ip_address->GetAuthUrl() + __KF_STRING__( zoneupdate );

        __JSON_DOCUMENT__( kfjson );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( zoneid ), kfzone->_id );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( zoneloginid ), kfzone->_login_id );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( ip ), _zone_data.ip() );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( port ), _zone_data.port() );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( count ), _zone_data.count() );
        _kf_http_client->MTGet< KFWorldModule >( _url, kfjson );
    }

    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandleDisconnectZoneToWorldReq )
    {
        __PROTO_PARSE__( KFMsg::S2SDisconnectZoneToWorldReq );

        if ( _zone_data.appid() != kfmsg.appid() )
        {
            return;
        }

        _zone_data.Clear();

        // 除小区信息到auth
        auto kfzone = _kf_zone->GetZone();
        static auto _url = _kf_ip_address->GetAuthUrl() + __KF_STRING__( zoneremove );

        __JSON_DOCUMENT__( kfjson );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( zoneid ), kfzone->_id );
        __JSON_SET_VALUE__( kfjson, __KF_STRING__( zoneloginid ), kfzone->_login_id );
        _kf_http_client->MTGet< KFWorldModule >( _url, kfjson );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandleLoginToWorldReq )
    {
        auto loginid = __ROUTE_SERVER_ID__;
        __PROTO_PARSE__( KFMsg::S2SLoginToWorldReq );

        auto pblogin = &kfmsg.pblogin();
        __LOG_DEBUG__( "player[{}:{}:{}] login world req!", pblogin->account(), pblogin->accountid(), pblogin->playerid() );

        // 踢掉已经在线的玩家, 只有踢下线以后才能登陆
        KickOnline( KFMsg::KickByLogin, pblogin->playerid(), __FUNC_LINE__ );

        // 选择Game服务器
        auto gameid = _kf_game_conhash.FindHashNode( pblogin->playerid() );
        if ( gameid == _invalid_int )
        {
            return SendLoginAckToLogin( KFMsg::LoginNoGameServer, loginid, pblogin->gateid(), pblogin->accountid(), pblogin->sessionid() );
        }

        // 登录请求到game服务器
        KFMsg::S2SLoginToGameReq req;
        req.mutable_pblogin()->CopyFrom( *pblogin );
        auto ok = _kf_tcp_server->SendNetMessage( gameid, KFMsg::S2S_LOGIN_TO_GAME_REQ, &req );
        if ( !ok )
        {
            __LOG_ERROR__( "player[{}:{}:{}] login game failed!", pblogin->account(), pblogin->accountid(), pblogin->playerid() );
            SendLoginAckToLogin( KFMsg::LoginGameServerBusy, loginid, pblogin->gateid(), pblogin->accountid(), pblogin->sessionid() );
        }
    }

    void KFWorldModule::SendLoginAckToLogin( uint32 result, uint64 loginid, uint64 gateid, uint64 accountid, uint64 sessionid )
    {
        KFMsg::S2SLoginToLoginAck ack;
        ack.set_result( result );
        ack.set_gateid( gateid );
        ack.set_accountid( accountid );
        ack.set_sessionid( sessionid );
        auto ok = _kf_tcp_server->SendNetMessage( loginid, KFMsg::S2S_LOGIN_TO_LOGIN_ACK, &ack );
        if ( !ok )
        {
            __LOG_ERROR__( "player[{}] world verify result[{}] failed!", accountid, result );
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandlePlayerSyncToWorldReq )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerSyncToWorldReq );

        auto serverid = __ROUTE_SERVER_ID__;
        for ( auto i = 0; i < kfmsg.playerid_size(); ++i )
        {
            auto playerid = kfmsg.playerid( i );

            auto kfonline = _kf_online_list.Create( playerid );

            kfonline->_game_id = serverid;
            kfonline->_player_id = playerid;
        }
    }

    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandlePlayerEnterToWorldReq )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerEnterToWorldReq );

        auto kfonline = _kf_online_list.Create( kfmsg.playerid() );
        kfonline->_player_id = kfmsg.playerid();
        kfonline->_account_id = kfmsg.accountid();
        kfonline->_game_id = __ROUTE_SERVER_ID__;

        // 更新到认证服务器
        UpdateOnlineToAuth( kfmsg.accountid(), kfmsg.playerid(), true );
    }

    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandlePlayerLeaveToWorldReq )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerLeaveToWorldReq );

        _kf_online_list.Remove( kfmsg.playerid() );

        // 更新到认证服务器
        UpdateOnlineToAuth( kfmsg.accountid(), kfmsg.playerid(), false );
    }

    void KFWorldModule::UpdateOnlineToAuth( uint64 accountid, uint64 playerid, bool online )
    {
        static auto _update_url = _kf_ip_address->GetAuthUrl() + __KF_STRING__( onlinedata );

        // 在线信息
        __JSON_DOCUMENT__( sendjson );
        __JSON_SET_VALUE__( sendjson, __KF_STRING__( accountid ), accountid );

        if ( online )
        {
            __JSON_SET_VALUE__( sendjson, __KF_STRING__( online ), 1 );
            __JSON_SET_VALUE__( sendjson, __KF_STRING__( playerid ), playerid );
            __JSON_SET_VALUE__( sendjson, __KF_STRING__( zoneid ), KFUtility::CalcZoneId( playerid ) );
        }
        else
        {
            __JSON_SET_VALUE__( sendjson, __KF_STRING__( online ), 0 );
        }

        _kf_http_client->MTGet< KFWorldModule >( _update_url, sendjson );
    }

    bool KFWorldModule::KickOnline( uint32 type, uint64 playerid, const char* function, uint32 line )
    {
        auto kfonline = _kf_online_list.Find( playerid );
        if ( kfonline == nullptr )
        {
            return false;
        }

        __LOG_DEBUG__( "kick player[{}]!", playerid );

        // 发送踢出消息
        KFMsg::S2SKickPlayerToGameReq req;
        req.set_type( type );
        req.set_playerid( playerid );
        auto ok = _kf_tcp_server->SendNetMessage( kfonline->_game_id, KFMsg::S2S_KICK_PLAYER_TO_GAME_REQ, &req );
        if ( ok )
        {
            _kf_online_list.Remove( playerid );
            // 发送成功, 先删除在线信息, 避免gameserver掉线以后,登录不正常的问题
        }
        return ok;
    }

    __KF_HTTP_FUNCTION__( KFWorldModule::HandleHttpKickOnline )
    {
        __JSON_PARSE_STRING__( request, data );

        auto playerid = __JSON_GET_UINT64__( request, __KF_STRING__( playerid ) );
        KickOnline( KFMsg::KickByPlatform, playerid, __FUNC_LINE__ );

        return _invalid_str;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////
    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandleTransmitToPlayerReq )
    {
        __PROTO_PARSE__( KFMsg::S2STransmitToPlayer );

        auto kfonline = _kf_online_list.Find( kfmsg.playerid() );
        if ( kfonline == nullptr )
        {
            return;
        }

        auto& msgdata = kfmsg.msgdata();
        kfonline->SendToOnline( kfmsg.msgid(), msgdata.data(), static_cast< uint32 >( msgdata.size() ) );
    }

    __KF_MESSAGE_FUNCTION__( KFWorldModule::HandleBroadcastToGameReq )
    {
        __PROTO_PARSE__( KFMsg::S2SBroadcastToGame );

        auto& msgdata = kfmsg.msgdata();
        _kf_tcp_server->SendMessageToType( __KF_STRING__( game ), kfmsg.msgid(), msgdata.data(), static_cast< uint32 >( msgdata.size() ) );
    }
}
