/********************************************************************
created:	2012/03/12
created:	12:3:2012   23:26
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_light.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_light_h__
#define h3d_light_h__

#include "h3d_def.h"
#include "h3d_vector3.h"
#include "h3d_vector4.h"
//   类
namespace Heaven3D
{
	class H3DLight
	{
	public:
		H3DLight();
		H3DLight(const H3DLight& rhs);
		H3DLight& operator = (const H3DLight& rhs );
		~H3DLight();

		H3D_INLINE void SetAmbientColor(float red, float green, float blue, float alpha)
		{
			ambient_color_ = H3DVector4(red, green, blue, alpha);
		}

		H3D_INLINE void SetDiffuseColor(float red, float green, float blue, float alpha)
		{
			diffuse_color_ = H3DVector4(red, green, blue, alpha);

		}

		H3D_INLINE void SetDirection(float x, float y, float z)
		{
			direction_ = H3DVector3(x, y, z);
		}

		H3D_INLINE void SetSpecularColor(float red, float green, float blue, float alpha)
		{
			specular_color_ = H3DVector4(red, green, blue, alpha);
		}

		H3D_INLINE void SetSpecularPower(float power)
		{
			specular_power_ = power;
		}

		H3D_INLINE const H3DVector4& GetAmbientColor()
		{
			return ambient_color_;
		}

		H3D_INLINE const H3DVector4& GetDiffuseColor()
		{
			return diffuse_color_;
		}

		H3D_INLINE const H3DVector3& GetDirection()
		{
			return direction_;
		}

		H3D_INLINE const H3DVector4& GetSpecularColor()
		{
			return specular_color_;
		}

		H3D_INLINE const H3DVector3& GetPosition()
		{
			return position_;
		}

		H3D_INLINE float GetSpecularPower()
		{
			return specular_power_;
		}

		H3D_INLINE float GetRange()
		{
			return range_;
		}

		H3D_INLINE float GetCone()
		{
			return cone_;
		}

		H3D_INLINE float GetAttenuation()
		{
			return attenuation_;
		}

		H3D_INLINE void SetAttenuation(float attenuation)
		{
			attenuation_ = attenuation;
		}

		H3D_INLINE void SetRange( float range )
		{
			range_ = range;
		}

		H3D_INLINE void SetCone( float cone )
		{
			cone_ = cone;
		}

		H3D_INLINE void SetPosition( float x , float y , float z )
		{
			position_.Set(x,y,z);
		}

	private:
		H3DVector4	ambient_color_;
		H3DVector4	diffuse_color_;
		H3DVector3	direction_;
		H3DVector4	specular_color_;
		H3DVector3	position_;
		float		range_;
		float		cone_;
		float		specular_power_;
		float		attenuation_;			// 衰减值
	};
}

#endif // h3d_light_h__
