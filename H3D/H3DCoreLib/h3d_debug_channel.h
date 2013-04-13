/********************************************************************
created:	2012/02/18
created:	18:2:2012   0:43
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_debug_channel.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_debug_channel_h__
#define h3d_debug_channel_h__
//   类
#include "h3d_bit_flags.h"

namespace Heaven3D
{
	class H3DFile;
	class H3DDebugMessageHandler;

	class H3DDebugMessageChannel
	{
	public:
		u32Flags messageFilter;
		H3DDebugMessageChannel():messageFilter(0){}
		virtual ~H3DDebugMessageChannel(){}
	private:
		friend H3DDebugMessageHandler;
		virtual bool open(){return true;}
		virtual void close(){}
		virtual bool write(const TCHAR* text) = 0;
	};
}

#endif // h3d_debug_channel_h__