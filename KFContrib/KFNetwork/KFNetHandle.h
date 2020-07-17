﻿#ifndef __NET_HANDLE_H__
#define __NET_HANDLE_H__

#include "KFDefine.h"
#include "KFNetConnector.h"

struct uv_tcp_s;
typedef struct uv_tcp_s uv_tcp_t;

namespace KFrame
{
    class KFNetServerEngine;
    class KFNetServerServices;
    class KFNetHandle : public KFNetConnector
    {
    public:
        KFNetHandle();
        virtual ~KFNetHandle();

        // 初始化
        void InitHandle( uint64 id, void* uvtcp, KFNetServerServices* netservices );

        // 关闭服务
        void CloseHandle();

        // 关闭连接
        virtual void CloseSession();

        // 断开事件
        virtual void OnDisconnect( int32 code, const char* function, uint32 line );

        // 初始化压缩加密数据
        void InitCompressEncrypt();
        //////////////////////////////////////////////////////////////////////////////////////////////
    protected:
        // 关闭回调
        static void OnShutCallBack( uv_handle_t* handle );
    public:
        // 状态
        bool _is_trustee;

        // 类型
        KFNetData _net_data;

        // 远程ip
        std::string _remote_ip;
    protected:
        // 网络服务
        uv_tcp_t* _uv_tcp;
    };
}


#endif