/********************************************************************
created:	2012/02/18
created:	18:2:2012   0:45
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_string.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_string_h__
#define h3d_string_h__
//   类
#include "h3d_def.h"

namespace Heaven3D
{

	/*	H3DString
	------------------------------------------------------------------------------------------

	A H3DString (aka "H3DString String") is a wrapper around the STL string template class. H3DString
	provides transparent access to string functions with some added functionality.

	------------------------------------------------------------------------------------------
	*/

	inline void format_text(TCHAR* buffer, const TCHAR* text, va_list ArgList)
	{
#ifdef _UNICODE
		vswprintf(buffer, text, ArgList);
#else
		vsprintf(buffer, text, ArgList);
#endif
	}

	inline void __cdecl format_text(TCHAR* buffer, const TCHAR* text, ...)
	{
		va_list		ArgList;
		va_start(ArgList, text);
		format_text(buffer, text, ArgList);
		va_end(ArgList);	
	}


	typedef std::basic_string<TCHAR,std::char_traits<TCHAR> > TEXT_BASE_CLASS;

	class H3DString: public TEXT_BASE_CLASS
	{
	public:

		// Creators...
		inline H3DString(){};
		H3DString(const H3DString& rhs);
		explicit H3DString(const TEXT_BASE_CLASS& rhs);
		explicit H3DString(TCHAR c);
		H3DString(const TCHAR *s, ...);
		~H3DString(){};

		// cast the string directly to a TCHAR pointer
		inline operator const TCHAR*()const;

		// copy data from various source types
		H3DString& operator=(const H3DString& rhs);
		H3DString& operator=(const TEXT_BASE_CLASS& rhs);
		H3DString& operator=(const TCHAR *s);
		H3DString& operator=(TCHAR *s);
		H3DString& operator=(TCHAR c);

		// append data from various sources
		H3DString& operator+=(const H3DString& rhs);
		H3DString& operator+=(const TEXT_BASE_CLASS& rhs);
		H3DString& operator+=(const TCHAR *s); 
		H3DString& operator+=(TCHAR c);

		// test equality and inequality (case sensitive)
		bool operator==(const H3DString& rhs);
		bool operator==(const TCHAR* rhs);	
		bool operator!=(const H3DString& rhs);

		// test without case sensitivity
		bool compare_without_case(const H3DString& rhs);

		// operator that allows H3DString to be used as an STL map key value
		bool operator<(const H3DString& rhs);

		// format the string from optional parameters
		void __cdecl format(const TCHAR* text, ...);
		void __cdecl format(const TCHAR* text, va_list arg_list);

		void remove_file_extension(H3DString* extension_found=0);

		H3D_INLINE TCHAR GetChar(difference_type offset)
		{
			return (*(*this + offset));
		}
	};

	//- Inline Functions ---------------------------------------------------------------------

	//. Creators .............................................................................

	inline H3DString::H3DString(const H3DString& rhs)
	{
		assign((const TEXT_BASE_CLASS) rhs); 
	}

	inline H3DString::H3DString(const TEXT_BASE_CLASS& rhs)
	{
		assign((const TEXT_BASE_CLASS) rhs); 
	}

	inline H3DString::H3DString(const TCHAR *s, ...)
	{
		va_list		arg_list;
		va_start(arg_list, s);

		if(*arg_list != 0)
		{
			format(s, arg_list);
		}
		else
		{
			assign(s);
		}

		va_end(arg_list);	
	}

	inline H3DString::H3DString(TCHAR c)
	{
		assign(1,c); 
	}

	//. Operators ............................................................................
	inline bool H3DString::operator==(const H3DString& rhs)
	{
		return (compare(rhs) == 0);
	}

	inline bool H3DString::operator==(const TCHAR* rhs)
	{
		return (compare(rhs) == 0);
	}

	inline bool H3DString::operator!=(const H3DString& rhs)
	{
		return (compare(rhs) != 0);
	}

	inline H3DString& H3DString::operator=(const H3DString& rhs) 
	{
		assign((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	inline H3DString& H3DString::operator=(const TEXT_BASE_CLASS& rhs) 
	{
		assign((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	inline H3DString& H3DString::operator=(const TCHAR *s)
	{
		assign(s); 
		return (*this);
	}

	inline H3DString& H3DString::operator=(TCHAR *s)
	{
		assign(s); 
		return (*this);
	}

	inline H3DString& H3DString::operator=(TCHAR c)
	{
		assign(1,c); 
		return (*this);
	}

	inline H3DString& H3DString::operator+=(const H3DString& rhs)
	{
		append((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	inline H3DString& H3DString::operator+=(const TEXT_BASE_CLASS& rhs)
	{
		append((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	inline H3DString& H3DString::operator+=(const TCHAR *s)
	{
		append(s); 
		return (*this);
	}

	inline H3DString& H3DString::operator+=(TCHAR c)
	{
		append(1,c); 
		return (*this);
	}


	//. Helper Functions .....................................................................


	inline H3DString operator+(const H3DString& lhs, const H3DString& rhs)
	{
		H3DString new_text(lhs);
		new_text+=rhs;
		return(new_text);
	}

	inline H3DString::operator const TCHAR*()const
	{
		return (c_str());
	}

	inline bool H3DString::compare_without_case(const H3DString& rhs)
	{
		return(_tcsicmp(c_str(), rhs.c_str()) == 0);
	}

	inline bool H3DString::operator<(const H3DString& rhs)
	{
		return(_tcscmp(c_str(), rhs.c_str()) < 0);
	};

	//***************************************************************

} 

#endif // h3d_string_h__