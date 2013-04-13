/********************************************************************
	created:	2012/03/01
	created:	1:3:2012   0:39
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_aabb.h
	author:		Xiong Xinke
	
	purpose:	轴对齐包围盒类 AABB
*********************************************************************/
#ifndef h3d_aabb_h__
#define h3d_aabb_h__

class H3DVector3;
class H3DMatrix;

namespace Heaven3D
{
	class H3D_AABB 
	{
	public:
		H3D_AABB();
		H3DVector3 center();
		H3DVector3 extent();
		void xform(const H3DMatrix& M, H3D_AABB& out);
		H3DVector3 min_pt_;
		H3DVector3 max_pt_;
	};


}
#endif // h3d_aabb_h__
