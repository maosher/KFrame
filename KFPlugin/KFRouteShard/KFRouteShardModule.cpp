﻿#include "KFRouteShardModule.hpp"

namespace KFrame
{
    void KFMessageData::AddNetMessage( uint32 msgid, const char* data, uint32 length )
    {
        auto netmessge = KFNetMessage::Create( length );
        netmessge->_head._msgid = msgid;
        netmessge->CopyData( data, length );
        _messages.push_back( netmessge );
    }

    void KFMessageData::SendNetMessage()
    {
        if ( _messages.empty() )
        {
            return;
        }

        for ( auto netmessage : _messages )
        {
            __CALL_MESSAGE__( netmessage->_head._route, netmessage->_head._msgid, netmessage->_data, netmessage->_head._length );
            netmessage->Release();
        }
        _messages.clear();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFRouteShardModule::BeforeRun()
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_ALL_REQ, &KFRouteShardModule::HandleRouteMessageToNameAllReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_RAND_REQ, &KFRouteShardModule::HandleRouteMessageToNameRandReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_BALANCE_REQ, &KFRouteShardModule::HandleRouteMessageToNameBalanceReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_OBJECT_REQ, &KFRouteShardModule::HandleRouteMessageToNameObjectReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_SERVER_REQ, &KFRouteShardModule::HandleRouteMessageToServerReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_PLAYER_REQ, &KFRouteShardModule::HandleRouteMessageToPlayerReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_CLIENT_LOST_TO_SHARD_REQ, &KFRouteShardModule::HandleRouteClientLostToShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_SYNC_OBJECT_TO_SHARD_REQ, &KFRouteShardModule::HandleRouteSyncObjectToShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_ADD_OBJECT_TO_SHARD_REQ, &KFRouteShardModule::HandleRouteAddObjectToShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ROUTE_REMOVE_OBJECT_TO_SHARD_REQ, &KFRouteShardModule::HandleRouteRemoveObjectToShardReq );

    }

    void KFRouteShardModule::BeforeShut()
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_ALL_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_RAND_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_BALANCE_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_NAME_OBJECT_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_SERVER_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_MESSAGE_TO_PLAYER_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_CLIENT_LOST_TO_SHARD_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_SYNC_OBJECT_TO_SHARD_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_ADD_OBJECT_TO_SHARD_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ROUTE_REMOVE_OBJECT_TO_SHARD_REQ );
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
#define __FIND_ROUTE_SERVICE__( name )\
    auto routeservice = _route_service_list.Find( name );\
    if ( routeservice == nullptr )\
    {\
        AddRouteFailedMessage( name, msgid, data, length );\
        return __LOG_ERROR__( "can't find service[{}]", name );\
    }\

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteMessageToNameAllReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteMessageToNameAllReq );

        auto pbroute = kfmsg.mutable_pbroute();
        __FIND_ROUTE_SERVICE__( kfmsg.name() );
        for ( auto& iter : routeservice->_server_object_count_list )
        {
            SendRouteMessage( iter.first, pbroute, kfmsg.msgid(), kfmsg.msgdata() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteMessageToNameRandReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteMessageToNameRandReq );

        auto pbroute = kfmsg.mutable_pbroute();
        __FIND_ROUTE_SERVICE__( kfmsg.name() );
        auto randid = ( pbroute->sendid() != 0u ? pbroute->sendid() : pbroute->serverid() );
        auto serverid = routeservice->RandServer( randid );
        if ( serverid == _invalid_int )
        {
            return __LOG_ERROR__( "service[{}] no server", kfmsg.name() );
        }

        SendRouteMessage( serverid, pbroute, kfmsg.msgid(), kfmsg.msgdata() );
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteMessageToNameBalanceReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteMessageToNameBalanceReq );

        __FIND_ROUTE_SERVICE__( kfmsg.name() );
        auto serverid = routeservice->BalanceServer();
        if ( serverid == _invalid_int )
        {
            return __LOG_ERROR__( "service[{}] no server", kfmsg.name() );
        }

        auto pbroute = kfmsg.mutable_pbroute();
        SendRouteMessage( serverid, pbroute, kfmsg.msgid(), kfmsg.msgdata() );
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteMessageToNameObjectReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteMessageToNameObjectReq );

        auto pbroute = kfmsg.mutable_pbroute();
        __FIND_ROUTE_SERVICE__( kfmsg.name() );
        auto serverid = routeservice->ObjectServer( pbroute->recvid() );
        if ( serverid == _invalid_int )
        {
            return __LOG_ERROR__( "service[{}] no server", kfmsg.name() );
        }

        SendRouteMessage( serverid, pbroute, kfmsg.msgid(), kfmsg.msgdata() );
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteMessageToServerReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteMessageToServerReq );

        auto pbroute = kfmsg.mutable_pbroute();
        SendRouteMessage( kfmsg.targetid(), pbroute, kfmsg.msgid(), kfmsg.msgdata() );
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteMessageToPlayerReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteMessageToPlayerReq );

        auto pbroute = kfmsg.mutable_pbroute();
        SendRouteMessage( kfmsg.targetid(), pbroute, kfmsg.msgid(), kfmsg.msgdata() );
    }

    void KFRouteShardModule::AddRouteFailedMessage( const std::string& name, uint32 msgid, const char* data, uint32 length )
    {
        auto messagedata = _route_message_list.Create( name );
        messagedata->AddNetMessage( msgid, data, length );
    }

    void KFRouteShardModule::SendRouteFailedMessage( const std::string& name )
    {
        auto messagedata = _route_message_list.Find( name );
        if ( messagedata == nullptr )
        {
            return;
        }

        messagedata->SendNetMessage();
    }

    void KFRouteShardModule::SendRouteMessage( uint64 clientid, KFMsg::PBRoute* pbroute, uint32 msgid, const std::string& msgdata )
    {
        KFMsg::S2SRouteMessageToClientAck ack;
        ack.set_msgid( msgid );
        ack.set_msgdata( msgdata );
        ack.mutable_pbroute()->CopyFrom( *pbroute );
        auto ok = _kf_cluster_shard->SendToClient( clientid, KFMsg::S2S_ROUTE_MESSAGE_TO_CLIENT_ACK, &ack );
        if ( !ok )
        {
            __LOG_ERROR__( "send=[{}] msg[{}] to recv=[{}] failed", KFAppId::ToString( pbroute->serverid() ), msgid, KFAppId::ToString( clientid ) );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteSyncObjectToShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteSyncObjectToShardReq );

        auto routeservice = _route_service_list.Create( kfmsg.name() );
        routeservice->_name = kfmsg.name();

        routeservice->RemoveServer( kfmsg.clientid() );
        routeservice->AddServer( kfmsg.clientid() );

        for ( auto i = 0; i < kfmsg.objectid_size(); ++i )
        {
            routeservice->AddObject( kfmsg.clientid(), kfmsg.objectid( i ), kfmsg.objectid_size() );
        }

        SendRouteFailedMessage( kfmsg.name() );
        __LOG_INFO__( "service[{}:{}] register ok", kfmsg.name(), KFAppId::ToString( kfmsg.clientid() ) );
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteAddObjectToShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteAddObjectToShardReq );

        auto routeservice = _route_service_list.Find( kfmsg.name() );
        if ( routeservice == nullptr )
        {
            return __LOG_ERROR__( "service[{}:{}] not find", kfmsg.name(), KFAppId::ToString( kfmsg.clientid() ) );
        }
        routeservice->AddObject( kfmsg.clientid(), kfmsg.objectid(), kfmsg.objectcount() );
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteRemoveObjectToShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteRemoveObjectToShardReq );

        auto routeservice = _route_service_list.Find( kfmsg.name() );
        if ( routeservice == nullptr )
        {
            return __LOG_ERROR__( "service[{}:{}] not find", kfmsg.name(), KFAppId::ToString( kfmsg.clientid() ) );
        }

        routeservice->RemoveObject( kfmsg.clientid(), kfmsg.objectid(), kfmsg.objectcount() );
    }

    __KF_MESSAGE_FUNCTION__( KFRouteShardModule::HandleRouteClientLostToShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRouteClientLostToShardReq );

        __LOG_WARN__( "route client[{}:{}] lost", kfmsg.name(), KFAppId::ToString( kfmsg.clientid() ) );

        auto routeservice = _route_service_list.Find( kfmsg.name() );
        if ( routeservice == nullptr )
        {
            return;
        }

        routeservice->RemoveServer( kfmsg.clientid() );
    }
}
