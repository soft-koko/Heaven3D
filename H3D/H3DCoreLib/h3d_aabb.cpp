#include "h3d_pch.h"
#include "h3d_vector3.h"
#include "h3d_matrix.h"
#include "h3d_aabb.h"
//   类
namespace Heaven3D
{
	H3D_AABB::H3D_AABB(): 
	min_pt_(0.f, 0.f, 0.f),max_pt_(0.f, 0.f, 0.f)
	{

	}

	H3DVector3 H3D_AABB::center()
	{
		return (min_pt_+max_pt_)*0.5f;
	}

	H3DVector3 H3D_AABB::extent()
	{
		return (max_pt_-min_pt_)*0.5f;
	}

	void H3D_AABB::xform(const H3DMatrix& m , H3D_AABB& out)
	{
		// Convert to center/extent representation.
		H3DVector3 c = center();
		H3DVector3 e = extent();

		// Transform center in usual way.
		//D3DXVec3TransformCoord(&c, &c, &M);
		c.TransformCoord(m);

		// Transform extent.
		H3DMatrix abs_mat;
		abs_mat.Identity();//D3DXMatrixIdentity(&abs_mat);
		abs_mat(0,0) = fabsf(m(0,0)); abs_mat(0,1) = fabsf(m(0,1)); abs_mat(0,2) = fabsf(m(0,2));
		abs_mat(1,0) = fabsf(m(1,0)); abs_mat(1,1) = fabsf(m(1,1)); abs_mat(1,2) = fabsf(m(1,2));
		abs_mat(2,0) = fabsf(m(2,0)); abs_mat(2,1) = fabsf(m(2,1)); abs_mat(2,2) = fabsf(m(2,2));
		e.TransformNormal( abs_mat );

		// Convert back to AABB representation.
		out.min_pt_ = c - e;
		out.max_pt_ = c + e;
	}

}