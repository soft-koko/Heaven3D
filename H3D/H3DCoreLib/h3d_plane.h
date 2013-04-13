/********************************************************************
	created:	2012/03/01
	created:	1:3:2012   1:32
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_plane.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_plane_h__
#define h3d_plane_h__
//   类
class H3DVector3;

namespace Heaven3D
{
	class H3DPlane : public XMFLOAT4
	{
	public:
		void DotCoord(const H3DVector3& coord );
	};
}

#endif // h3d_plane_h__