/********************************************************************
	created:	2012/02/18
	created:	18:2:2012   22:33
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_file_output_channel.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_file_output_channel_h__
#define h3d_file_output_channel_h__

#include "h3d_debug_channel.h"
//   类
namespace Heaven3D
{
	class H3DFileOutputChannel : public H3DDebugMessageChannel
	{
	public:
		H3DFileOutputChannel(const TCHAR* file_name=0, uint32_t access_rights=H3DFile::READ_WRITE);
		~H3DFileOutputChannel();
		H3DFile& file();
	private:
		bool open();
		void close();
		bool write(const TCHAR* text);
	private:
		H3DFile file_;
	};
}

#endif // h3d_file_output_channel_h__