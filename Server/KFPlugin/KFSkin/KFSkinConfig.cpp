﻿#include "KFSkinConfig.h"

namespace KFrame
{
    const KFSkinSetting* KFSkinConfig::FindSkinSetting( uint32 id ) const
    {
        return _skin_setting.Find( id );
    }

    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    bool KFSkinConfig::LoadConfig()
    {
        //////////////////////////////////////////////////////////////////
        KFXml kfxml( _file );
        auto config = kfxml.RootNode();
        auto xmlnode = config.FindNode( "Setting" );
        while ( xmlnode.IsValid() )
        {
            auto id = xmlnode.GetUInt32( "Id" );
            auto kfsetting = _skin_setting.Create( id );

            kfsetting->_id = id;
            kfsetting->_quality = xmlnode.GetUInt32( "Quality" );

            xmlnode.NextNode();
        }
        //////////////////////////////////////////////////////////////////

        return true;
    }
}