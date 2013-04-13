#include "h3d_pch.h"
#include "h3d_vector2.h"
#include "h3d_numeric_tools.h"
#include "h3d_math_tool.h"
//   类
namespace Heaven3D
{
	void H3DVector2::Set(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	void H3DVector2::Clear()
	{
		x = 0.0f;
		y = 0.0f;
	}

	void H3DVector2::SetToAbsoluteValue()
	{
		x = AbsoluteValue(x);
		y = AbsoluteValue(y);
	}

	void H3DVector2::Invert()
	{
		x = Inverse(x);
		y = Inverse(y);
	}

	void H3DVector2::Negate()
	{
		x = -x;
		y = -y;
	}

	float H3DVector2::MaxValue() const
	{
		return maximum(x, y);
	}

	float H3DVector2::MinValue()const
	{
		return minimum(x, y);
	}

	bool H3DVector2::IsClear()const
	{
		return (x == 0.0f) && (y == 0.0f);
	}

	void H3DVector2::Normalize()
	{
		XMVECTOR norm = XMVector2Normalize( XMLoadFloat2(this) );
		XMVectorGetXPtr(&x,norm);
		XMVectorGetYPtr(&y,norm);
	}

	float H3DVector2::length()const
	{
		float len = 0.f;
		XMVECTOR len_v = XMVector2Length( XMLoadFloat2(this) );
		XMVectorGetXPtr(&len,len_v);
		return len;
	}

	float H3DVector2::LengthSquared() const
	{
		XMVECTOR lensq_v = XMVector2LengthSq( XMLoadFloat2(this) );
		return XMVectorGetX(lensq_v);
	}

	float H3DVector2::distance(const H3DVector2& vec)const
	{
		XMFLOAT2 tmp( x - vec.x , y - vec.y);
		XMVECTOR len_v = XMVector2Length( XMLoadFloat2(&tmp) );
		return XMVectorGetX(len_v);
	}

	float H3DVector2::DistanceSquared(const H3DVector2& vec)const
	{
		XMFLOAT2 tmp( x - vec.x , y - vec.y);
		XMVECTOR len_v = XMVector2LengthSq( XMLoadFloat2(&tmp) );
		return XMVectorGetX(len_v);
	}

	float H3DVector2::DotProduct(const H3DVector2& vec)const
	{
		XMVECTOR dot_v = XMVector2Dot( XMLoadFloat2(this) , XMLoadFloat2(&vec) );
		return XMVectorGetX(dot_v);
	}

	float DotProduct(const H3DVector2& a, const H3DVector2& b)
	{
		return a.DotProduct(b);
	}

	void H3DVector2::Reflect(const H3DVector2& source, const H3DVector2& normal)
	{
		float scalar = source.DotProduct(normal) * 2.0f;
		x = (scalar*normal.x) - source.x;
		y = (scalar*normal.y) - source.y;
	}

	void H3DVector2::Refract(const H3DVector2& normal, float incident, float transmitted)
	{
		debug_assert(transmitted != 0.0f, "invalid transmitted value");

		float v_dot_n = DotProduct(normal);
		float n = incident / transmitted;
		float discrim = 1.0f - square(n)*(1.0f - square(v_dot_n));

		if(discrim < 0.0f)
		{	
			float double_dot = 2.0f*v_dot_n;

			x= (double_dot*(normal.x)) - (x);
			y= (double_dot*(normal.y)) - (y);
		}
		else
		{
			float sqrt_discrim = sqrtf((float)discrim);
			float scalar = sqrt_discrim - (n*v_dot_n);

			x= (n*x) - (scalar*normal.x);    
			y= (n*y) - (scalar*normal.y);    
		}
	}

	void H3DVector2::Refract(const H3DVector2& source, const H3DVector2& normal, float incident, float transmitted)
	{
		debug_assert(transmitted != 0.0f, "invalid transmitted value");

		float v_dot_n = source.DotProduct(normal);
		float n = incident / transmitted;
		float discrim = 1.0f - square(n)*(1.0f - square(v_dot_n));

		if( discrim < 0.0f )
		{	
			float double_dot = 2.0f*v_dot_n;

			x= (double_dot*(normal.x)) - (source.x);
			y= (double_dot*(normal.y)) - (source.y);
		}
		else
		{
			float sqrt_discrim = sqrtf((float)discrim);
			float scalar = sqrt_discrim - (n*v_dot_n);

			x= (n*source.x) - (scalar*normal.x);    
			y= (n*source.y) - (scalar*normal.y);    
		}
	}

	void H3DVector2::Project(const H3DVector2& normal)
	{
		XMVECTOR normal_v = XMLoadFloat2(&normal);
		XMVECTOR this_v = XMLoadFloat2(this);
		XMVECTOR scalar_v = XMVector2Dot(this_v,normal_v);
		XMVECTOR result_v = XMVectorMultiply( normal_v , scalar_v );
		
		x = XMVectorGetX(result_v);
		y = XMVectorGetY(result_v);
	}


	void H3DVector2::Project(const H3DVector2& source, const H3DVector2& normal)
	{
		XMVECTOR normal_v = XMLoadFloat2(&normal);
		XMVECTOR source_v = XMLoadFloat2(&source);
		XMVECTOR scalar_v = XMVector2Dot(source_v,normal_v);
		XMVECTOR result_v = XMVectorMultiply( normal_v , scalar_v );

		x = XMVectorGetX(result_v);
		y = XMVectorGetY(result_v);
	}

	void H3DVector2::Lerp(const H3DVector2& b, float scale)
	{
		debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

		XMVECTOR this_v = XMLoadFloat2(this);
		XMVECTOR b_v = XMLoadFloat2(&b);
		XMVECTOR lerp_v = XMVectorLerp(this_v,b_v,scale);
		x = XMVectorGetX(lerp_v);
		y = XMVectorGetY(lerp_v);
	}

	void H3DVector2::Lerp(const H3DVector2& a, const H3DVector2& b, float scale)
	{
		debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

		XMVECTOR a_v = XMLoadFloat2(&a);
		XMVECTOR b_v = XMLoadFloat2(&b);
		XMVECTOR lerp_v = XMVectorLerp(a_v,b_v,scale);
		x = XMVectorGetX(lerp_v);
		y = XMVectorGetY(lerp_v);
	}

	bool H3DVector2::TestPositiveHalfSpace(const H3DVector2& normal)const
	{
		return DotProduct(normal) > 0.0f; 
	}

	bool H3DVector2::ComparePosition(const H3DVector2& source)const
	{
		return delta_range_test(x, source.x) && delta_range_test(y, source.y);
	}

	bool H3DVector2::ComparePosition(const H3DVector2& source, float epsilon)const
	{
		return delta_range_test(x, source.x, epsilon)
			&& delta_range_test(y, source.y, epsilon);
	}

	bool H3DVector2::CompareLength(const H3DVector2& source)const
	{
		return delta_range_test(LengthSquared(), source.LengthSquared());
	}

	bool H3DVector2::CompareLength(const H3DVector2& source, float epsilon)const
	{
		return delta_range_test(LengthSquared(), source.LengthSquared(), epsilon);
	}

	bool H3DVector2::CompareDirection(const H3DVector2& source)const
	{
		H3DVector2 rotate90(source.y, -source.x);
		return delta_range_test(this->DotProduct(rotate90), 0.0f);
	}

	bool H3DVector2::CompareDirection(const H3DVector2& source, float epsilon)const
	{
		H3DVector2 rotate90(source.y, -source.x);
		return delta_range_test(this->DotProduct(rotate90), 0.0f, epsilon);
	}

	bool H3DVector2::CompareToZero()const
	{
		return delta_range_test(x, 0.0f) && delta_range_test(y, 0.0f);
	}

	bool H3DVector2::CompareToZero(float epsilon)const
	{
		return delta_range_test(x, 0.0f, epsilon) && delta_range_test(y, 0.0f, epsilon);
	}

	void H3DVector2::SetMinByLength(const H3DVector2& b)
	{
		float mag_squared = LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if(mag_squared_b < mag_squared)
		{
			x = b.x;
			y = b.y;
		}
	}

	void H3DVector2::SetMinByLength(const H3DVector2& a, const H3DVector2& b)
	{
		float mag_squared_a = a.LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if(mag_squared_a < mag_squared_b)
		{
			x = a.x;
			y = a.y;
		}
		else 
		{
			x = b.x;
			y = b.y;
		}
	}

	void H3DVector2::SetMaxByLength(const H3DVector2& b)
	{
		float mag_squared = LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if(mag_squared_b > mag_squared)
		{
			x = b.x;
			y = b.y;
		}
	}

	void H3DVector2::SetMaxByLength(const H3DVector2& a, const H3DVector2& b)
	{
		float mag_squared_a = a.LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if(mag_squared_a > mag_squared_b)
		{
			x = a.x;
			y = a.y;
		}
		else 
		{
			x = b.x;
			y = b.y;
		}
	}

	void H3DVector2::SetMinByValue(const H3DVector2& source_b)
	{
		XMVECTOR this_v = XMLoadFloat2(this);
		XMVECTOR source_b_v =  XMLoadFloat2(&source_b);
		XMVECTOR min_v = XMVectorMin(this_v,source_b_v);
		x = XMVectorGetX(min_v);
		y = XMVectorGetY(min_v);
	}

	void H3DVector2::SetMinByValue(const H3DVector2& source_a, const H3DVector2& source_b)
	{
		XMVECTOR source_a_v = XMLoadFloat2(this);
		XMVECTOR source_b_v =  XMLoadFloat2(&source_b);
		XMVECTOR min_v = XMVectorMin(source_a_v,source_b_v);
		x = XMVectorGetX(min_v);
		y = XMVectorGetY(min_v);
	}

	void H3DVector2::SetMaxByValue(const H3DVector2& source_b)
	{
		XMVECTOR this_v = XMLoadFloat2(this);
		XMVECTOR source_b_v =  XMLoadFloat2(&source_b);
		XMVECTOR min_v = XMVectorMax(this_v,source_b_v);
		x = XMVectorGetX(min_v);
		y = XMVectorGetY(min_v);
	}

	void H3DVector2::SetMaxByValue(const H3DVector2& source_a, const H3DVector2& source_b)
	{
		XMVECTOR source_a_v = XMLoadFloat2(this);
		XMVECTOR source_b_v =  XMLoadFloat2(&source_b);
		XMVECTOR min_v = XMVectorMax(source_a_v,source_b_v);
		x = XMVectorGetX(min_v);
		y = XMVectorGetY(min_v);
	}

	H3DOStream& operator << (H3DOStream& ofs,const H3DVector2& v) 
	{
		return ofs << _T("v ") << v.x <<_T(" ") << v.y <<_T(" ");
	}

	H3DIStream& operator >> (H3DIStream& ifs,H3DVector2& v)
	{
		return ifs >> v.x >> v.y;
	}
}