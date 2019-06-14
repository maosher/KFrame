﻿#include "KFAchieveConfig.hpp"

namespace KFrame
{
    bool KFAchieveConfig::LoadConfig( const std::string& file )
    {
        _achieve_types.clear();
        _achieve_setting.Clear();
        //////////////////////////////////////////////////////////////////
        KFXml kfxml( file );
        auto config = kfxml.RootNode();
        auto xmlnode = config.FindNode( "item" );
        while ( xmlnode.IsValid() )
        {
            auto id = xmlnode.GetUInt32( "Id" );
            auto kfsetting = _achieve_setting.Create( id );

            kfsetting->_id = id;
            kfsetting->_parent_name = xmlnode.GetString( "ParentName" );
            kfsetting->_data_name = xmlnode.GetString( "DataName" );
            kfsetting->_data_key = xmlnode.GetUInt32( "DataKey" );

            kfsetting->_trigger_type = xmlnode.GetUInt32( "TriggerType" );
            kfsetting->_trigger_value = xmlnode.GetUInt32( "TriggerValue" );

            kfsetting->_operate = xmlnode.GetUInt32( "Operate" );
            kfsetting->_use_type = xmlnode.GetUInt32( "UseType" );
            kfsetting->_use_value = xmlnode.GetUInt32( "UseValue" );

            kfsetting->_done_value = xmlnode.GetUInt32( "DoneValue" );
            kfsetting->_done_type = xmlnode.GetUInt32( "DoneType" );

            auto strlimit = xmlnode.GetString( "Limits" );
            if ( !strlimit.empty() )
            {
                kfsetting->_limits.Parse( strlimit, __FUNC_LINE__ );
            }

            auto strrewards = xmlnode.GetString( "Rewards" );
            if ( !strrewards.empty() )
            {
                kfsetting->_rewards.Parse( strrewards, __FUNC_LINE__ );
            }

            AddAchieveType( kfsetting );
            xmlnode.NextNode();
        }
        //////////////////////////////////////////////////////////////////

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void KFAchieveTypes::AddAchieveType( KFAchieveSetting* setting )
    {
        auto iter = _achieve_type.find( setting->_data_name );
        if ( iter == _achieve_type.end() )
        {
            iter = _achieve_type.insert( std::make_pair( setting->_data_name, KFAchieveType() ) ).first;
        }

        iter->second.AddKFAchieveType( setting );
    }

    const KFAchieveType* KFAchieveTypes::FindAchieveType( const std::string& dataname ) const
    {
        auto iter = _achieve_type.find( dataname );
        if ( iter == _achieve_type.end() )
        {
            return nullptr;
        }

        return &iter->second;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    const KFAchieveSetting* KFAchieveConfig::FindAchieveSetting( uint32 id ) const
    {
        return _achieve_setting.Find( id );
    }

    void KFAchieveConfig::AddAchieveType( KFAchieveSetting* kfsetting )
    {
        if ( kfsetting->_parent_name.empty() || kfsetting->_data_name.empty() )
        {
            return;
        }

        auto iter = _achieve_types.find( kfsetting->_parent_name );
        if ( iter == _achieve_types.end() )
        {
            iter = _achieve_types.insert( std::make_pair( kfsetting->_parent_name, KFAchieveTypes() ) ).first;
        }

        iter->second.AddAchieveType( kfsetting );
    }

    const KFAchieveType* KFAchieveConfig::FindTypeAchieve( const std::string& parentname, const std::string& dataname ) const
    {
        auto iter = _achieve_types.find( parentname );
        if ( iter == _achieve_types.end() )
        {
            return nullptr;
        }

        return iter->second.FindAchieveType( dataname );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFAchieveSetting::CheckCanUpdate( uint64 key, uint64 operate ) const
    {
        if ( _data_key != 0u && _data_key != key )
        {
            return false;
        }

        if ( _trigger_type == 0u || _trigger_type == operate )
        {
            return true;
        }

        return false;
    }

    uint64 KFAchieveSetting::CheckTriggerValue( uint64 operatevalue, uint64 nowvalue ) const
    {
        // 触发值
        if ( _use_type == KFEnum::UseFinal )
        {
            operatevalue = nowvalue;
        }

        if ( operatevalue < _trigger_value )
        {
            return 0u;
        }

        // 最终使用值
        if ( _use_value != 0u )
        {
            operatevalue = _use_value;
        }

        return operatevalue;
    }
}