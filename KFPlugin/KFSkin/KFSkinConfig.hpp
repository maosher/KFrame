﻿#ifndef __KF_SKIN_CONFIG_H__
#define __KF_SKIN_CONFIG_H__

#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    ////////////////////////////////////////////////////////////////////////////////
    class KFSkinSetting : public KFIntSetting
    {
    public:
        // 品质
        uint32 _quality = 0u;
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    class KFSkinConfig : public KFIntConfigT< KFSkinSetting >, public KFSingleton< KFSkinConfig >
    {
    public:
        KFSkinConfig( const std::string& file, bool isclear )
            : KFIntConfigT< KFSkinSetting >( file, isclear )
        {
        }
    protected:
        // 读取配置
        void ReadSetting( KFNode& xmlnode, KFSkinSetting* kfsetting );
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static auto _kf_skin_config = KFSkinConfig::Instance( "skin.xml", true );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif