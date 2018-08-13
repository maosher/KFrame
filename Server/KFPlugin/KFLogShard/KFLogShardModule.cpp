﻿#include "KFLogShardModule.h"
#include "spdlog/contrib/sinks/date_and_hour_file_sink.h"

namespace KFrame
{

#ifdef __USE_WORKER__
    #define __REGISTER_LOG_MESSAGE__ __REGISTER_WORKER_MESSAGE__
    #define __UNREGISTER_LOG_MESSAGE__ __UNREGISTER_WORKER_MESSAGE__
#else
    #define __REGISTER_LOG_MESSAGE__ __REGISTER_MESSAGE__
    #define __UNREGISTER_LOG_MESSAGE__ __UNREGISTER_MESSAGE__
#endif

    KFLogShardModule::~KFLogShardModule()
    {
        spdlog::drop_all();
    }

    void KFLogShardModule::BeforeRun()
    {
        __REGISTER_LOG_MESSAGE__( KFMsg::S2S_LOG_REQ, &KFLogShardModule::HandleRemoteLogReq );
    }

    void KFLogShardModule::BeforeShut()
    {
        __UNREGISTER_LOG_MESSAGE__( KFMsg::S2S_LOG_REQ );
    }

    //////////////////////////////////////////////////////////////////////////

    void KFLogShardModule::Log( uint32 level, uint32 category, uint32 zoneid, const std::string& appname, const std::string& apptype, uint32 appid, const std::string& loginfo )
    {
        if ( level < spdlog::level::trace || level >= spdlog::level::off )
        {
            assert( 0 );
            return;
        }

        auto& logger = GetLogger( category, zoneid, appname, apptype, appid );
        if ( logger == nullptr )
        {
            return;
        }

        logger->log( ( spdlog::level::level_enum )level, loginfo.c_str() );
    }

    void KFLogShardModule::CreateLogger( const std::string& zonename, const std::string& appname, const std::string& apptype, uint32 appid, const std::string& category )
    {
        std::vector<spdlog::sink_ptr> sinks_vec;

        auto strappid = KFAppID::ToString( appid );

#if __KF_SYSTEM__ == __KF_WIN__
        std::string logname = __FORMAT__( "..\\binlog\\{}\\{}-{}-{}-{}.log", zonename, appname, apptype, strappid, category );
#else
        std::string logname = __FORMAT__( "../binlog/{}/{}-{}-{}-{}.log", zonename, appname, apptype, strappid, category );
#endif

        auto date_and_hour_sink = std::make_shared<spdlog::sinks::date_and_hour_file_sink_mt>( logname );
#if defined(__KF_DEBUG__)
#if __KF_SYSTEM__ == __KF_WIN__
        auto color_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
        auto color_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

        sinks_vec.push_back( color_sink );
#endif
        sinks_vec.push_back( date_and_hour_sink );

        std::string name = __FORMAT__( "{}-{}-{}-{}", appname, apptype, appid, category );
        auto remote_logger = std::make_shared<spdlog::async_logger>( name, std::begin( sinks_vec ), std::end( sinks_vec ), 1024 );

#if defined(__KF_DEBUG__)
        remote_logger->set_level( spdlog::level::level_enum::trace );
        remote_logger->set_pattern( "%^[%Y%m%d %H:%M:%S.%e][%l]%v%$" );
        remote_logger->flush_on( spdlog::level::trace );
#else
        remote_logger->set_pattern( "[%Y%m%d %H:%M:%S.%e][%l]%v" );
        remote_logger->flush_on( spdlog::level::warn );
#endif

        spdlog::register_logger( remote_logger );

        _loggers.insert( std::make_pair( name, remote_logger ) );
    }

    const std::shared_ptr<spdlog::logger>& KFLogShardModule::GetLogger( uint32 category, uint32 zoneid, const std::string& appname, const std::string& apptype, uint32 appid )
    {
        static std::shared_ptr<spdlog::logger> NULLPTR_ = nullptr;

        if ( category <= KFLogEnum::None || category >= KFLogEnum::Max )
        {
            assert( 0 );
            return NULLPTR_;
        }

        std::string zonename = __TO_STRING__( zoneid );
        if ( zoneid == 0 )
        {
            zonename = "cluster";
        }

        std::string logger_unique_name = __FORMAT__( "{}-{}-{}-{}", appname, apptype, appid, _log_category_name[category] );
        auto iter = _loggers.find( logger_unique_name );
        if ( iter == _loggers.end() )
        {
            CreateLogger( zonename, appname, apptype, appid, _log_category_name[category] );
        }

        iter = _loggers.find( logger_unique_name );
        return ( iter == _loggers.end() ) ? NULLPTR_ : iter->second;
    }

    //////////////////////////////////////////////////////////////////////////

    __KF_MESSAGE_FUNCTION__( KFLogShardModule::HandleRemoteLogReq )
    {
        __PROTO_PARSE__( KFMsg::S2SLogReq );

        Log( kfmsg.log_level(), kfmsg.log_category(), kfmsg.zone_id(), kfmsg.app_name(), kfmsg.app_type(), kfmsg.app_id(), kfmsg.log_info() );
    }
}