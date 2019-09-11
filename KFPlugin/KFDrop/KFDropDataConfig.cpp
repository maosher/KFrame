﻿#include "KFDropDataConfig.hpp"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////////
    void KFDropDataConfig::ReadSetting( KFNode& xmlnode, KFDropDataSetting* kfsetting )
    {
        auto id = kfsetting->_drop_data_list._weight_data.size();
        auto weight = xmlnode.GetUInt32( "Weight", true );
        auto kfdropdataweight = kfsetting->_drop_data_list.Create( ++id, weight );

        kfdropdataweight->_drop_data._data_name = xmlnode.GetString( "DropName" );
        kfdropdataweight->_drop_data._data_key = xmlnode.GetUInt32( "DropKey", true );

        auto strvalue = xmlnode.GetString( "DropValue" );
        kfdropdataweight->_drop_data._data_value = strvalue;
        kfdropdataweight->_drop_data._min_value = KFUtility::SplitValue<uint32>( strvalue, FUNCTION_RANGE_STRING );
        kfdropdataweight->_drop_data._max_value = KFUtility::SplitValue<uint32>( strvalue, FUNCTION_RANGE_STRING );
    }

    void KFDropDataConfig::FormatDropElement( KFDropData* dropdata )
    {
        // 掉落
        {
            auto kfelementsetting = KFElementConfig::Instance()->FindElementSetting( dropdata->_data_name );
            auto strelement = __FORMAT__( kfelementsetting->_element_template, dropdata->_data_name, dropdata->_data_value, dropdata->_data_key );
            dropdata->_elements.Parse( strelement, __FUNC_LINE__ );
        }
    }
}