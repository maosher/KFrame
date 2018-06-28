﻿#include "KFBuffer.h"
#include "KFThread/KFThread.h"

namespace KFrame
{
	/////////////////////////////////////////////////////////////
	KFThreadBuffer::KFThreadBuffer()
	{
	}

	KFThreadBuffer::~KFThreadBuffer()
	{
	}

	int8* KFThreadBuffer::GetInt8( uint32 length, const char* function, uint32 line )
	{
		auto threadid = KFThread::GetThreadID();
		BuffKey key( threadid, length );
		
		auto kfbuffer = _kf_int8.Create( key );
		if ( kfbuffer->_buffer == nullptr )
		{
			kfbuffer->_length = length;
			kfbuffer->_buffer = new int8[ length ];

			KFLogger::LogMemory( KFLogger::Info, "[%s:%u] thread[%u] malloc buffer[%u]!",
				function, line, threadid, length );
		}

		return kfbuffer->_buffer;
	}

	uint8* KFThreadBuffer::GetUInt8( uint32 length, const char* function, uint32 line )
	{
		auto threadid = KFThread::GetThreadID();
		BuffKey key( threadid, length );

		auto kfbuffer = _kf_uint8.Create( key );
		if ( kfbuffer->_buffer == nullptr )
		{
			kfbuffer->_length = length;
			kfbuffer->_buffer = new uint8[ length ];

			KFLogger::LogMemory( KFLogger::Info, "[%s:%u] thread[%u] malloc buffer[%u]!",
				function, line, threadid, length );
		}

		return kfbuffer->_buffer;
	}
}