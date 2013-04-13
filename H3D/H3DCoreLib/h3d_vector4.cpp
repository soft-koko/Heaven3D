#include "h3d_pch.h"
#include "h3d_vector4.h"
//   类
namespace Heaven3D
{
	H3DVector4::H3DVector4()
	{
		this->x = this->y = 
		this->z = this->w = 0;
	}

	H3DVector4::H3DVector4(float x , float y , float z , float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	H3DVector4::H3DVector4(const H3DVector4& vec )
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = vec.w;
	}

	const H3DVector4& H3DVector4::operator = (const H3DVector4& vec )
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = vec.w;
		return *this;
	}

	H3DOStream& operator << (H3DOStream& ofs,const H3DVector4& v) 
	{
		return ofs << _T("v ") << v.x <<_T(" ") << v.y <<_T(" ")<< v.z <<_T(" ")<< v.w;
	}

	H3DIStream& operator >> (H3DIStream& ifs,H3DVector4& v)
	{
		return ifs >> v.x >> v.y >> v.z >> v.w;
	}
}