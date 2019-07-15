﻿#include "KFDouble.h"

namespace KFrame
{
    double KFDouble::GetDouble()
    {
        return _data;
    }

    double KFDouble::SetDouble( double value )
    {
        _data = value;
        return _data;
    }

    void KFDouble::Reset()
    {
        _data = 0.0f;
    }

    bool KFDouble::IsValid()
    {
        return _data != 0.0f;
    }

    void KFDouble::InitData()
    {
        if ( _data_setting->_int_init_value != 0.0f )
        {
            _data = _data_setting->_int_init_value;
        }
    }

    void KFDouble::CopyFrom( KFData* kfother )
    {
        SetValue( kfother->GetValue<double>() );
    }

    void KFDouble::SaveTo( KFData* kfother )
    {
        kfother->SetValue( GetValue<double>() );
    }

    std::string KFDouble::ToString()
    {
        return KFUtility::ToString( _data );
    }

    void KFDouble::FromString( const std::string& value )
    {
        _data = KFUtility::ToValue<double>( value );
    }
}