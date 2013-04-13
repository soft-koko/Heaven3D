#include "h3d_pch.h"
#include "h3d_camera.h"

namespace Heaven3D
{
	H3DCamera::H3DCamera()
	{
		camera_offset_ = vDefCamOffset;
		camera_position_ = H3DVector3( 0.0f , 0.0f , 0.0f );	// Initial position
		Init();
	}

	/** 
	* @brief Constructor for	H3DCamera::H3DCamera.
	* 
	* Detailed description.
	* @param[in] pos 
	* @param[in] cam_offset 
	*/
	H3DCamera::H3DCamera( const H3DVector3& pos , const H3DVector3& cam_offset )
	{
		camera_offset_ = cam_offset;
		camera_position_ = pos;

		Init();
	}

	H3DCamera::~H3DCamera()
	{

	}

	void H3DCamera::Init()
	{
		slerp_speed_ = 0.1f;
		speed_ = 10.0f;
		rotation_per_minute_ = 60.0f; // 60 rotations per minute
		camera_position_ -= camera_offset_;

		orientation_.Identity();
		view_matrix_.Identity();

		need_updated_ = true;
	}

	void H3DCamera::SetPos(const H3DVector3& new_pos)
	{
		camera_position_ = new_pos;
		need_updated_ = true;
	}

	H3DVector3 H3DCamera::GetAxisX()
	{	
		H3DVector3 vAxis;
		vAxis.x = view_matrix_._11;
		vAxis.y = view_matrix_._21;
		vAxis.z = view_matrix_._31;
		return vAxis;
	}

	H3DVector3 H3DCamera::GetAxisY()
	{
		H3DVector3 vAxis;
		vAxis.x = view_matrix_._12;
		vAxis.y = view_matrix_._22;
		vAxis.z = view_matrix_._32;
		return vAxis;
	}

	H3DVector3 H3DCamera::GetAxisZ()
	{
		H3DVector3 vAxis;
		vAxis.x = view_matrix_._13;
		vAxis.y = view_matrix_._23;
		vAxis.z = view_matrix_._33;
		return vAxis;
	}

	const H3DMatrix& H3DCamera::GetViewMatrix() 
	{
		if( need_updated_ )
		{
			Update();
		}

		return view_matrix_;
	}

	void H3DCamera::Update()
	{
		// 1) Build a new view matrix

		// 1.1) First calcuate Translation
		// 计算平移矩阵
		H3DMatrix matTranslation;

		matTranslation.Translation(-camera_position_.x , -camera_position_.y , -camera_position_.z );

		// 1.2) Now calculate rotation, by taking the conjucate of the quaternion
		// 计算旋转矩阵
		H3DMatrix rot_mat;
		rot_mat.RotationQuaternion( orientation_.GetOpposite() );

		// 2) Apply rotation & translation matrix at view matrix
		view_matrix_.Multiply(matTranslation,rot_mat);

		// 3) Set flag to false, to save CPU
		need_updated_ = false;
	}

	void H3DCamera::SetLookAt(const H3DVector3& from, const H3DVector3& to,  const H3DVector3& up)
	{
		H3DMatrix matTemp;
		matTemp.LookAtLH( from, to , up);
		camera_position_ = from;
		orientation_.RotationMatrix(matTemp);
		need_updated_ = true;
	}

	void H3DCamera::SetPerspectiveLH(float fov_angle_y , float aspect_h_w , float near_z ,float far_z)
	{
		perspective_matrix_.PerspectiveFovLH( fov_angle_y , aspect_h_w , near_z ,far_z );
	}

	void H3DCamera::SetOrthoLH( float screen_width, float screen_height, float screen_near, float screen_depth)
	{
		ortho_matrix_.OrthographicLH( screen_width,screen_height,screen_near,screen_depth );
	}

	void H3DCamera::SetCameraOffset(const H3DVector3& offset)
	{
		camera_offset_ = offset;//
		camera_position_ -= offset;
		need_updated_ = true;
	}

	void H3DCamera::SetOrientation(const H3DQuaternion& orient)
	{
		orientation_ = orient;
		need_updated_ = true;
	}

	void H3DCamera::ApplyTranslation(float distance , eDir dir)
	{
		H3DVector3 dir_vec ;

		switch( dir)
		{
		case ceMove:
			{
				dir_vec = GetAxisZ();
				break;
			}
		case ceStraf:
			{
				dir_vec = GetAxisX();
				break;
			}
		case ceUp:
			{
				dir_vec = GetAxisY();
				break;
			}
		}

		camera_position_ += dir_vec * distance * speed_;
		need_updated_ = true;
	}

	/** 
	* @brief H3DCamera::ApplyRotate 
	* 
	* Detailed description.
	* @param[in] angle 
	* @param[in] orient 
	* @return	void  
	*/
	void H3DCamera::ApplyRotate(float angle, eOrient orient)
	{
		angle *= (rotation_per_minute_ / 60); // angle * per minute rotation

		switch( orient )
		{
		case cePitch:
			{
				RotateXAxis(&orientation_ , angle);
				break;
			}
		case ceRoll:
			{
				RotateZAxis(&orientation_ , angle);
				break;
			}
		case ceYaw:
			{
				RotateYAxis(&orientation_ , angle);
				break;
			}
		}

		orientation_.Normalize();
		need_updated_ = true;
	}

	bool H3DCamera::RotateAxis(H3DQuaternion *orientation, H3DVector3 *axis, float angle)
	{ 
		bool Success=false;

		if(orientation && axis)
		{
			H3DQuaternion Rotation;

			/*D3DXQuaternionRotationAxis(	&Rotation, 
				TransformVector(orientation, axis), 
				angle);*/
			H3DVector3* transformed_vec = TransformVector( orientation, axis);
			Rotation.RotationAxis( *transformed_vec , angle );
			*orientation *= Rotation;

			Success = true;
		}

		return(Success);
	}


	bool H3DCamera::RotateXAxis(H3DQuaternion* orientation, float angle)
	{ 
		bool success = false;

		if(orientation)
		{
			H3DQuaternion rot;
			H3DVector3* transformed_vec = TransformVector( orientation, &H3DVector3(1.f, 0.f, 0.f));
			rot.RotationAxis( *transformed_vec , angle );
			*orientation *= rot;
			success = true;
		}

		return success;
	}


	bool H3DCamera::RotateYAxis(H3DQuaternion* orientation, float angle)
	{ 
		bool success = false;

		H3DQuaternion rot;
		H3DVector3* transformed_vec = TransformVector( orientation, &H3DVector3(0.f, 1.f, 0.f));
		rot.RotationAxis( *transformed_vec , angle );
		*orientation *= rot;
		success = true;
		/*H3DQuaternion Rotation;

		D3DXQuaternionRotationAxis(	&Rotation, 
		TransformVector(orientation, 
		&H3DVector3(0.0f, 1.0f, 0.0f)), 
		angle);
		*orientation *= Rotation;
		success = true;*/

		return success;
	}


	bool H3DCamera::RotateZAxis(H3DQuaternion* orientation, float angle)
	{ 
		bool success = false;


		H3DQuaternion rot;
		H3DVector3* transformed_vec = TransformVector( orientation, &H3DVector3(0.f, 0.f, 1.f));
		rot.RotationAxis( *transformed_vec , angle );
		*orientation *= rot;
		success = true;
		/*H3DQuaternion Rotation;

		D3DXQuaternionRotationAxis(	&Rotation, 
		TransformVector( orientation, 
		&H3DVector3(0.0f, 0.0f, 1.0f)), 
		angle);
		*orientation *= Rotation;

		success = true;*/


		return success;
	}

	bool H3DCamera::Slerp( const H3DQuaternion& orientation )
	{ 
		bool success = false;


		if( orientation_ == orientation)
			return false;

		orientation_.Slerp(orientation,slerp_speed_);
		success = true;
		need_updated_ = true;


		return success;
	}


	H3DVector3* H3DCamera::TransformVector( H3DQuaternion* orientation, H3DVector3 *axis)
	{
		H3DMatrix rot_mat;
		// Build a matrix from the quaternion.
		rot_mat.RotationQuaternion(*orientation);
		axis->Transform(rot_mat);
		return(axis);
	}

}
