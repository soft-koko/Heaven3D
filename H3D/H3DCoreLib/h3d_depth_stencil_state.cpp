// 深度缓冲区和模板缓冲区类
#include "h3d_pch.h"
#include "h3d_depth_stencil_state.h"
#include "h3d_renderer.h"
#include "h3d_singleton.h"

namespace Heaven3D
{
	H3DDepthStencilState::H3DDepthStencilState()
	{
		state_ = NULL;
	}

	H3DDepthStencilState::~H3DDepthStencilState()
	{
		H3D_SAFE_RELEASE(state_);
	}

	bool H3DDepthStencilState::Create(bool depth_enable , D3D11_DEPTH_WRITE_MASK depth_write_mask,
		D3D11_COMPARISON_FUNC comparison_func )
	{
		D3D11_DEPTH_STENCIL_DESC dss_desc;
		ZeroMemory(&dss_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dss_desc.DepthEnable = depth_enable ? TRUE : FALSE;
		dss_desc.DepthWriteMask = depth_write_mask;
		dss_desc.DepthFunc = comparison_func;

		HRESULT hr = H3DRenderer::GetSingletonPtr()->GetDevice()->CreateDepthStencilState(&dss_desc, &state_);

		if( S_OK == hr )
		{
			return true;
		}

		return false;
	}

	void H3DDepthStencilState::Apply()
	{
		if( state_ )
		{
			H3DRenderer::GetSingletonPtr()->GetDeviceContext()->OMSetDepthStencilState(state_, 0);
		}
	}

	void H3DDepthStencilState::ZeroClearing()
	{
		H3DRenderer::GetSingletonPtr()->GetDeviceContext()->OMSetDepthStencilState(NULL, 0);
	}
}