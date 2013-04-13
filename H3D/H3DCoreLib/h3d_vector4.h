/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   1:56
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_vector4.h
	author:		Xiong Xinke
	
	purpose:	四维向量类
*********************************************************************/
#ifndef h3d_vector4_h__
#define h3d_vector4_h__

#include "h3d_def.h"
#include "h3d_iosfwd.h"

namespace Heaven3D
{
	class H3DVector4 : public XMFLOAT4
	{
	public:
		H3DVector4();
		H3DVector4(float x , float y , float z , float w );
		H3DVector4(const H3DVector4& vec );
		const H3DVector4& operator = (const H3DVector4& vec );  

		H3D_INLINE H3DVector4( H3DIStream& ifs) 
		{
			ifs >> x >> y >> z;
		}
	};

	H3DOStream& operator << (H3DOStream& ofs,const H3DVector4& v);
	H3DIStream& operator >> (H3DIStream& ifs,H3DVector4& v);
}

#endif // h3d_vector4_h__