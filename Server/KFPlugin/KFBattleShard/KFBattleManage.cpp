﻿#include "KFBattleManage.h"

namespace KFrame
{
    KFBattleServer::KFBattleServer()
    {
        Reset();
    }

    void KFBattleServer::Reset()
    {
        _ip.clear();
        _port = _invalid_int;
        _proxy_id = _invalid_int;
        _server_id = _invalid_int;
    }

    bool KFBattleServer::IsValid() const
    {
        if ( _server_id == _invalid_int ||
                _proxy_id == _invalid_int ||
                _ip.empty() ||
                _port == _invalid_int )
        {
            return false;
        }

        return true;
    }
    /////////////////////////////////////////////////////////////////////////
    KFBattleManage::KFBattleManage()
    {
        _battle_redis_driver = nullptr;
    }

    void KFBattleManage::Initialize()
    {
        _battle_redis_driver = _kf_redis->CreateExecute( __KF_STRING__( battle ) );
    }

    void KFBattleManage::RegisterBattleServer( uint32 serverid, uint32 proxyid, const std::string& ip, uint32 port )
    {
        // 为了避免单点问题, 保存到redis缓存中
        // 先删除列表在添加到列表中
        MapString values;
        values[ __KF_STRING__( ip ) ] = ip;
        values[ __KF_STRING__( port ) ] = __TO_STRING__( port );
        values[ __KF_STRING__( proxyid ) ] = __TO_STRING__( proxyid );
        values[ __KF_STRING__( serverid ) ] = __TO_STRING__( serverid );

        _battle_redis_driver->Append( values, "hmset {}:{}", __KF_STRING__( battle ), serverid );
        _battle_redis_driver->Append( "sadd {}:{} {}", __KF_STRING__( ip ), ip, serverid );
        _battle_redis_driver->Pipeline();

        // 设置数量
        UpdateBattleCount( ip );
    }

    void KFBattleManage::UnRegisterBattleServer( uint32 serverid )
    {
        auto kfresult = _battle_redis_driver->QueryString( "hget {}:{} {}", __KF_STRING__( battle ), serverid, __KF_STRING__( ip ) );
        if ( kfresult->_value.empty() )
        {
            return;
        }

        _battle_redis_driver->Append( "del {}:{}", __KF_STRING__( battle ), serverid );
        _battle_redis_driver->Append( "srem {}:{} {}", __KF_STRING__( ip ), kfresult->_value, serverid );
        _battle_redis_driver->Pipeline();

        __LOG_DEBUG__( "unregister battle server[{}|{}]!", serverid, kfresult->_value );

        // 设置数量
        UpdateBattleCount( kfresult->_value );
    }

    void KFBattleManage::AllocBattleServer( uint32 battleserverid, KFBattleServer* battleserver )
    {
        // 找到负载最小的一个
        auto queryserverid = battleserverid;
        if ( queryserverid == _invalid_int )
        {
            auto listresult = _battle_redis_driver->QueryList( "zrevrange {} 0 0", __KF_STRING__( battlelist ) );
            if ( listresult->_value.empty() )
            {
                return __LOG_ERROR__( "no battle server!" );
            }

            auto& queryip = listresult->_value.front();

            // 弹出一个可用的服务器
            auto stringresult = _battle_redis_driver->QueryString( "spop {}:{}", __KF_STRING__( ip ), queryip );
            if ( stringresult->_value.empty() )
            {
                _battle_redis_driver->Execute( "zrem {} {}", __KF_STRING__( battlelist ), queryip );
                return __LOG_ERROR__( "[{}] battle list empty!", queryip );
            }

            // 数量减1
            _battle_redis_driver->Execute( "zincrby {} -1 {}", __KF_STRING__( battlelist ), queryip );

            queryserverid = KFUtility::ToValue< uint32 >( stringresult->_value );
        }

        // 查询所有信息
        auto mapresult = _battle_redis_driver->QueryMap( "hgetall {}:{}", __KF_STRING__( battle ), queryserverid );
        if ( mapresult->_value.empty() )
        {
            return __LOG_ERROR__( "[{}] battle data empty!", queryserverid );
        }

        auto ip = mapresult->_value[ __KF_STRING__( ip ) ];
        auto port = KFUtility::ToValue< uint32 >( mapresult->_value[ __KF_STRING__( port ) ] );
        auto serverid = KFUtility::ToValue< uint32 >( mapresult->_value[ __KF_STRING__( serverid ) ] );
        auto proxyid = KFUtility::ToValue< uint32 >( mapresult->_value[ __KF_STRING__( proxyid ) ] );
        if ( ip.empty() || port == _invalid_int || proxyid == _invalid_int || serverid == _invalid_int )
        {
            return __LOG_ERROR__( "battle data [{}:{}:{}] error!", ip, port, proxyid );
        }

        // 保存数据./
        battleserver->_ip = ip;
        battleserver->_port = port;
        battleserver->_proxy_id = proxyid;
        battleserver->_server_id = serverid;

        if ( battleserverid != _invalid_int )
        {
            // 如果是指定战场, 删除列表
            _battle_redis_driver->Execute( "srem {}:{} {}", __KF_STRING__( ip ), ip, battleserverid );
        }

        // 更新数量
        UpdateBattleCount( ip );
    }

    void KFBattleManage::RemoveBattleServer( uint32 serverid, const std::string& ip )
    {
        _battle_redis_driver->Execute( "srem {}:{} {}", __KF_STRING__( ip ), ip, serverid );

        // 设置数量
        UpdateBattleCount( ip );

        __LOG_DEBUG__( "remove battle server[{}|{}]!", serverid, ip );
    }

    void KFBattleManage::FreeBattleServer( uint32 serverid, const std::string& ip )
    {
        __LOG_DEBUG__( "free battle server[{}|{}!", serverid, ip );

        _battle_redis_driver->Execute( "sadd {}:{} {}", __KF_STRING__( ip ), ip, serverid );

        // 更新数量
        UpdateBattleCount( ip );
    }

    void KFBattleManage::UpdateBattleCount( const std::string& ip )
    {
        auto kfresult = _battle_redis_driver->QueryUInt64( "scard {}:{}", __KF_STRING__( ip ), ip );
        if ( !kfresult->IsOk() )
        {
            return;
        }

        if ( kfresult->_value == _invalid_int )
        {
            _battle_redis_driver->Execute( "zrem {} {}", __KF_STRING__( battlelist ), ip );
        }
        else
        {
            _battle_redis_driver->Execute( "zadd {} {} {}", __KF_STRING__( battlelist ), kfresult->_value, ip );
        }

        __LOG_DEBUG__( "ip[{}] server count[{}]!", ip, kfresult->_value );
    }
}

