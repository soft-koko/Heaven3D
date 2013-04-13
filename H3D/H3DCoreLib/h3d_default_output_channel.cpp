#include "h3d_pch.h"
#include "h3d_default_output_channel.h"
//   类
namespace Heaven3D
{
	H3DDefaultOutputChannel::H3DDefaultOutputChannel()
	{
		messageFilter = 0xffffffff;
	}

	H3DDefaultOutputChannel::~H3DDefaultOutputChannel()
	{

	}

	bool H3DDefaultOutputChannel::write(const TCHAR* text)
	{
		OutputDebugString(text);
		return true;
	}
}