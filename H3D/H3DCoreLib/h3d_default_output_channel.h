/********************************************************************
	created:	2012/02/18
	created:	18:2:2012   22:37
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_default_output_channel.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_default_output_channel_h__
#define h3d_default_output_channel_h__
//   类
#include "h3d_debug_channel.h"

namespace Heaven3D
{
	class H3DDefaultOutputChannel : public H3DDebugMessageChannel
	{
	public:
		H3DDefaultOutputChannel();
		~H3DDefaultOutputChannel();
	private:
		bool write(const TCHAR* text);
	};
}

#endif // h3d_default_output_channel_h__
