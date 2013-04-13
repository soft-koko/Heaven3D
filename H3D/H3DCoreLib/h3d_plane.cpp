#include "h3d_pch.h"
#include "h3d_vector3.h"
#include "h3d_plane.h"
//   类
namespace Heaven3D
{
	void H3DPlane::DotCoord(const H3DVector3& coord )
	{
		XMVECTOR v_coord = XMLoadFloat3(&coord);
		XMVECTOR v_this = XMLoadFloat4(this);
		v_this = XMPlaneDotCoord( v_this , v_coord );
		XMStoreFloat4(this,v_this);
	}
}