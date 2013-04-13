#include "h3d_pch.h"
#include "h3d_renderer.h"
#include "h3d_singleton.h"
#include "h3d_depth_stencil_zone.h"

namespace Heaven3D
{
	H3DDepthStencilZone::H3DDepthStencilZone(void):
	depth_stencil_view_(0),
	depth_stencil_buffer_(0),
	clear_flags_(D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL),
	clear_depth_init_value_(1.f),
	clear_stencil_init_value_(0)
	{
	}

	H3DDepthStencilZone::~H3DDepthStencilZone(void)
	{
		Cleanup();
	}


	bool H3DDepthStencilZone::Create(const D3D11_TEXTURE2D_DESC* texture_desc , const D3D11_DEPTH_STENCIL_VIEW_DESC* depth_stencil_view_desc )
	{
		Cleanup();
		ID3D11Device* d3d_device = H3DRenderer::GetSingletonPtr()->GetDevice();
		HRESULT hr = d3d_device->CreateTexture2D(texture_desc, NULL, &depth_stencil_buffer_);

		if( FAILED(hr) )
			return false;

		hr = d3d_device->CreateDepthStencilView( depth_stencil_buffer_,depth_stencil_view_desc, &depth_stencil_view_);

		if( FAILED(hr) )
			return false;

		return true;
	}


	void H3DDepthStencilZone::Cleanup()
	{
		H3D_SAFE_RELEASE(depth_stencil_buffer_);
		H3D_SAFE_RELEASE(depth_stencil_view_);
	}

	void H3DDepthStencilZone::AttachToRenderTarget( ID3D11RenderTargetView** render_target_view , unsigned int view_count/* = 1 */)
	{
		if( depth_stencil_view_ )
		{
			ID3D11DeviceContext* dev_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
			dev_context->OMSetRenderTargets(view_count,render_target_view , depth_stencil_view_ );
		}
	}

	void H3DDepthStencilZone::ClearView()
	{
		ID3D11DeviceContext* dev_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		dev_context->ClearDepthStencilView(depth_stencil_view_, clear_flags_, clear_depth_init_value_, clear_stencil_init_value_);
	}

	void H3DDepthStencilZone::ClearDepthOnly( bool only )
	{
		clear_flags_ = only ? (clear_flags_ & (~D3D11_CLEAR_DEPTH)) : 
			(clear_flags_ | D3D11_CLEAR_DEPTH);
	}

	void H3DDepthStencilZone::ClearStencilOnly( bool only )
	{
		clear_flags_ = only ? (clear_flags_ & (~D3D11_CLEAR_STENCIL)) : 
			(clear_flags_ | D3D11_CLEAR_STENCIL);
	}
}