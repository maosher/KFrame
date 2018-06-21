﻿#ifndef __KF_INTERFACE_H__
#define __KF_INTERFACE_H__

#include "KFInclude.h"

namespace KFrame
{
	class KFGlobal;
	class KFPluginManage;
	class KFInterface
	{
	public:
		KFInterface()
		{
			_kf_plugin_manage = nullptr;
		}

		virtual ~KFInterface() {};

		// 释放
		virtual void Release() {};

		// 初始化
		virtual void InitModule() = 0;

		// 加载配置
		virtual void LoadConfig() = 0;
		virtual void AfterLoad() = 0;

		// 开始初始化
		virtual void BeforeRun() = 0;

		// 执行一次
		virtual void OnceRun() = 0;

		// 关闭
		virtual void BeforeShut() = 0;
		virtual void ShutDown() = 0;
		virtual void AfterShut() = 0;

	public:
		// 名字
		std::string _class_name;

		// 插件名字
		std::string _plugin_name;

		// 插件管理
		KFPluginManage* _kf_plugin_manage;
	};
}

#endif