#include "h3d_pch.h"
#include "h3d_file.h"
#include "h3d_file_output_channel.h"
//   类
namespace Heaven3D
{
	H3DFileOutputChannel::H3DFileOutputChannel(const TCHAR* file_name , uint32_t access_rights ):
	file_(file_name, access_rights)
	{
	}

	H3DFileOutputChannel::~H3DFileOutputChannel()
	{

	}

	H3DFile& H3DFileOutputChannel::file() 
	{
		return file_;
	}
	
	bool H3DFileOutputChannel::open() 
	{
		return file_.open();
	}

	void H3DFileOutputChannel::close() 
	{
		file_.close();
	}

	bool H3DFileOutputChannel::write(const TCHAR* text) 
	{
		file_.write(text, _tcslen(text) * sizeof(TCHAR));
		return true;
	}
}