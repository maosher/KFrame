﻿#ifndef __KF_LOGIN_MODULE_H__
#define __KF_LOGIN_MODULE_H__

/************************************************************************
//    @Module			:    登录服务器模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-12-4
************************************************************************/

#include "KFrame.h"
#include "KFLoginInterface.h"
#include "KFZone/KFZoneInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFIpAddress/KFIpAddressInterface.h"
#include "KFTcpServer/KFTcpServerInterface.h"
#include "KFTcpClient/KFTcpClientInterface.h"
#include "KFHttpClient/KFHttpClientInterface.h"
#include "KFDataClient/KFDataClientInterface.h"
#include "KFDeployCommand/KFDeployCommandInterface.h"

namespace KFrame
{
    class KFLoginModule : public KFLoginInterface
    {
    public:
        KFLoginModule();
        ~KFLoginModule();

        // 初始化
        virtual void InitModule();

        // 刷新
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 是否连接到World
        virtual bool IsWorldConnected();

        // 发送消息到世界服务器
        virtual bool SendMessageToWorld( uint32 msgid, ::google::protobuf::Message* message );

        // 发送消息到Gate服务器
        virtual bool SendMessageToGate( uint32 gateid, uint32 msgid, ::google::protobuf::Message* message );

    protected:
        // 连接成功
        __KF_CLIENT_CONNECT_FUNCTION__( OnClientConnectionWorld );

        // 断开连接
        __KF_CLIENT_LOST_FUNCTION__( OnClientLostWorld );

        // 关闭回调
        __KF_COMMAND_FUNCTION__( OnDeployShutDownServer );

    protected:

        // 登录验证请求
        __KF_MESSAGE_FUNCTION__( HandleLoginVerifyReq );

        // 登录验证结果
        __KF_MESSAGE_FUNCTION__( HandleLoginFailedAck );

    protected:
        // 平台验证回调
        void OnHttpAuthLoginVerifyCallBack( std::string& senddata, std::string& recvdata );

        // 发送登录验证结果消息
        void SendLoginVerifyMessage( uint32 result, uint32 gateid, uint32 sessionid, uint32 accountid );

    private:

        // 世界服务器id
        uint32 _world_server_id;

        // 关闭登录
        bool _is_login_close;
    };
}

#endif