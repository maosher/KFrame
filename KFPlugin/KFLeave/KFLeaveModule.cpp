﻿#include "KFLeaveModule.hpp"

namespace KFrame
{
    void KFLeaveModule::BeforeRun()
    {
        __REGISTER_LEAVE_PLAYER__( &KFLeaveModule::LeaveGameWorld );
    }

    void KFLeaveModule::BeforeShut()
    {
        __UN_LEAVE_PLAYER__();
    }

    __KF_LEAVE_PLAYER_FUNCTION__( KFLeaveModule::LeaveGameWorld )
    {
        auto kfobject = player->GetData();
        //////////////////////////////////////////////////////////////////////////
        auto playerid = player->GetKeyID();
        auto kfnoteparent = kfobject->FindData( __KF_STRING__( note ) );
        for ( auto& iter : KFLeaveConfig::Instance()->_settings._objects )
        {
            auto kfsetting = iter.second;
            if ( kfsetting->_id != _invalid_int )
            {
                auto notevalue = kfnoteparent->GetValue( kfsetting->_id, __KF_STRING__( value ) );
                if ( notevalue != _invalid_int )
                {
                    continue;
                }

                // 设置属性
                player->UpdateData( kfnoteparent, kfsetting->_id, __KF_STRING__( value ), KFEnum::Set, 1 );
            }

            // 调用脚本
            _kf_lua->Call( kfsetting->_lua_file, kfsetting->_lua_function, playerid );
        }
    }
}