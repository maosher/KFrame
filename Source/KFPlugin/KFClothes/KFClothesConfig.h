﻿#ifndef __KF_CLOTHES_CONFIG_H__
#define __KF_CLOTHES_CONFIG_H__

#include "KFClothesSetting.h"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	class KFClothesConfig : public KFConfig, public KFSingleton< KFClothesConfig >
	{
	public:
		KFClothesConfig();
		~KFClothesConfig();

		// 查找配置
		const KFClothesSetting* FindClothesSetting( uint32 id ) const;

		// 读取配置
		bool LoadConfig( const char* file );

	protected:
		// 添加配置
		void AddClothesSetting( KFClothesSetting* setting );

	private:

		// 成就列表
		KFMap< uint32, uint32, KFClothesSetting > _clothes_setting;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static auto _kf_clothes_config = KFClothesConfig::Instance();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif