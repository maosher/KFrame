﻿#ifndef __KF_DEFINE_H__
#define __KF_DEFINE_H__

#include "KFRouter.h"

namespace google
{
    namespace protobuf
    {
        class Message;

        template <typename Key, typename T>
        class Map;
    }
}

namespace KFMsg
{
    class PBObject;
    class PBRecord;
    class PBLoginData;
    class PBShowElement;
    class ListenData;
}

namespace KFrame
{
    class KFData;
    class KFEntity;
    ////////////////////////////////////////////////////////////////
    static const uint64 _invalid_int = 0u;
    static const std::string _invalid_str = "";
    static const std::string _globbing_str = "*";
    static const std::string _route_cluster_name = "route";
    static const std::string _route_cluster_key = "route@kframe,./";
    /////////////////////////////////////////////////////////////////////////
    typedef std::list< std::string > ListString;
    typedef std::vector< std::string > VectorString;
    typedef std::unordered_map< std::string, std::string > MapString;
    typedef std::list< MapString > ListMapString;
    /////////////////////////////////////////////////////////////////////////
    // 消息函数
    typedef std::function<void( const Route& route, uint32 msgid, const char* data, uint32 length )> KFMessageFunction;
#define __KF_MESSAGE_FUNCTION__( function ) \
    void function( const Route& route, uint32 msgid, const char* data, uint32 length )

    typedef std::function<bool( const Route& route, uint32 msgid, const char* data, uint32 length )> KFTranspondFunction;
#define __KF_TRANSPOND_MESSAGE_FUNCTION__( function ) \
    bool function( const Route& route, uint32 msgid, const char* data, uint32 length )

    // http client 回调函数
    typedef std::function<void( std::string& data, std::string& result, std::string& callback )> KFHttpClientFunction;

    // httphandle 函数
    typedef std::function< std::string( const std::string& ip, const std::string& data )> KFHttpMethodFunction;

    // 集群连接
    typedef std::function< void( uint64 ) > KFClusterConnectionFunction;
    /////////////////////////////////////////////////////////////////////////
}

#endif // !__KF_DEFINE_H__
