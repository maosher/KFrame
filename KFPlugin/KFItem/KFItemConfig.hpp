﻿#ifndef __KF_ITEM_CONFIG_H__
#define __KF_ITEM_CONFIG_H__

#include "KFItemSetting.hpp"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    class KFItemConfig : public KFIntConfigT< KFItemSetting >, public KFSingleton< KFItemConfig >
    {
    public:
        KFItemConfig( const std::string& file, bool isclear )
            : KFIntConfigT< KFItemSetting >( file, isclear )
        {
        }
    protected:
        // 读取配置
        void ReadSetting( KFNode& xmlnode, KFItemSetting* kfsetting );
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static auto _kf_item_config = KFItemConfig::Instance( "item.xml", true );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif