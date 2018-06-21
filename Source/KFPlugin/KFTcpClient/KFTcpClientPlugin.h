﻿#ifndef __KF_CLIENT_PLUGIN_H__
#define __KF_CLIENT_PLUGIN_H__

#include "KFPlugin/KFPlugin.h"

namespace KFrame
{
	class KFTcpClientPlugin : public KFPlugin
	{
	public:
		// 注册
		virtual void Install();

		// 卸载
		virtual void UnInstall();

		// 加载
		virtual void LoadModule();
	};
}


#endif