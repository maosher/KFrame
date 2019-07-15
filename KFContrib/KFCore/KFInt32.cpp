﻿#include "KFInt32.h"

namespace KFrame
{
    int32 KFInt32::GetInt32()
    {
        return _data;
    }

    int32 KFInt32::SetInt32( int32 value )
    {
        _data = value;
        if ( _data > ( int32 )_data_setting->_int_max_value )
        {
            _data = ( int32 )_data_setting->_int_max_value;
        }

        return _data;
    }

    bool KFInt32::IsFull()
    {
        return _data >= ( int32 )_data_setting->_int_max_value;
    }

    void KFInt32::Reset()
    {
        _data = _invalid_int;
    }

    bool KFInt32::IsValid()
    {
        return _data != _invalid_int;
    }

    void KFInt32::InitData()
    {
        if ( _data_setting->_int_init_value != 0 )
        {
            _data = _data_setting->_int_init_value;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    void KFInt32::CopyFrom( KFData* kfother )
    {
        SetValue( kfother->GetValue<int32>() );
    }

    void KFInt32::SaveTo( KFData* kfother )
    {
        kfother->SetValue( GetValue<int32>() );
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

    std::string KFInt32::ToString()
    {
        return KFUtility::ToString( _data );
    }

    void KFInt32::FromString( const std::string& value )
    {
        SetInt32( KFUtility::ToValue<int32>( value ) );
    }
}
