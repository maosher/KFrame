﻿#ifndef __KF_ROBOT_CONFIG_H__
#define __KF_ROBOT_CONFIG_H__

#include "KFZConfig/KFConfig.h"

namespace KFrame
{
    class KFRobotConfig : public KFConfig, public KFInstance< KFRobotConfig >
    {
    public:
        KFRobotConfig()
        {
            _file_name = "robot";
        }

        // 读取配置
        bool LoadConfig( const std::string& filename, const std::string& filepath, uint32 loadmask );

    public:
        // 认证服务器地址
        std::string _auth_address;

        // 加密等级
        uint32 _compress_level = 0u;

        // 机器人账号名
        std::string _robot_account;

        // 机器人总数
        uint32 _total_robot_count = 1u;

        // 登录的间隔时间
        uint32 _login_interval_time = 1000;

        // 开启属性更新log
        bool _open_data_log = false;
    };
}

#endif