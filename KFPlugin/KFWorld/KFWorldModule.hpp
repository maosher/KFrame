﻿#ifndef __KF_WORLD_MODULE_H__
#define __KF_WORLD_MODULE_H__

/************************************************************************
//    @Module			:    世界功能模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-11-1
************************************************************************/

#include "KFWorldInterface.h"
#include "KFProtocol/KFProtocol.h"
#include "KFTimer/KFTimerInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFIpAddress/KFIpAddressInterface.h"
#include "KFTcpServer/KFTcpServerInterface.h"
#include "KFTcpClient/KFTcpClientInterface.h"
#include "KFHttpServer/KFHttpServerInterface.h"
#include "KFHttpClient/KFHttpClientInterface.h"
#include "KFDirClient/KFDirClientInterface.h"

namespace KFrame
{
    class KFWorldModule : public KFWorldInterface
    {
    public:
        KFWorldModule() = default;
        ~KFWorldModule() = default;

        // 刷新
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
    protected:
        // 断开Game
        __KF_NET_EVENT_FUNCTION__( OnServerLostGame );

        // 处理踢人
        __KF_HTTP_FUNCTION__( HandleHttpKickOnline );

        // 注册小区信息
        __KF_TIMER_FUNCTION__( OnTimerZoneRegister );
    protected:
        // 添加Gate信息
        __KF_MESSAGE_FUNCTION__( HandleAddGateToWorldReq );

        // 删除gate信息
        __KF_MESSAGE_FUNCTION__( HandleRemoveGateToWorldReq );

        // 验证登录
        __KF_MESSAGE_FUNCTION__( HandleLoginToWorldReq );

        // 玩家进入游戏
        __KF_MESSAGE_FUNCTION__( HandlePlayerEnterToWorldReq );

        // 玩家离开游戏
        __KF_MESSAGE_FUNCTION__( HandlePlayerLeaveToWorldReq );

        // 广播消息
        __KF_MESSAGE_FUNCTION__( HandleBroadcastToGameReq );
        __KF_MESSAGE_FUNCTION__( HandleBroadcastToWorldReq );
        __KF_MESSAGE_FUNCTION__( HandleBroadcastToWorldAck );

    protected:
        // 发送验证结果消息
        void SendLoginAckToLogin( uint32 result, uint64 loginid, uint64 gateid, uint64 accountid, uint64 sessionid );

        // 踢掉在线玩家
        void KickOnlineToGame( uint32 type, uint64 playerid, uint64 gameid, const char* function, uint32 line );

        // 查找一个可以用的game
        uint64 FindLoginGame( uint64 gateid, uint64 playerid );

        // 广播消息
        void BroadCastToGame( uint32 msgid, const std::string& msgdata, uint32 serial, uint64 worldid );

        // 更新玩家在线信息
        void UpdatePlayerOnline( uint64 accountid, uint64 playerid, uint64 worldid, uint64 gameid );

    private:
        // 游戏服务列表
        KFHashMap< uint64, uint64, KFConHash > _gate_conhash;

        // 广播的序号
        uint32 _broadcast_serial = 0u;
    };
}

#endif