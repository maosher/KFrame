﻿#include "KFDeployClientModule.hpp"
#include "KFProtocol/KFProtocol.h"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    void KFDeployClientModule::BeforeRun()
    {
        __REGISTER_TCP_CLIENT_CONNECTION__( &KFDeployClientModule::OnClientConnectAgent );
        __REGISTER_TCP_CLIENT_SHUTDOWN__( &KFDeployClientModule::OnClientLostAgent );
        ////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::S2S_DEPLOY_COMMAND_TO_CLIENT_REQ, &KFDeployClientModule::HandleDeployCommandToClientReq );
    }

    void KFDeployClientModule::ShutDown()
    {
        __UN_TIMER_0__();
        __UN_TCP_CLIENT_CONNECTION__();
        __UN_TCP_CLIENT_SHUTDOWN__();
        /////////////////////////////////////////////////////////////////////////
        __UN_MESSAGE__( KFMsg::S2S_DEPLOY_COMMAND_TO_CLIENT_REQ );
    }

    void KFDeployClientModule::PrepareRun()
    {
        auto agentdata = KFGlobal::Instance()->_startup_params[ __STRING__( agent ) ];
        if ( agentdata.empty() )
        {
            return;
        }

        _agent_id = KFUtility::SplitValue<uint64>( agentdata, "|" );
        _agent_ip = KFUtility::SplitString( agentdata, "|" );
        _agent_port = KFUtility::SplitValue< uint32 >( agentdata, "|" );
        _kf_tcp_client->StartClient( __STRING__( deploy ), __STRING__( agent ), _agent_id, _agent_ip, _agent_port );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_NET_EVENT_FUNCTION__( KFDeployClientModule::OnClientConnectAgent )
    {
        if ( netdata->_id == _agent_id )
        {
            // 启动定时器
            __LOOP_TIMER_1__( _agent_id, 45000, 10000, &KFDeployClientModule::OnTimerSendHeartbeatToAgent );
        }
    }

    __KF_NET_EVENT_FUNCTION__( KFDeployClientModule::OnClientLostAgent )
    {
        if ( netdata->_id == _agent_id )
        {
            __UN_TIMER_1__( _agent_id );

            // 重新开始连接
            _kf_tcp_client->StartClient( __STRING__( deploy ), __STRING__( agent ), _agent_id, _agent_ip, _agent_port );
        }
    }

    __KF_TIMER_FUNCTION__( KFDeployClientModule::OnTimerSendHeartbeatToAgent )
    {
        KFMsg::S2SDeployHeartbeatToAgentReq req;
        req.set_id( KFGlobal::Instance()->_app_id->GetId() );
        _kf_tcp_client->SendNetMessage( _agent_id, KFMsg::S2S_DEPLOY_HEARTBEAT_TO_AGENT_REQ, &req );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFDeployClientModule::AddFunction( const std::string& command, const std::string& module, KFDeployFunction& function )
    {
        auto kfcommand = _command_data.Create( command );
        auto kffunction = kfcommand->_functions.Create( module );
        kffunction->_function = function;
    }

    void KFDeployClientModule::RemoveFunction( const std::string& command, const std::string& module )
    {
        auto kfcommand = _command_data.Create( command );
        kfcommand->_functions.Remove( module );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_MESSAGE_FUNCTION__( KFDeployClientModule::HandleDeployCommandToClientReq )
    {
        __PROTO_PARSE__( KFMsg::S2SDeployCommandToClientReq );

        auto pbdeploy = &kfmsg.deploycommand();

        // 部署命令
        DeployCommand( pbdeploy->command(), pbdeploy->value(), pbdeploy->appname(), pbdeploy->apptype(), pbdeploy->appid(), pbdeploy->zoneid() );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFDeployClientModule::DeployCommand( const std::string& command, const std::string& value,
            const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid )
    {
        // 判断是不是自己
        auto ok = IsSelfServer( appname, apptype, appid, zoneid );
        if ( !ok )
        {
            return;
        }

        __LOG_INFO__( "[{}:{} | {}:{}:{}:{}] deploy command req", command, value, appname, apptype, appid, zoneid );

        if ( command == __STRING__( shutdown ) )
        {
            auto delaytime = __TO_UINT32__( value );
            return ShutDownServer( delaytime );
        }
        else if ( command == __STRING__( loglevel ) )
        {
            auto level = __TO_UINT32__( value );
            return KFLogger::Instance()->SetLogLevel( level );
        }
        else if ( command == __STRING__( logmemory ) )
        {
            auto memory = __TO_UINT32__( value );
            return KFMalloc::Instance()->SetLogOpen( memory == 1 ? true : false );
        }
        else if ( command == __STRING__( loadconfig ) )
        {
            return _kf_config->ReloadConfig( value );
        }
        else if ( command == __STRING__( loadplugin ) )
        {
            auto strcommand = __FORMAT__( "{}={}", command, value );
            return _kf_plugin_manage->AddCommand( strcommand );
        }
        else if ( command == __STRING__( messageclose ) )
        {
            UInt32List messagelist;
            auto strvalue = value;
            KFUtility::SplitList( messagelist, strvalue, __SPLIT_STRING__ );
            for ( auto messageid : messagelist )
            {
                _kf_message->OpenFunction( messageid, false );
            }
        }
        else if ( command == __STRING__( messageopen ) )
        {
            UInt32List messagelist;
            auto strvalue = value;
            KFUtility::SplitList( messagelist, strvalue, __SPLIT_STRING__ );
            for ( auto messageid : messagelist )
            {
                _kf_message->OpenFunction( messageid, true );
            }
        }

        auto kfcommand = _command_data.Find( command );
        if ( kfcommand != nullptr )
        {
            for ( auto& iter : kfcommand->_functions._objects )
            {
                auto kffunction = iter.second;
                kffunction->_function( value );
            }
        }
    }

    bool KFDeployClientModule::IsSelfServer( const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid )
    {
        auto kfglobal = KFGlobal::Instance();

        // 指定appid
        if ( appid != _globbing_string )
        {
            return ( appid == kfglobal->_app_id->ToString() );
        }

        // appname
        if ( appname != _globbing_string )
        {
            if ( appname != kfglobal->_app_name )
            {
                return false;
            }
        }

        // apptype
        if ( apptype != _globbing_string )
        {
            if ( apptype != kfglobal->_app_type )
            {
                return false;
            }
        }

        // zoneid
        if ( kfglobal->_app_name == __STRING__( zone ) )
        {
            if ( zoneid != _invalid_int )
            {
                if ( zoneid != kfglobal->_app_id->GetZoneId() )
                {
                    return false;
                }
            }
        }

        return true;
    }

    void KFDeployClientModule::ShutDownServer( uint32 delaytime )
    {
        auto kfglobal = KFGlobal::Instance();
        __LOG_INFO__( "[{}:{}:{}:{}] shutdown start", kfglobal->_app_name, kfglobal->_app_type, kfglobal->_app_id->ToString(), delaytime );
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        OnShutDownServerCommand( "0" );
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        delaytime = __MAX__( delaytime, 10000u );
        if ( kfglobal->_app_name != __STRING__( zone ) )
        {
            delaytime += 30000;
        }
        else
        {
            if ( kfglobal->_app_type == __STRING__( world ) )
            {
                delaytime += 20000;
            }
        }
        __LIMIT_TIMER_1__( kfglobal->_app_id->GetId(), delaytime, 1u, &KFDeployClientModule::OnTimerShutDownServer );
    }

    __KF_TIMER_FUNCTION__( KFDeployClientModule::OnTimerShutDownServer )
    {
        auto kfglobal = KFGlobal::Instance();
        __LOG_INFO__( "[{}:{}:{}] shutdown ok", kfglobal->_app_name, kfglobal->_app_type, kfglobal->_app_id->ToString() );

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        OnShutDownServerCommand( "1" );
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        // linux程序正常退出时会core, 应该是每个so文件中的同名全局变量造成的
        // 这里不让程序退出, 由agent等待超时kill
        // kfglobal->_app_run = false;
    }

    void KFDeployClientModule::OnShutDownServerCommand( const std::string& param )
    {
        auto kfcommand = _command_data.Find( __STRING__( shutdown ) );
        if ( kfcommand != nullptr )
        {
            for ( auto& iter : kfcommand->_functions._objects )
            {
                auto kffunction = iter.second;
                kffunction->_function( param );
            }
        }
    }
}