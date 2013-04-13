#include "h3d_pch.h"
#include "h3d_matrix.h"
#include "h3d_quaternion.h"
#include "h3d_vector3.h"
//   类
namespace Heaven3D
{
	H3DQuaternion::H3DQuaternion(float x,float y, float z , float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	H3DQuaternion::H3DQuaternion(void)
	{

	}

	H3DQuaternion::~H3DQuaternion(void)
	{

	}

	void H3DQuaternion::Identity()
	{
		XMVECTOR this_q = XMQuaternionIdentity();
		XMStoreFloat4(this,this_q);
	}

	H3DQuaternion H3DQuaternion::GetOpposite()
	{
		H3DQuaternion r( -this->x , -this->y , -this->z , -this->w );
		return r;
	}

	void H3DQuaternion::Opposite()
	{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
	}

	void H3DQuaternion::RotationMatrix(const H3DMatrix& mat )
	{
		XMMATRIX x_mat = XMLoadFloat4x4(&mat);
		XMVECTOR this_q = XMQuaternionRotationMatrix(x_mat);
		XMStoreFloat4(this,this_q);
	}

	void H3DQuaternion::Normalize()
	{
		XMVECTOR this_q = XMLoadFloat4(this);
		this_q = XMQuaternionNormalize(this_q);
		XMStoreFloat4(this,this_q);
	}

	void H3DQuaternion::RotationAxis(const H3DVector3& axis , float angle )
	{
		XMVECTOR this_q = XMLoadFloat3(&axis);
		this_q = XMQuaternionRotationAxis( this_q , angle );
		XMStoreFloat4(this,this_q);
	}

	void H3DQuaternion::Slerp(const H3DQuaternion& orientation , float slerp_speed )
	{
		XMVECTOR start_q = XMLoadFloat4(this);
		XMVECTOR orientation_q = XMLoadFloat4(&orientation);
		XMQuaternionSlerp( start_q , orientation_q , slerp_speed );
		start_q = XMQuaternionNormalize(start_q);
		XMStoreFloat4(this,start_q);
	}

	void H3DQuaternion::Slerp(const H3DQuaternion& start , const H3DQuaternion& orientation , float slerp_speed )
	{
		XMVECTOR start_q = XMLoadFloat4(&start);
		XMVECTOR orientation_q = XMLoadFloat4(&orientation);
		XMQuaternionSlerp( start_q , orientation_q , slerp_speed );
		start_q = XMQuaternionNormalize(start_q);
		XMStoreFloat4(this,start_q);
	}

	bool H3DQuaternion::operator == (const H3DQuaternion& rhs )
	{
		XMVECTOR this_v = XMLoadFloat4(this);
		XMVECTOR rhs_v = XMLoadFloat4(&rhs);
		return XMVector4Equal(this_v,rhs_v) ? true : false;
	}

	bool operator == ( const H3DQuaternion& lhs , const H3DQuaternion& rhs )
	{
		XMVECTOR lhs_v = XMLoadFloat4(&lhs);
		XMVECTOR rhs_v = XMLoadFloat4(&rhs);
		return XMVector4Equal(lhs_v,rhs_v) ? true : false;
	}

	H3DQuaternion& H3DQuaternion::operator *= (const H3DQuaternion& rhs )
	{
		XMVECTOR this_v = XMLoadFloat4(this);
		XMVECTOR rhs_v = XMLoadFloat4(&rhs);
		this_v = XMQuaternionMultiply(this_v,rhs_v);
		XMStoreFloat4(this,this_v);
		return *this;
	}

	H3DQuaternion operator * (const H3DQuaternion& lhs,const H3DQuaternion& rhs )
	{
		H3DQuaternion r;
		XMVECTOR lhs_v = XMLoadFloat4(&lhs);
		XMVECTOR rhs_v = XMLoadFloat4(&rhs);
		lhs_v = XMQuaternionMultiply(lhs_v,rhs_v);
		XMStoreFloat4(&r,lhs_v);
		return r;
	}
}