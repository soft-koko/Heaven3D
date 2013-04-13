#include "h3d_pch.h"
#include "h3d_rendered_texture.h"
#include "h3d_renderer.h"
//   类
namespace Heaven3D
{

	H3DRenderedTexture::H3DRenderedTexture()
	{
		rendertarget_texture_ = 0;
		rendertarget_view_ = 0;
		shader_resource_view_ = 0;
	}


	H3DRenderedTexture::H3DRenderedTexture(const H3DRenderedTexture& other)
	{
	}


	H3DRenderedTexture::~H3DRenderedTexture()
	{
	}

	bool H3DRenderedTexture::CreateFromFile(const TCHAR* file_name )
	{
		return false;
	}


	bool H3DRenderedTexture::Create( int32_t texture_width, int32_t texture_height )
	{
		D3D11_TEXTURE2D_DESC texture_desc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC rendertarget_view_desc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ID3D11Device*		 d3d_device = H3DRenderer::GetSingletonPtr()->GetDevice();
		
		ZeroMemory(&texture_desc, sizeof(texture_desc));

		
		texture_desc.Width = texture_width;
		texture_desc.Height = texture_height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;

		result = d3d_device->CreateTexture2D(&texture_desc, NULL, &rendertarget_texture_);

		if(FAILED(result))
		{
			return false;
		}

		rendertarget_view_desc.Format = texture_desc.Format;
		rendertarget_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rendertarget_view_desc.Texture2D.MipSlice = 0;

		result = d3d_device->CreateRenderTargetView(rendertarget_texture_, &rendertarget_view_desc, &rendertarget_view_);
		
		if(FAILED(result))
		{
			return false;
		}

		shaderResourceViewDesc.Format = texture_desc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		result = d3d_device->CreateShaderResourceView(rendertarget_texture_, &shaderResourceViewDesc, &shader_resource_view_);
		
		if(FAILED(result))
		{
			return false;
		}

		return true;
	}


	void H3DRenderedTexture::Shutdown()
	{
		if(shader_resource_view_)
		{
			shader_resource_view_->Release();
			shader_resource_view_ = 0;
		}

		if(rendertarget_view_)
		{
			rendertarget_view_->Release();
			rendertarget_view_ = 0;
		}

		if(rendertarget_texture_)
		{
			rendertarget_texture_->Release();
			rendertarget_texture_ = 0;
		}

		return;
	}


	void H3DRenderedTexture::SetRenderTarget(ID3D11DepthStencilView* depth_stencil_view)
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->OMSetRenderTargets(1, &rendertarget_view_, depth_stencil_view);
	}


	void H3DRenderedTexture::ClearRenderTarget(ID3D11DepthStencilView* depth_stencil_view, 
		float red, float green, float blue, float alpha)
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		
		float color[4];
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		device_context->ClearRenderTargetView(rendertarget_view_, color);
		device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return;
	}

	ID3D11ShaderResourceView* H3DRenderedTexture::GetTexture()
	{
		return shader_resource_view_;
	}

	void H3DRenderedTexture::SetResourcesForPixelShader()
	{
	}
}