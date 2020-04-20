﻿#ifndef __KF_BASIC_ATTRIBUTE_MODULE_H__
#define __KF_BASIC_ATTRIBUTE_MODULE_H__

/************************************************************************
//    @Module			:    公共数据模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-11-30
************************************************************************/

#include "KFBasicAttributeInterface.h"
#include "KFBasicAttributeRedis.hpp"
#include "KFBasicAttributeMongo.hpp"

namespace KFrame
{
    class KFBasicAttributeModule : public KFBasicAttributeInterface
    {
    public:
        KFBasicAttributeModule() = default;
        ~KFBasicAttributeModule() = default;

        // 初始化
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 更新属性
        virtual void UpdateBasicIntValue( uint64 playerid, uint64 serverid, const KeyValue& values );
        virtual void UpdateBasicStrValue( uint64 playerid, uint64 serverid, const StringMap& values );

        // 查询玩家id
        virtual uint64 QueryBasicPlayerid( const std::string& playername, uint32 zoneid );

        // 清除玩家的在线服务器id
        virtual void ClearBasicServerId( uint64 serverid );

        // 查询玩家的服务器id
        virtual uint64 QueryBasicServerId( uint64 playerid );

        // 查询玩家基础信息
        virtual uint32 QueryBasicAttribute( uint64 playerid, StringMap& values );
        virtual uint32 QueryBasicAttribute( const std::string& playername, uint32 zoneid, StringMap& values );

        // 设置玩家名字
        virtual uint32 SetPlayerName( uint32 zoneid, uint64 playerid, const std::string& oldname, const std::string& newname );

    private:
        // 属性逻辑
        KFBasicAttributeLogic* _basic_attribute_logic = nullptr;
    };
}



#endif