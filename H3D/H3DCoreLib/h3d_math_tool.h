/********************************************************************
created:	2012/02/19
created:	19:2:2012   0:24
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_math_tool.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_math_tool_h__
#define h3d_math_tool_h__

#include "h3d_numeric_tools.h"
//   类
namespace Heaven3D
{
	template<class T>
	T square( T number )
	{
		return number * number;
	}

	template <class T> inline T arctangent_of_ratio(T Numerator, T Denominator)
	{
		return fast_cast<T>(atan2f(fast_cast<float>(Numerator), fast_cast<float>(Denominator))); 
	}

	template <> inline int32_t arctangent_of_ratio(int32_t Numerator, int32_t Denominator)
	{
		return realToInt32_chop(atan2f(fast_cast<float>(Numerator), fast_cast<float>(Denominator))); 
	}

}


#endif // h3d_math_tool_h__