﻿#ifndef __KF_MYSQL_MODULE_H__
#define __KF_MYSQL_MODULE_H__

/************************************************************************
//    @Module			:    mysql数据库
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-5-10
************************************************************************/

#include "KFrame.h"
#include "KFMySQLInterface.h"
#include "KFMySQLLogic.hpp"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    class KFMySQLModule : public KFMySQLInterface
    {
    public:
        KFMySQLModule() = default;
        ~KFMySQLModule() = default;

        // 加载配置
        virtual void InitModule();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 创建Execute
        virtual KFMySQLDriver* Create( const std::string& module, uint32 logicid = 0 );

    protected:
        // 查询
        KFMySQLLogic* FindMySQLLogic( uint32 id );

        // 插入
        void InsertMySQLLogic( uint32 id, KFMySQLLogic* kflogic );

    private:
        // 互斥量
        KFMutex _kf_mutex;

        // 数据库列表
        typedef std::pair< uint32, uint32 > MySQLKey;
        KFMap< MySQLKey, const MySQLKey&, KFMySQLLogic > _mysql_logic_map;
    };
}



#endif