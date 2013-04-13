#include "h3d_pch.h"
#include "h3d_source_texture.h"
#include "h3d_renderer.h"

//   类
namespace Heaven3D
{
	H3DSourceTexture::H3DSourceTexture(void)
	{
		d3d_texture_ = NULL;
		start_slot_ = 0;
	}

	H3DSourceTexture::~H3DSourceTexture(void)
	{
		Shutdown();
	}

	bool H3DSourceTexture::Create(int32_t texture_width, int32_t texture_height)
	{
		return false;
	}

	bool H3DSourceTexture::CreateFromFile( const TCHAR* filename )
	{
		HRESULT result  = D3DX11CreateShaderResourceViewFromFile( 
			H3DRenderer::GetSingletonPtr()->GetDevice(), 
			filename, NULL, NULL, &d3d_texture_, NULL);

		if(FAILED(result))
		{
			return false;
		}

		return true;
	}

	void H3DSourceTexture::Shutdown()
	{
		H3D_SAFE_RELEASE(d3d_texture_);
	}

	ID3D11ShaderResourceView* H3DSourceTexture::GetTexture()
	{
		return d3d_texture_;
	}

	void H3DSourceTexture::SetResourcesForPixelShader()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->PSSetShaderResources( start_slot_ , 1, &d3d_texture_ );
	}
}