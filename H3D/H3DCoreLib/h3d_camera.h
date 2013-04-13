
/********************************************************************
	created:	2012/03/01
	created:	1:3:2012   0:29
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_camera.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/

#ifndef h3d_camera_h__
#define h3d_camera_h__

#include "h3d_def.h"
#include "h3d_math_tool.h"
#include "h3d_matrix.h"
#include "h3d_vector3.h"
#include "h3d_quaternion.h"

namespace Heaven3D
{
	const H3DVector3 vDefCamOffset(0.0f , 5.0f , -10.0f);

	class H3DCamera  
	{
	private:
		
		enum eDir		
		{ 
			ceMove, 
			ceStraf, // 扫射，即绕自身的y轴旋转
			ceUp
		};

		enum eOrient	
		{ 
			cePitch , 
			ceRoll , 
			ceYaw 
		};

	public:
		/** 
		* @brief Constructor for		H3DCamera.
		* 
		* Detailed description.
		*/
		H3DCamera();

		/** 
		* @brief Constructor for		H3DCamera.
		* 
		* Detailed description.
		* @param[in] pos 
		* @param[in] cam_offset 
		*/
		H3DCamera( const H3DVector3& pos , const H3DVector3& cam_offset );

		/** 
		* @brief ~H3DCamera 
		* 
		* Detailed description.
		* @return al  
		*/
		virtual ~H3DCamera();

		/** 
		* @brief SetOrientation 
		* 
		* Detailed description.
		* @param[in] orient 
		* @return		void  
		*/

		void SetOrientation(const H3DQuaternion& orient );

		/** 
		* @brief SetLookAt 
		* 
		* Detailed description. 根据摄像机自身在世界坐标下的坐标值，被摄像机观察的目标的坐标值，摄像机自身坐标系的向上的向量值，计算出观察坐标系
		* @param[in] from		摄像机自身在世界坐标下的坐标值
		* @param[in] to			被摄像机观察的目标的坐标值
		* @param[in] up			摄像机自身坐标系的向上的向量值，为单位向量
		* @return	 void  
		*/
		void SetLookAt(const H3DVector3& from, const H3DVector3& to,  const H3DVector3& up);

		/** 
		* @brief SetOrthoLH 
		* 
		* Detailed description.
		* @param[in] screen_width 
		* @param[in] screen_height 
		* @param[in] screen_near 
		* @param[in] screen_depth 
		* @return		void  
		*/
		void SetOrthoLH( float screen_width, float screen_height, float screen_near, float screen_depth);

		/** 
		* @brief SetPerspectiveLH 
		* 
		* Detailed description.
		* @param[in] fov_angle_y 
		* @param[in] aspect_h_w 
		* @param[in] near_z 
		* @param[in] far_z 
		* @return		void  
		*/
		void SetPerspectiveLH(float fov_angle_y , float aspect_h_w , float near_z ,float far_z);
		
		/** 
		* @brief SetPos 
		* 
		* Detailed description.
		* @param[in] new_pos 
		* @return		void  
		*/
		void SetPos(const H3DVector3& new_pos);

		/** 
		* @brief SetCameraOffset 
		* 
		* Detailed description.
		* @param[in] offset 
		* @return		void  
		*/
		void SetCameraOffset( const H3DVector3& offset);

		/** 
		* @brief GetOrthoMatrix 
		* 
		* Detailed description.
		* @return		H3D_INLINE const H3DMatrix&  
		*/
		H3D_INLINE const H3DMatrix& GetOrthoMatrix()
		{
			return ortho_matrix_;
		}

		/** 
		* @brief GetPerspectiveMatrix 
		* 
		* Detailed description.
		* @return		H3D_INLINE const H3DMatrix&  
		*/
		H3D_INLINE const H3DMatrix& GetPerspectiveMatrix()
		{
			return perspective_matrix_;
		}

		/** 
		* @brief GetPos 
		* 
		* Detailed description.
		* @return		H3D_INLINE const H3DVector3&  
		*/
		H3D_INLINE const H3DVector3& GetPos() const 
		{ 
			return camera_position_; 
		}

		/** 
		* @brief Pitch 
		* 
		* Detailed description.
		* @param[in] angle 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void Pitch(float angle)
		{
			ApplyRotate(angle , cePitch); 
		}
		
		/** 
		* @brief Roll 
		* 
		* Detailed description.
		* @param[in] angle 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void Roll(float angle)
		{ 
			ApplyRotate(angle , ceRoll);	 
		}
		
		/** 
		* @brief Yaw 
		* 
		* Detailed description.
		* @param[in] angle 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void Yaw(float angle)
		{ 
			ApplyRotate(angle , ceYaw);	 
		}

		/** 
		* @brief Move 
		* 
		* Detailed description.
		* @param[in] distance 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void Move(float distance )
		{ 
			ApplyTranslation(distance , ceMove); 
		}

		/** 
		* @brief Strafe 
		* 
		* Detailed description.
		* @param[in] distance 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void Strafe(float distance )
		{ 
			ApplyTranslation(distance , ceStraf); 
		}

		/** 
		* @brief Up 
		* 
		* Detailed description.
		* @param[in] distance 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void Up(float distance)
		{ 
			ApplyTranslation(distance , ceUp);   
		}

		/** 
		* @brief GetViewMatrix 
		* 
		* Detailed description.
		* @return		const H3DMatrix&  
		*/
		const H3DMatrix& GetViewMatrix();

		/** 
		* @brief GetAxisZ 
		* 
		* Detailed description.
		* @return		H3DVector3  
		*/
		H3DVector3 GetAxisZ();
		/** 
		* @brief GetAxisY 
		* 
		* Detailed description.
		* @return		H3DVector3  
		*/
		H3DVector3 GetAxisY();

		/** 
		* @brief GetAxisX 
		* 
		* Detailed description.
		* @return		H3DVector3  
		*/
		H3DVector3 GetAxisX();

		/** 
		* @brief H3DCamera::GetRotation 
		* 
		* Detailed description.
		* @return		H3D_INLINE const H3DQuaternion&  
		*/
		H3D_INLINE const H3DQuaternion& H3DCamera::GetRotation()
		{ 
			return orientation_; 
		}

		static bool RotateZAxis(H3DQuaternion*orientation, float angle);
		/** 
		* @brief RotateYAxis 
		* 
		* Detailed description.
		* @param[in] orientation 
		* @param[in] angle 
		* @return ic bool  
		*/
		static bool RotateYAxis(H3DQuaternion* orientation, float angle);

		/** 
		* @brief RotateXAxis 
		* 
		* Detailed description.
		* @param[in] orientation 
		* @param[in] angle 
		* @return ic bool  
		*/
		static bool RotateXAxis(H3DQuaternion* orientation, float angle);
		/** 
		* @brief RotateAxis 
		* 
		* Detailed description.
		* @param[in] orientation 
		* @param[in] axis 
		* @param[in] angle 
		* @return ic bool  
		*/
		static bool RotateAxis(H3DQuaternion* orientation, H3DVector3* axis, float angle);
		/** 
		* @brief TransformVector 
		* 
		* Detailed description.
		* @param[in] orientation 
		* @param[in] axis 
		* @return ic H3DVector3*  
		*/
		static H3DVector3* TransformVector(H3DQuaternion*  orientation, H3DVector3* axis);

		/** 
		* @brief Slerp 
		* 
		* Detailed description.
		* @param[in] orientation 
		* @return		bool  
		*/
		bool Slerp( const H3DQuaternion& orientation);

		/** 
		* @brief SetSpeed 
		* 
		* Detailed description.
		* @param[in] speed 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void SetSpeed(float speed)
		{ 
			speed_ = (speed < 0.0f) ? 0.0f : speed;
		}

		/** 
		* @brief SetRPM 
		* 
		* Detailed description.
		* @param[in] rotation_per_minute 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void SetRPM(float rotation_per_minute)
		{ 
			rotation_per_minute_ = (rotation_per_minute < 0.0f) ? 0.0f : rotation_per_minute; 
		}
		
		/** 
		* @brief SetSlerpSpeed 
		* 
		* Detailed description.
		* @param[in] slerp_speed 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void SetSlerpSpeed(float slerp_speed)
		{ 
			slerp_speed_ = (slerp_speed >= 0.0f && slerp_speed <= 1.0) ? slerp_speed : slerp_speed_; 
		}

		/** 
		* @brief GetSpeed 
		* 
		* Detailed description.
		* @return		H3D_INLINE float  
		*/
		H3D_INLINE float GetSpeed()
		{ 
			return speed_;
		}
		
		/** 
		* @brief GetRPM 
		* 
		* Detailed description.
		* @return		H3D_INLINE float  
		*/
		H3D_INLINE float GetRPM()
		{ 
			return rotation_per_minute_;
		}

		/** 
		* @brief GetSlerpSpeed 
		* 
		* Detailed description.
		* @return		H3D_INLINE float  
		*/
		H3D_INLINE float GetSlerpSpeed() 
		{
			return slerp_speed_; 
		}

		/** 
		* @brief Init 
		* 
		* Detailed description.
		* @return		void  
		*/
		void Init();

		/** 
		* @brief Update 
		* 
		* Detailed description.
		* @return		void  
		*/
		void Update();
	protected:

	private:
		/** 
		* @brief ApplyTranslation 
		* 
		* Detailed description.
		* @param[in] distance 
		* @param[in] dir 
		* @return		void  
		*/
		void ApplyTranslation(float distance , eDir dir );
		/** 
		* @brief ApplyRotate 
		* 
		* Detailed description.
		* @param[in] angle 
		* @param[in] orient 
		* @return		void  
		*/
		void ApplyRotate(float angle , eOrient orient);

	private:
		H3DVector3		camera_position_;
		H3DVector3		camera_offset_;
		H3DQuaternion	orientation_;
		H3DMatrix		view_matrix_;
		H3DMatrix		ortho_matrix_;
		H3DMatrix		perspective_matrix_;
		bool			need_updated_;
		float			slerp_speed_;
		float			speed_;
		float			rotation_per_minute_;					// RPM = rotations per minute
	};

}

#endif // h3d_camera_h__