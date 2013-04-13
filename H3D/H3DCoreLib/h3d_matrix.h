/********************************************************************
	created:	2012/03/01
	created:	1:3:2012   0:51
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_matrix.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_matrix_h__
#define h3d_matrix_h__

namespace Heaven3D
{
	class H3DQuaternion;
	class H3DVector3;

	class H3DMatrix : public XMFLOAT4X4
	{
	public:
		H3DMatrix(void);
		~H3DMatrix(void);

		H3DMatrix& operator = (CXMMATRIX mat );
		//H3DMatrix& operator = (const H3DMatrix& mat);

		void PerspectiveFovLH(float fov_angle_y , float aspect_h_w , float near_z , float far_z );

		void OrthographicLH( float screen_width , float screen_height , float near_z , float far_z);

		void Identity();

		void Translation(float offset_x , float offset_y , float offset_z);
		
		void TranslationFromVector(const H3DVector3& vec );
		
		// 根据四元数转化为旋转矩阵
		void RotationQuaternion(const H3DQuaternion& quaternion );

		void Multiply( const H3DMatrix& mat1 , const H3DMatrix& mat2 );

		H3DMatrix& Multiply( const H3DMatrix& mat );

		H3DMatrix& operator *= (const H3DMatrix& mat );

		H3DMatrix operator * (const H3DMatrix& mat );

		void LookAtLH(const H3DVector3& eye_pos , const H3DVector3& focus_pos , const H3DVector3& up_dir);

		void Transpose();

		void RotationY(float rotation);

		void RotationX(float rotation);

		void RotationZ(float rotation);

		void Scaling(float x , float y , float z );

		void RotationRollPitchYaw(float pitch, float yaw, float roll);
	};

}

#endif // h3d_matrix_h__
