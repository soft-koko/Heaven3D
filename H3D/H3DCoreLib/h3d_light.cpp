#include "h3d_pch.h"
#include "h3d_light.h"
//   类
namespace Heaven3D
{
	H3DLight::H3DLight()
	{
		memset( &ambient_color_ , 0 ,sizeof(H3DVector4) );
		memset( &diffuse_color_ , 0 ,sizeof(H3DVector4) );
		memset( &specular_color_ , 0 ,sizeof(H3DVector4) );
		memset( &direction_ , 0 ,sizeof(H3DVector3) );
		memset( &position_ , 0 ,sizeof(H3DVector3) );
		range_ = 0.f;
		cone_ = 0.f;
		specular_power_ = 0.f;
		attenuation_ = 0.f;
	}


	H3DLight::H3DLight(const H3DLight& rhs )
	{
		ambient_color_ = rhs.ambient_color_;
		diffuse_color_ = rhs.diffuse_color_;
		direction_ = rhs.direction_;
		position_ = rhs.direction_;
		specular_color_ = rhs.specular_color_;
		specular_power_ = rhs.specular_power_;
		range_ = rhs.range_;
		cone_ = rhs.cone_;
		attenuation_ = rhs.attenuation_;
	}

	H3DLight& H3DLight::operator = (const H3DLight& rhs )
	{
		ambient_color_ = rhs.ambient_color_;
		diffuse_color_ = rhs.diffuse_color_;
		direction_ = rhs.direction_;
		position_ = rhs.direction_;
		specular_color_ = rhs.specular_color_;
		specular_power_ = rhs.specular_power_;
		range_ = rhs.range_;
		cone_ = rhs.cone_;
		attenuation_ = rhs.attenuation_;
		return *this;
	}


	H3DLight::~H3DLight()
	{
	}

	

}
