#define CSTRING_CPP
#define CORE_DLL

#include "h3d_pch.h"
#include "h3d_string.h"
//   类
namespace Heaven3D
{
	void __cdecl H3DString::format(const TCHAR* text, ...)
	{
		va_list arg_list;
		va_start(arg_list, text);
		format(text, arg_list);
		va_end(arg_list);	
	}

	void __cdecl H3DString::format(const TCHAR* text, va_list arg_list)
	{
		const int TEXT_BUFFERSIZE = 2048;
		TCHAR text_buffer[TEXT_BUFFERSIZE];
		format_text(text_buffer, text, arg_list);
		(*this) = text_buffer; 
	}

	void H3DString::remove_file_extension(H3DString* extension_found)
	{
		size_t length = size();
		size_t extension_start = find_last_of('.');

		if( extension_start >=0 && extension_start<length)
		{
			if(extension_found)
			{
				*extension_found = substr(extension_start+1, length-extension_start-1);
			}

			*this = substr(0, extension_start);
		}
	}
}