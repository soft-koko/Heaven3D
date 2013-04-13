#include "h3d_pch.h"
#include "h3d_vector3.h"
#include "h3d_matrix.h"
#include "h3d_math_tool.h"
//   类
namespace Heaven3D
{
	void H3DVector3::Set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	void H3DVector3::clear()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	void H3DVector3::SetToAbsoluteValue()
	{
		x = AbsoluteValue(x);
		y = AbsoluteValue(y);
		z = AbsoluteValue(z);
	}

	void H3DVector3::Invert()
	{
		x = Inverse(x);
		y = Inverse(y);
		z = Inverse(z);
	}

	void H3DVector3::Negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	float H3DVector3::MaxValue()const
	{
		return maximum(x, maximum(y, z));
	}

	float H3DVector3::MinValue()const
	{
		return minimum(x, minimum(y, z));
	}

	bool H3DVector3::IsClear()const
	{
		return (x == 0.0f) && (y == 0.0f) && (z == 0.0f);
	}

	void H3DVector3::Normalize()
	{
		XMVECTOR norm = XMVector3Normalize( XMLoadFloat3(this) );
		XMVectorGetXPtr(&x,norm);
		XMVectorGetYPtr(&y,norm);
		XMVectorGetZPtr(&z,norm);
	}

	float H3DVector3::length()const
	{
		XMVECTOR len_v = XMVector3Length( XMLoadFloat3(this) );
		return XMVectorGetX(len_v);
	}

	float H3DVector3::LengthSquared()const
	{
		XMVECTOR lensq_v = XMVector3LengthSq( XMLoadFloat3(this) );
		return XMVectorGetX(lensq_v);
	}

	float H3DVector3::distance(const H3DVector3& vec)const
	{
		XMFLOAT3 tmp( x - vec.x , y - vec.y , z - vec.z);
		XMVECTOR len_v = XMVector3Length( XMLoadFloat3(&tmp) );
		return XMVectorGetX(len_v);
	}

	float H3DVector3::DistanceSquared(const H3DVector3& vec)const
	{
		XMFLOAT3 tmp( x - vec.x , y - vec.y , z - vec.z);
		XMVECTOR len_v = XMVector3LengthSq( XMLoadFloat3(&tmp) );
		return XMVectorGetX(len_v);
	}

	float H3DVector3::DotProduct(const H3DVector3& vec)const
	{
		XMVECTOR dot_v = XMVector3Dot( XMLoadFloat3(this) , XMLoadFloat3(&vec) );
		return XMVectorGetX(dot_v);
	}

	float DotProduct(const H3DVector3& a, const H3DVector3& b)
	{
		XMVECTOR dot_v = XMVector3Dot( XMLoadFloat3(&a) , XMLoadFloat3(&b) );
		return XMVectorGetX(dot_v);
	}

	void H3DVector3::CrossProduct(const H3DVector3& b)
	{
		XMVECTOR cross_v = XMVector3Cross( XMLoadFloat3(this) , XMLoadFloat3(&b) );
		x = XMVectorGetX(cross_v);
		y = XMVectorGetY(cross_v);
		z = XMVectorGetZ(cross_v);
	}

	void H3DVector3::CrossProduct(const H3DVector3& a, const H3DVector3& b)
	{
		XMVECTOR cross_v = XMVector3Cross( XMLoadFloat3(&a) , XMLoadFloat3(&b) );
		x = XMVectorGetX(cross_v);
		y = XMVectorGetY(cross_v);
		z = XMVectorGetZ(cross_v);
	}

	void H3DVector3::compute_heading_and_pitch(float& heading, float& pitch)const
	{
		heading = arctangent_of_ratio(-x, y);  
		pitch   = -acosf(z) + XM_PIDIV2;
	}

	void H3DVector3::set_from_heading_and_pitch(float heading, float pitch)
	{
		float ch = cosf(heading);
		float cp = cosf(pitch);
		float sh = sinf(heading);
		float sp = sinf(pitch);
		x = cp * sh;
		y = -sp;
		z = cp * ch;
	}

	void H3DVector3::compute_spherical_coords(float& longitude, float& latitude)const
	{
		compute_heading_and_pitch(longitude, latitude);
		longitude = modulus(longitude, XM_2PI);
		latitude = modulus(latitude, XM_2PI);

		if (longitude<0.0f) longitude = XM_2PI + longitude;
		if (latitude<0.0f) latitude = XM_2PI + latitude;

		longitude /= XM_2PI;
		latitude /= XM_2PI;
	}

	void H3DVector3::set_from_spherical_coords(float longitude, float latitude)
	{
		set_from_heading_and_pitch(longitude * XM_2PI, latitude * XM_2PI);
	}

	void H3DVector3::Reflect(const H3DVector3& normal)
	{
		XMVECTOR outcident = XMVector3Reflect( XMLoadFloat3(this),XMLoadFloat3(&normal));
		x = XMVectorGetX(outcident);
		y = XMVectorGetY(outcident);
		z = XMVectorGetZ(outcident);
	}

	void H3DVector3::Reflect(const H3DVector3& source, const H3DVector3& normal)
	{
		XMVECTOR outcident = XMVector3Reflect( XMLoadFloat3(&source),XMLoadFloat3(&normal));
		x = XMVectorGetX(outcident);
		y = XMVectorGetY(outcident);
		z = XMVectorGetZ(outcident);
	}

	void H3DVector3::Refract(const H3DVector3& normal, float incident, float transmitted)
	{
		debug_assert(transmitted != 0.0f, "invalid transmitted value");
		XMFLOAT3 _incident(incident,incident,incident);
		XMVECTOR outcident = XMVector3Refract( XMLoadFloat3(&_incident),XMLoadFloat3(&normal) , transmitted );
		x = XMVectorGetX(outcident);
		y = XMVectorGetY(outcident);
		z = XMVectorGetZ(outcident);
	}

	void H3DVector3::Refract(const H3DVector3& source, const H3DVector3& normal, float incident, float transmitted)
	{
		debug_assert(transmitted != 0.0f, "invalid transmitted value");
		XMFLOAT3 _incident(incident,incident,incident);
		XMVECTOR outcident = XMVector3Refract( XMLoadFloat3(&source),XMLoadFloat3(&normal) , transmitted );
		x = XMVectorGetX(outcident);
		y = XMVectorGetY(outcident);
		z = XMVectorGetZ(outcident);
	}

	void H3DVector3::Project(const H3DVector3& normal)
	{
		XMVECTOR normal_v = XMLoadFloat3(&normal);
		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR scalar_v = XMVector3Dot(this_v,normal_v);
		XMVECTOR result_v = XMVectorMultiply( normal_v , scalar_v );

		x = XMVectorGetX(result_v);
		y = XMVectorGetY(result_v);
		z = XMVectorGetY(result_v);
	}

	void H3DVector3::Project(const H3DVector3& source, const H3DVector3& normal)
	{
		XMVECTOR normal_v = XMLoadFloat3(&normal);
		XMVECTOR source_v = XMLoadFloat3(&source);
		XMVECTOR scalar_v = XMVector3Dot(source_v,normal_v);
		XMVECTOR result_v = XMVectorMultiply( normal_v , scalar_v );

		x = XMVectorGetX(result_v);
		y = XMVectorGetY(result_v);
		z = XMVectorGetY(result_v);
	}

	void H3DVector3::Lerp(const H3DVector3& b, float scale)
	{
		debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR b_v = XMLoadFloat3(&b);
		XMVECTOR lerp_v = XMVectorLerp(this_v,b_v,scale);
		x = XMVectorGetX(lerp_v);
		y = XMVectorGetY(lerp_v);
		z = XMVectorGetZ(lerp_v);
	}

	void H3DVector3::Lerp(const H3DVector3& a, const H3DVector3& b, float scale)
	{
		debug_assert(scale>=0.0f && scale <=1.0f, "invalid scale value");

		XMVECTOR a_v = XMLoadFloat3(&a);
		XMVECTOR b_v = XMLoadFloat3(&b);
		XMVECTOR lerp_v = XMVectorLerp(a_v,b_v,scale);
		x = XMVectorGetX(lerp_v);
		y = XMVectorGetY(lerp_v);
	}

	void H3DVector3::CreateNormal(const H3DVector3& a, const H3DVector3& b, const H3DVector3& c)
	{
		H3DVector3 SideA(b);
		H3DVector3 SideB(c);
		SideA -= a;
		SideB -= b;
		CrossProduct(SideA, SideB);
		Normalize();
	}

	void H3DVector3::CreateNormal(const H3DVector3& a, const H3DVector3& b)
	{
		CrossProduct(a, b);
		Normalize();
	}
	 
	bool H3DVector3::TestPositiveHalfSpace(const H3DVector3& normal)const
	{
		return (DotProduct(normal) > 0.0f); 
	}

	bool H3DVector3::ComparePosition(const H3DVector3& source)const
	{
		return delta_range_test(x, source.x)
			&& delta_range_test(y, source.y)
			&& delta_range_test(z, source.z);
	}

	bool H3DVector3::ComparePosition(const H3DVector3& source, float epsilon)const
	{
		return delta_range_test(x, source.x, epsilon)
			&& delta_range_test(y, source.y, epsilon)
			&& delta_range_test(z, source.z, epsilon);
	}

	bool H3DVector3::CompareLength(const H3DVector3& source)const
	{
		return delta_range_test(LengthSquared(), source.LengthSquared());
	}

	bool H3DVector3::CompareLength(const H3DVector3& source, float epsilon)const
	{
		return delta_range_test(LengthSquared(), source.LengthSquared(), epsilon);
	}

	bool H3DVector3::CompareDirection(const H3DVector3& source)const
	{
		H3DVector3 rotate90(source.y, -source.x, source.z);
		return delta_range_test(DotProduct(rotate90), 0.0f);
	}

	bool H3DVector3::CompareDirection(const H3DVector3& source, float epsilon)const
	{
		H3DVector3 rotate90(source.y, -source.x, source.z);
		return delta_range_test(DotProduct(rotate90), 0.0f, epsilon);
	}

	bool H3DVector3::CompareToZero()const
	{
		return delta_range_test(x, 0.0f)
			&& delta_range_test(y, 0.0f)
			&& delta_range_test(z, 0.0f);
	}

	bool H3DVector3::CompareToZero(float epsilon)const
	{
		return delta_range_test(x, 0.0f, epsilon)
			&& delta_range_test(y, 0.0f, epsilon)
			&& delta_range_test(z, 0.0f, epsilon);
	}

	void H3DVector3::SetMinByLength(const H3DVector3& b)
	{
		float mag_squared = LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if (mag_squared_b < mag_squared)
		{
			x = b.x;
			y = b.y;
			z = b.z;
		}
	}

	void H3DVector3::SetMinByLength(const H3DVector3& a, const H3DVector3& b)
	{
		float mag_squared_a = a.LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if (mag_squared_a < mag_squared_b)
		{
			x = a.x;
			y = a.y;
			z = a.z;
		}
		else 
		{
			x = b.x;
			y = b.y;
			z = b.z;
		}
	}

	void H3DVector3::SetMaxByLength(const H3DVector3& b)
	{
		float mag_squared = LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if (mag_squared_b > mag_squared)
		{
			x = b.x;
			y = b.y;
			z = b.z;
		}
	}

	void H3DVector3::SetMaxByLength(const H3DVector3& a, const H3DVector3& b)
	{
		float mag_squared_a = a.LengthSquared();
		float mag_squared_b = b.LengthSquared();

		if (mag_squared_a > mag_squared_b)
		{
			x = a.x;
			y = a.y;
			z = a.z;
		}
		else 
		{
			x = b.x;
			y = b.y;
			z = b.z;
		}
	}

	void H3DVector3::SetMinByValue(const H3DVector3& source_b)
	{
		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR b_v = XMLoadFloat3(&source_b);
		this_v = XMVectorMin(this_v,b_v);
		XMStoreFloat3(this,this_v);
	}

	void H3DVector3::SetMinByValue(const H3DVector3& source_a, const H3DVector3& source_b)
	{
		XMVECTOR a_v = XMLoadFloat3(&source_a);
		XMVECTOR b_v = XMLoadFloat3(&source_b);
		XMVECTOR this_v = XMVectorMin(a_v,b_v);
		XMStoreFloat3(this,this_v);
	}

	void H3DVector3::SetMaxByValue(const H3DVector3& source_b)
	{
		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR b_v = XMLoadFloat3(&source_b);
		this_v = XMVectorMax(this_v,b_v);
		XMStoreFloat3(this,this_v);
	}

	void H3DVector3::SetMaxByValue(const H3DVector3& source_a, const H3DVector3& source_b)
	{
		XMVECTOR a_v = XMLoadFloat3(&source_a);
		XMVECTOR b_v = XMLoadFloat3(&source_b);
		XMVECTOR this_v = XMVectorMax(a_v,b_v);
		XMStoreFloat3(this,this_v);
	}

	void H3DVector3::TransformNormal(const H3DMatrix& mat )
	{
		XMVECTOR v = XMLoadFloat3(this);
		XMMATRIX m = XMLoadFloat4x4(&mat);
		v = XMVector3TransformNormal( v , m );
		XMStoreFloat3(this,v);
	}

	void H3DVector3::TransformCoord(const H3DMatrix& mat )
	{
		XMVECTOR v = XMLoadFloat3(this);
		XMMATRIX m = XMLoadFloat4x4(&mat);
		v = XMVector3TransformCoord( v , m );
		XMStoreFloat3(this,v);
	}

	void H3DVector3::TransformNormal(H3DVector3* output_v ,const H3DMatrix& mat )
	{
		XMVECTOR v = XMLoadFloat3(this);
		XMMATRIX m = XMLoadFloat4x4(&mat);
		v = XMVector3TransformNormal( v , m );
		XMStoreFloat3(output_v,v);
	}

	void H3DVector3::TransformCoord(H3DVector3* output_v ,const H3DMatrix& mat )
	{
		XMVECTOR v = XMLoadFloat3(this);
		XMMATRIX m = XMLoadFloat4x4(&mat);
		v = XMVector3TransformCoord( v , m );
		XMStoreFloat3(output_v,v);
	}

	void H3DVector3::Transform( const H3DMatrix& transform_mat )
	{
		XMVECTOR v = XMLoadFloat3(this);
		XMMATRIX m = XMLoadFloat4x4(&transform_mat);
		v = XMVector3Transform( v , m );
		XMStoreFloat3(this,v);
	}

	void H3DVector3::Transform(H3DVector3* output_v , const H3DMatrix& transform_mat )
	{
		XMVECTOR v = XMLoadFloat3(this);
		XMMATRIX m = XMLoadFloat4x4(&transform_mat);
		v = XMVector3Transform( v , m );
		XMStoreFloat3(output_v,v);
	}

	const H3DVector3& H3DVector3::operator += (const H3DVector3& vec)
	{
		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR x_v = XMLoadFloat3(&vec);
		this_v = XMVectorAdd(this_v,x_v);
		XMStoreFloat3(this,this_v);
		return *this;
	}

	const H3DVector3& H3DVector3::operator *= (float v)
	{
		XMFLOAT3 _v(v,v,v);
		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR x_v = XMLoadFloat3(&_v);
		this_v = XMVectorMultiply(this_v,x_v);
		XMStoreFloat3(this,this_v);
		return *this;
	}

	const H3DVector3& H3DVector3::operator /= (float v)
	{
		XMFLOAT3 _v(v,v,v);
		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR x_v = XMLoadFloat3(&_v);
		this_v = XMVectorDivide(this_v,x_v);
		XMStoreFloat3(this,this_v);
		return *this;
	}

	const H3DVector3& H3DVector3::operator -= (const H3DVector3& v)
	{
		XMVECTOR this_v = XMLoadFloat3(this);
		XMVECTOR x_v = XMLoadFloat3(&v);
		this_v = XMVectorSubtract(this_v,x_v);
		XMStoreFloat3(this,this_v);
		return *this;
	}

	H3DVector3 operator - (const H3DVector3& lhs , const H3DVector3& rhs )
	{
		H3DVector3 r = lhs;
		r -= rhs;
		return r;
	}

	H3DVector3 operator + (const H3DVector3& lhs , const H3DVector3& rhs )
	{
		H3DVector3 r = lhs;
		r += lhs;
		return r;
	}

	H3DVector3 operator * (const H3DVector3& lhs , float rhs )
	{
		H3DVector3 r = lhs;
		r *= rhs;
		return r;
	}

	H3DVector3 operator / (const H3DVector3& lhs , float rhs )
	{
		if( rhs == 0.f )
		{
			return H3DVector3(0.f,0.f,0.f);
		}
		else
		{
			H3DVector3 r = lhs;
			r /= rhs;
			return r;
		}
	}

	H3DOStream& operator << (H3DOStream& ofs,const H3DVector3& v) 
	{
		return ofs << _T("v ") << v.x <<_T(" ") << v.y <<_T(" ")<< v.z;
	}

	H3DIStream& operator >> (H3DIStream& ifs,H3DVector3& v)
	{
		return ifs >> v.x >> v.y >> v.z;
	}
}