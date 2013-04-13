/********************************************************************
created:	2012/02/18
created:	18:2:2012   0:43
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_stack_trace.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_stack_trace_h__
#define h3d_stack_trace_h__
//   类
namespace Heaven3D
{/*	Stack Tracing
 ------------------------------------------------------------------------------------------

 The stack tracing code is based on an article by John Robbins
 that appeared in Microsoft Systems Journal Bugslayer Column - Feb 99

 The article can be found at:
 http://www.microsoft.com/msj/defaultframe.asp?page=/msj/0299/bugslayer/bugslayer0299.htm

 ------------------------------------------------------------------------------------------
 */

	void BuildStackTrace( TCHAR* _string,uint32_t  _size, uint32_t  num_skip  );
}

#endif // h3d_stack_trace_h__