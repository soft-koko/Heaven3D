#include "h3d_pch.h"
#include "h3d_matrix.h"
#include "h3d_quaternion.h"
#include "h3d_vector3.h"

namespace Heaven3D
{
	H3DMatrix::H3DMatrix(void)
	{
	}

	H3DMatrix::~H3DMatrix(void)
	{
	}

	H3DMatrix& H3DMatrix::operator = (CXMMATRIX mat )
	{
		XMStoreFloat4x4(this,mat);
		return *this;
	}

	void H3DMatrix::PerspectiveFovLH(float fov_angle_y , float aspect_h_w , float near_z , float far_z )
	{
		XMMATRIX mat = XMMatrixPerspectiveFovLH( fov_angle_y , aspect_h_w , near_z , far_z);
		XMStoreFloat4x4(this,mat);
	}

	void H3DMatrix::Identity()
	{
		XMMATRIX mat = XMMatrixIdentity();
		XMStoreFloat4x4(this,mat);
	}

	void H3DMatrix::OrthographicLH( float screen_width , float screen_height , float near_z , float far_z)
	{
		XMMATRIX mat = XMMatrixOrthographicLH( screen_width ,screen_height , near_z , far_z );
		XMStoreFloat4x4(this,mat);
	}

	void H3DMatrix::Translation(float offset_x , float offset_y , float offset_z)
	{
		XMMATRIX this_mat = XMMatrixTranslation( offset_x , offset_y , offset_z );
		XMStoreFloat4x4(this,this_mat);
	}

	void H3DMatrix::TranslationFromVector(const H3DVector3& vec )
	{
		XMVECTOR offset_vec = XMLoadFloat3(&vec);
		XMMATRIX this_mat = XMMatrixTranslationFromVector(offset_vec );
		XMStoreFloat4x4(this,this_mat);
	}

	// 根据四元数转化为旋转矩阵
	void H3DMatrix::RotationQuaternion(const H3DQuaternion& quaternion )
	{
		XMVECTOR q_v = XMLoadFloat4(&quaternion);
		XMMATRIX this_mat = XMMatrixRotationQuaternion(q_v);
		XMStoreFloat4x4(this,this_mat);
	}

	void H3DMatrix::Multiply( const H3DMatrix& mat1 , const H3DMatrix& mat2 )
	{
		XMMATRIX x_mat1 = XMLoadFloat4x4(&mat1);
		XMMATRIX x_mat2 = XMLoadFloat4x4(&mat2);
		XMMATRIX this_mat = XMMatrixMultiply(x_mat1,x_mat2);
		XMStoreFloat4x4(this,this_mat);
	}

	H3DMatrix& H3DMatrix::Multiply( const H3DMatrix& mat )
	{
		XMMATRIX this_mat = XMLoadFloat4x4(this);
		XMMATRIX x_mat2 = XMLoadFloat4x4(&mat);
		this_mat = XMMatrixMultiply(this_mat,x_mat2);
		XMStoreFloat4x4(this,this_mat);
		return *this;
	}

	H3DMatrix& H3DMatrix::operator *= (const H3DMatrix& mat )
	{
		XMMATRIX this_mat = XMLoadFloat4x4(this);
		XMMATRIX x_mat2 = XMLoadFloat4x4(&mat);
		this_mat = XMMatrixMultiply(this_mat,x_mat2);
		XMStoreFloat4x4(this,this_mat);
		return *this;
	}

	H3DMatrix H3DMatrix::operator * (const H3DMatrix& mat )
	{
		H3DMatrix r_mat;
		XMMATRIX x_mat1 = XMLoadFloat4x4(&r_mat);
		XMMATRIX x_mat2 = XMLoadFloat4x4(&mat);
		x_mat1 = XMMatrixMultiply(x_mat1,x_mat2);
		XMStoreFloat4x4(&r_mat,x_mat1);
		return r_mat;
	}

	void H3DMatrix::LookAtLH(const H3DVector3& eye_pos , const H3DVector3& focus_pos , const H3DVector3& up_dir)
	{
		XMVECTOR v_eye_pos = XMLoadFloat3(&eye_pos);
		XMVECTOR v_focus_pos = XMLoadFloat3(&focus_pos);
		XMVECTOR v_up_dir = XMLoadFloat3(&up_dir);
		XMMATRIX this_mat = XMMatrixLookAtLH( v_eye_pos , v_focus_pos , v_up_dir );
		XMStoreFloat4x4(this,this_mat);
	}

	void H3DMatrix::Transpose()
	{
		XMMATRIX this_m = XMLoadFloat4x4(this);
		this_m = XMMatrixTranspose(this_m);
		XMStoreFloat4x4(this,this_m);
	}

	void H3DMatrix::RotationY(float rotation)
	{
		XMMATRIX this_m = XMMatrixRotationY(rotation);
		XMStoreFloat4x4(this,this_m);
	}

	void H3DMatrix::RotationZ(float rotation)
	{
		XMMATRIX this_m = XMMatrixRotationZ(rotation);
		XMStoreFloat4x4(this,this_m);
	}

	void H3DMatrix::RotationX(float rotation)
	{
		XMMATRIX this_m = XMMatrixRotationX(rotation);
		XMStoreFloat4x4(this,this_m);
	}

	void H3DMatrix::Scaling(float x , float y , float z )
	{
		XMMATRIX this_m = XMMatrixScaling(x,y,z);
		XMStoreFloat4x4(this,this_m);
	}

	void H3DMatrix::RotationRollPitchYaw(float pitch, float yaw, float roll)
	{
		XMMATRIX this_m = XMMatrixRotationRollPitchYaw(pitch,yaw,roll);
		XMStoreFloat4x4(this,this_m);
	}
}