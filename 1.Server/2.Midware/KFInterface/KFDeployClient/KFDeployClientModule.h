﻿#ifndef __KF_DEPLOY_CLIENT_MODULE_H__
#define __KF_DEPLOY_CLIENT_MODULE_H__


/************************************************************************
//    @Module			:    部署Client
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-7-2
************************************************************************/

#include "KFrame.h"
#include "KFDeployClientInterface.h"
#include "KFTimer/KFTimerInterface.h"
#include "KFConfig/KFConfigInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFTcpClient/KFTcpClientInterface.h"

namespace KFrame
{
    class KFDeployClientModule : public KFDeployClientInterface
    {
    public:
        KFDeployClientModule() = default;
        ~KFDeployClientModule() = default;

        // 逻辑
        virtual void BeforeRun();
        virtual void OnceRun();

        // 关闭
        virtual void ShutDown();


    protected:
        virtual void AddCommandFunction( const std::string& command, const std::string& module, KFCommandFunction& function );
        virtual void RemoveComandFunction( const std::string& command, const std::string& module );

        // 判断是不是自己
        bool IsSelfServer( const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid );

        // 关闭服务器
        void ShutDownServer( const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid, uint32 delaytime );

        // 部署命令
        void DeployCommand( const std::string& command, const std::string& value, const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid );
    protected:
        // 准备关闭服务器
        __KF_TIMER_FUNCTION__( OnTimerShutDownPrepare );

        // 关闭服务器
        __KF_TIMER_FUNCTION__( OnTimerShutDownServer );

    protected:
        // 关闭服务器
        __KF_MESSAGE_FUNCTION__( HandleDeployCommandToClientReq );

    private:

        // 命令回调函数
        KFMap< std::string, const std::string&, KFCommand > _command_data;
    };
}

#endif