﻿#include "KFTeamClientModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    void KFTeamClientModule::BeforeRun()
    {
        _kf_component = _kf_kernel->FindComponent( __STRING__( player ) );
        ///////////////////////////////////////////////////////////////////////////////////////////
    }

    void KFTeamClientModule::BeforeShut()
    {

    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////

}
