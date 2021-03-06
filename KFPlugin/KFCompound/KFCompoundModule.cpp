﻿#include "KFCompoundModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    void KFCompoundModule::BeforeRun()
    {
        __REGISTER_MESSAGE__( KFMsg::MSG_COMPOUND_REQ, &KFCompoundModule::HandleCompoundReq );
    }

    void KFCompoundModule::ShutDown()
    {
        ///////////////////////////////////////////////////////////////////////////////
        __UN_MESSAGE__( KFMsg::MSG_COMPOUND_REQ );
    }

    __KF_MESSAGE_FUNCTION__( KFCompoundModule::HandleCompoundReq )
    {
        __CLIENT_PROTO_PARSE__( KFMsg::MsgCompoundReq );

        // 查找合成信息
        auto kfsetting = KFCompoundConfig::Instance()->FindSetting( kfmsg.id() );
        if ( kfsetting == nullptr )
        {
            return _kf_display->SendToClient( player, KFMsg::CompoundNotExist );
        }

        // 扣除材料
        auto& dataname = player->RemoveElement( &kfsetting->_cost_data, _default_multiple, __STRING__( compound ), kfsetting->_id, __FUNC_LINE__ );
        if ( !dataname.empty() )
        {
            return _kf_display->SendToClient( player, KFMsg::DataNotEnough, dataname );
        }

        // 添加合成的属性
        player->AddElement( &kfsetting->_compound_data, _default_multiple, __STRING__( compound ), kfsetting->_id, __FUNC_LINE__ );
        _kf_display->SendToClient( player, KFMsg::CompoundOk );
    }
}