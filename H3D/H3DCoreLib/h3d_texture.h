/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   2:03
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_texture.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_texture_h__
#define h3d_texture_h__
//   类
#include "h3d_def.h"

namespace Heaven3D
{
	class H3DTexture
	{
	public:
		virtual bool Create(int32_t texture_width, int32_t texture_height) = 0;
		virtual bool CreateFromFile(const TCHAR* file_name ) = 0;
		virtual void Shutdown() = 0;
		virtual ID3D11ShaderResourceView* GetTexture() = 0;
		virtual void SetResourcesForPixelShader() = 0;//PSSetShaderResources
	};
}

#endif // h3d_texture_h__
