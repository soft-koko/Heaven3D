/********************************************************************
	created:	2012/03/02
	created:	2:3:2012   0:10
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_quaternion.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_quaternion_h__
#define h3d_quaternion_h__

namespace Heaven3D
{
	class H3DMatrix;

	class H3DQuaternion : public XMFLOAT4
	{
	public:
		H3DQuaternion(void);
		
		H3DQuaternion(float x,float y, float z , float w);
		
		~H3DQuaternion(void);
		
		void Identity();
		
		H3DQuaternion operator - (const H3DQuaternion rhs );
		
		void RotationMatrix(const H3DMatrix& mat );
		
		void Normalize();

		// 根据旋转轴和旋转角构建四元数
		void RotationAxis(const H3DVector3& axis , float angle );

		// 以本身为起始，orientation为目标进行球面线性插值，得到的值再赋给本身
		void Slerp(const H3DQuaternion& orientation , float slerp_speed );
		
		// 以start为起始，orientation为目标进行球面线性插值，得到的值再赋给本身
		void Slerp(const H3DQuaternion& start , const H3DQuaternion& orientation , float slerp_speed );

		bool operator == (const H3DQuaternion& rhs );
		
		H3DQuaternion GetOpposite();

		void Opposite();

		H3DQuaternion& operator *= (const H3DQuaternion& rhs );
	};

	bool operator == ( const H3DQuaternion& lhs , const H3DQuaternion& rhs );

	H3DQuaternion operator * (const H3DQuaternion& lhs , const H3DQuaternion& rhs );
}
#endif // h3d_quaternion_h__

