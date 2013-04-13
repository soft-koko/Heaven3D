/********************************************************************
created:	2012/02/19
created:	19:2:2012   22:49
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_vector3.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_vector3_h__
#define h3d_vector3_h__

#include "h3d_def.h"
#include "h3d_vector2.h"
#include "h3d_iosfwd.h"

namespace Heaven3D
{
	class H3DMatrix;

	class H3DVector3 : public XMFLOAT3
	{
	public:
		H3DVector3(){};

		H3DVector3(const H3DVector3& vec):XMFLOAT3(vec.x, vec.y, vec.z){};
		
		H3DVector3(float _x, float _y, float _z):XMFLOAT3(_x,_y,_z){};
		
		H3DVector3(H3DVector2 _xy, float _z):XMFLOAT3(_xy.x, _xy.y, _z){};
		
		~H3DVector3(void){};
		
		H3D_INLINE H3DVector3( H3DIStream& ifs) 
		{
			ifs >> x >> y >> z;
		}
		
		H3D_INLINE H3DVector2& vec2() 
		{
			return *((H3DVector2*)this);
		}
		
		H3D_INLINE operator H3DVector2& () 
		{
			return vec2();
		}

		H3D_INLINE const H3DVector2& vec2()const
		{
			return *((H3DVector2*)this);
		}
		
		H3D_INLINE operator const H3DVector2&()const 
		{
			return vec2();
		}

		const H3DVector3& operator *= (const H3DVector3& vec);
		const H3DVector3& operator /= (const H3DVector3& vec);
		const H3DVector3& operator *= (float v);
		const H3DVector3& operator /= (float v);
		const H3DVector3& operator -= (const H3DVector3& v);
		const H3DVector3& operator += (const H3DVector3& v);

		void Set(float _x, float _y, float _z);
		void clear(void);
		void SetToAbsoluteValue();
		void Invert();
		void Negate();

		float MaxValue()const;
		float MinValue()const;
		bool IsClear()const;

		void Normalize();
		float length()const;
		float LengthSquared()const;
		float distance(const H3DVector3& vec)const;
		float DistanceSquared(const H3DVector3& vec)const;

		float DotProduct(const H3DVector3& vec)const;
		void CrossProduct(const H3DVector3& vec_b);
		void CrossProduct(const H3DVector3& vec_a, const H3DVector3& vec_b);

		void compute_heading_and_pitch(float& heading, float& pitch)const;
		void set_from_heading_and_pitch(float heading, float pitch);
		void compute_spherical_coords(float& longitude, float& latitude)const;
		void set_from_spherical_coords(float longitude, float latitude);

		void Reflect(const H3DVector3& normal);
		void Reflect(const H3DVector3& source, const H3DVector3& normal);

		void Refract(const H3DVector3& normal, float incident, float transmitted);
		void Refract(const H3DVector3& source, const H3DVector3& normal, float incident, float transmitted);

		void Project(const H3DVector3& normal);
		void Project(const H3DVector3& source, const H3DVector3& normal);

		void Lerp(const H3DVector3& source_b, float scale);
		void Lerp(const H3DVector3& source_a, const H3DVector3& source_b, float scale);

		// 根据三个给定的点，生成这三个点所确定的平面的法线，然后赋给本身
		void CreateNormal(const H3DVector3& a, const H3DVector3& b, const H3DVector3& c);

		// 顺时针方向
		void CreateNormal(const H3DVector3& a, const H3DVector3& b);

		bool TestPositiveHalfSpace(const H3DVector3& normal)const;

		bool ComparePosition(const H3DVector3& source)const;
		bool ComparePosition(const H3DVector3& source, float epsilon)const;
		bool CompareLength(const H3DVector3& source)const;
		bool CompareLength(const H3DVector3& source, float epsilon)const;
		bool CompareDirection(const H3DVector3& source)const;
		bool CompareDirection(const H3DVector3& source, float epsilon)const;
		bool CompareToZero()const;
		bool CompareToZero(float epsilon)const;

		void SetMinByValue(const H3DVector3& source_b);
		void SetMinByValue(const H3DVector3& source_a, const H3DVector3& source_b);

		void SetMaxByValue(const H3DVector3& source_b);
		void SetMaxByValue(const H3DVector3& source_a, const H3DVector3& source_b);

		void SetMinByLength(const H3DVector3& source_b);
		void SetMinByLength(const H3DVector3& source_a, const H3DVector3& source_b);

		void SetMaxByLength(const H3DVector3& source_b);
		void SetMaxByLength(const H3DVector3& source_a, const H3DVector3& source_b);

		void TransformNormal(const H3DMatrix& mat );

		void TransformCoord(const H3DMatrix& mat );

		void TransformNormal(H3DVector3* output_v ,const H3DMatrix& mat );

		void TransformCoord(H3DVector3* output_v ,const H3DMatrix& mat );

		void Transform( const H3DMatrix& transform_mat );

		void Transform(H3DVector3* output_v , const H3DMatrix& transform_mat );
	};

	H3DVector3 operator - (const H3DVector3& lhs , const H3DVector3& rhs );
	H3DVector3 operator + (const H3DVector3& lhs , const H3DVector3& rhs );
	H3DVector3 operator * (const H3DVector3& lhs , float rhs );
	H3DVector3 operator / (const H3DVector3& lhs , float rhs );
	H3DOStream& operator << (H3DOStream& ofs,const H3DVector3& v);
	H3DIStream& operator >> (H3DIStream& ifs,H3DVector3& v);
}

#endif // h3d_vector3_h__