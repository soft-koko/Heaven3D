/********************************************************************
	created:	2012/04/02
	created:	2:4:2012   22:45
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_iosfwd.h
	author:	Xiong Xinke
	
	purpose:	定义ansi/unicode通用的ios前置声明的头文件
*********************************************************************/
#ifndef h3d_iosfwd_h__
#define h3d_iosfwd_h__
//   类
namespace Heaven3D
{
	typedef basic_ios<TCHAR, char_traits<TCHAR> > H3DIOS;
	typedef basic_streambuf<TCHAR, char_traits<TCHAR> > H3DStreamBuf;
	typedef basic_istream<TCHAR, char_traits<TCHAR> > H3DIStream;
	typedef basic_ostream<TCHAR, char_traits<TCHAR> > H3DOStream;
	typedef basic_iostream<TCHAR, char_traits<TCHAR> > H3DIOStream;
	typedef basic_stringbuf<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > H3DStringBuf;
	typedef basic_istringstream<TCHAR, char_traits<TCHAR>,allocator<TCHAR> > H3DIStringStream;
	typedef basic_ostringstream<TCHAR, char_traits<TCHAR>,allocator<TCHAR> > H3DOStringStream;
	typedef basic_stringstream<TCHAR, char_traits<TCHAR>,allocator<TCHAR> > H3DStringStream;
	typedef basic_filebuf<TCHAR, char_traits<TCHAR> > H3DFileBuf;
	typedef basic_ifstream<TCHAR, char_traits<TCHAR> > H3DIFStream;
	typedef basic_ofstream<TCHAR, char_traits<TCHAR> > H3DOFStream;
	typedef basic_fstream<TCHAR, char_traits<TCHAR> > H3DFStream;
}

#endif // h3d_iosfwd_h__