// 深度缓冲区和模板缓冲区类
#include "h3d_pch.h"
#include "h3d_renderer.h"
#include "h3d_rasterizer_state.h"

namespace Heaven3D
{
	H3DRasterizerState::H3DRasterizerState()
	{
		state_ = NULL;
	}

	H3DRasterizerState::~H3DRasterizerState()
	{
		H3D_SAFE_RELEASE(state_);
	}

	bool H3DRasterizerState::Create( D3D11_FILL_MODE fill_mode , D3D11_CULL_MODE cull_mode, bool front_counter_clockwise )
	{
		H3D_SAFE_RELEASE(state_);

		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = fill_mode;
		desc.CullMode = cull_mode;
		desc.FrontCounterClockwise = front_counter_clockwise ? TRUE : FALSE;
		HRESULT hr = H3DRenderer::GetSingletonPtr()->GetDevice()->CreateRasterizerState(&desc, &state_);

		if( S_OK == hr )
		{
			return true;
		}

		return false;
	}

	void H3DRasterizerState::Apply()
	{
		if( state_ )
		{
			H3DRenderer::GetSingletonPtr()->GetDeviceContext()->RSSetState(state_);
		}
	}
}