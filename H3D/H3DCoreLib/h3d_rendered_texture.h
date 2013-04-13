/********************************************************************
	created:	2012/04/02
	created:	2:4:2012   0:15
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_rendered_texture.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_rendered_texture_h__
#define h3d_rendered_texture_h__

#include "h3d_texture.h"
//   类
namespace Heaven3D
{
	class H3DRenderedTexture : public H3DTexture
	{
	public:
		H3DRenderedTexture();
		H3DRenderedTexture(const H3DRenderedTexture&);
		~H3DRenderedTexture();
		virtual bool Create(int32_t texture_width, int32_t texture_height);
		virtual bool CreateFromFile(const TCHAR* file_name );
		virtual void Shutdown();
		virtual ID3D11ShaderResourceView* GetTexture();
		virtual void SetResourcesForPixelShader();
		void SetRenderTarget(ID3D11DepthStencilView* depth_stencil_view);
		void ClearRenderTarget(ID3D11DepthStencilView* depth_stencil_view, float red, float green, float blue, float alpha);
	private:
		ID3D11Texture2D*			rendertarget_texture_;
		ID3D11RenderTargetView*		rendertarget_view_;
		ID3D11ShaderResourceView*	shader_resource_view_;
	};
}

#endif // h3d_rendered_texture_h__
