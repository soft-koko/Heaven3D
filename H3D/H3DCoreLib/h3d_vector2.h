/********************************************************************
	created:	2012/02/18
	created:	18:2:2012   0:27
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_vector2.h
	author:	Xiong Xinke
	
	purpose:	二维向量类
*********************************************************************/
#ifndef h3d_vector2_h__
#define h3d_vector2_h__

#include "h3d_def.h"
#include "h3d_iosfwd.h"

namespace Heaven3D
{
	class H3DVector2 : public XMFLOAT2
	{
	public:
		H3DVector2()
		{
		}
		
		H3DVector2(const H3DVector2& vec)
		{
			x = vec.x; 
			y = vec.y;
		}
		
		H3DVector2(float _x, float _y)
		{ 
			x = _x; 
			y = _y;
		}

		~H3DVector2(void)
		{

		}

		H3DVector2( H3DIStream& ifs) 
		{
			ifs >> x >> y;
		}

		const H3DVector2& operator *= (const H3DVector2& vec);
		const H3DVector2& operator /= (const H3DVector2& vec);
		const H3DVector2& operator *= (float v);
		const H3DVector2& operator /= (float v);
		void Clear(void);
		void SetToAbsoluteValue();
		void Set(float _x, float _y);
		void Invert();
		void Negate();
		float MaxValue()const;
		float MinValue()const;
		bool IsClear()const;
		void Normalize();
		float length()const;
		float LengthSquared()const;
		float distance(const H3DVector2& vec)const;
		float DistanceSquared(const H3DVector2& vec)const;
		float DotProduct(const H3DVector2& vec)const;
		void Reflect(const H3DVector2& normal);
		void Reflect(const H3DVector2& source, const H3DVector2& normal);
		void Refract(const H3DVector2& normal, float incident, float transmitted);
		void Refract(const H3DVector2& source, const H3DVector2& normal, float incident, float transmitted);
		void Project(const H3DVector2& normal);
		void Project(const H3DVector2& source, const H3DVector2& normal);
		void Lerp(const H3DVector2& source_b, float scale);
		void Lerp(const H3DVector2& source_a, const H3DVector2& source_b, float scale);
		bool TestPositiveHalfSpace(const H3DVector2& normal)const;
		bool ComparePosition(const H3DVector2& source)const;
		bool ComparePosition(const H3DVector2& source, float epsilon)const;
		bool CompareLength(const H3DVector2& source)const;
		bool CompareLength(const H3DVector2& source, float epsilon)const;
		bool CompareDirection(const H3DVector2& source)const;
		bool CompareDirection(const H3DVector2& source, float epsilon)const;
		bool CompareToZero()const;
		bool CompareToZero(float epsilon)const;
		void SetMaxByLength(const H3DVector2& source_b);
		void SetMaxByLength(const H3DVector2& source_a, const H3DVector2& source_b);
		void SetMinByLength(const H3DVector2& source_b);
		void SetMinByLength(const H3DVector2& source_a, const H3DVector2& source_b);
		void SetMinByValue(const H3DVector2& source_b);
		void SetMinByValue(const H3DVector2& source_a, const H3DVector2& source_b);
		void SetMaxByValue(const H3DVector2& source_b);
		void SetMaxByValue(const H3DVector2& source_a, const H3DVector2& source_b);
	};

	H3DOStream& operator << (H3DOStream& ofs,const H3DVector2& v);
	H3DIStream& operator >> (H3DIStream& ifs,H3DVector2& v);
}

#endif // h3d_vector2_h__