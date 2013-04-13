#include "h3d_pch.h"
#include "h3d_single_render_target_blend_state.h"
#include "h3d_renderer.h"

namespace Heaven3D
{
	H3DSingleRenderTargetBlendState::H3DSingleRenderTargetBlendState(void)
	{
		d3d_blend_state_ = NULL;
		memset( blend_factor_ , NULL , sizeof(float));
		sample_mask_ = 0xFFFFFFFF;
	}

	H3DSingleRenderTargetBlendState::~H3DSingleRenderTargetBlendState(void)
	{
		H3D_SAFE_RELEASE(d3d_blend_state_);
	}

	bool H3DSingleRenderTargetBlendState::Create(BOOL blend_enable ,D3D11_BLEND src_blend ,
		D3D11_BLEND dest_blend , D3D11_BLEND_OP blend_op , 
		D3D11_BLEND src_blend_alpha , D3D11_BLEND dst_blend_alpha ,  D3D11_BLEND_OP blend_op_alpha ,
		uint8_t render_target_write_mask )
	{
		// 创建alpha blend状态对象，并且指定这个对象所作用的0号render target的相关混合操作属性
		D3D11_BLEND_DESC blend_state_desc;
		ZeroMemory(&blend_state_desc, sizeof(D3D11_BLEND_DESC));
		blend_state_desc.IndependentBlendEnable = FALSE;
		blend_state_desc.AlphaToCoverageEnable = FALSE;
		blend_state_desc.RenderTarget[0].BlendEnable = blend_enable;
		blend_state_desc.RenderTarget[0].SrcBlend = src_blend;
		blend_state_desc.RenderTarget[0].DestBlend = dest_blend;
		blend_state_desc.RenderTarget[0].BlendOp = blend_op;
		blend_state_desc.RenderTarget[0].SrcBlendAlpha = src_blend_alpha;
		blend_state_desc.RenderTarget[0].DestBlendAlpha = dst_blend_alpha;
		blend_state_desc.RenderTarget[0].BlendOpAlpha = blend_op_alpha;
		blend_state_desc.RenderTarget[0].RenderTargetWriteMask = render_target_write_mask;

		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		HRESULT result = device->CreateBlendState(&blend_state_desc, &d3d_blend_state_);

		if( FAILED(result) )
		{
			return false;
		}
		
		return true;
	}

	void H3DSingleRenderTargetBlendState::Set()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->OMSetBlendState(d3d_blend_state_, blend_factor_, sample_mask_);
	}
}