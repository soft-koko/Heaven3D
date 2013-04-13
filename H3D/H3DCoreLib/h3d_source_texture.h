/********************************************************************
	created:	2012/04/02
	created:	2:4:2012   1:09
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_source_texture.h
	author:		Xiong Xinke
	
	purpose:	代表了从资源创建的纹理图，既包括标准的图像文件，
				或者RAM中的一块存储区域
*********************************************************************/
#ifndef h3d_source_texture_h__
#define h3d_source_texture_h__

#include "h3d_texture.h"

namespace Heaven3D
{
	class H3DSourceTexture : public H3DTexture
	{
	public:
		H3DSourceTexture(void);
		~H3DSourceTexture(void);
		virtual bool Create(int32_t texture_width, int32_t texture_height);
		virtual bool CreateFromFile(const TCHAR* file_name );
		virtual void Shutdown();
		virtual ID3D11ShaderResourceView* GetTexture();
		virtual void SetResourcesForPixelShader();
	public:
		H3D_INLINE void SetStartSlot( uint32_t start_slot )
		{
			start_slot_ = start_slot;
		}
	protected:
		ID3D11ShaderResourceView*	d3d_texture_;
		uint32_t					start_slot_;
	};
}

#endif // h3d_source_texture_h__