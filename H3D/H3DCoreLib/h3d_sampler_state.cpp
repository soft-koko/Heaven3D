#include "h3d_pch.h"
#include "h3d_sampler_state.h"
#include "h3d_renderer.h"
//   类
namespace Heaven3D
{
	H3DSamplerState::H3DSamplerState(void)
	{
		d3d_sample_state_ = NULL;
	}

	H3DSamplerState::~H3DSamplerState(void)
	{
	}

	bool H3DSamplerState::Create( const D3D11_SAMPLER_DESC* desc )
	{
		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		HRESULT result = device->CreateSamplerState(desc, &d3d_sample_state_);

		if(FAILED(result))
		{
			return false;
		}

		return true;
	}

	bool H3DSamplerState::Create(D3D11_FILTER filter ,
		D3D11_TEXTURE_ADDRESS_MODE address_u , D3D11_TEXTURE_ADDRESS_MODE address_v , 
		D3D11_TEXTURE_ADDRESS_MODE address_w , float mip_lod_bias , 
		uint32_t max_anisotroy ,D3D11_COMPARISON_FUNC comparison_func ,
		float border_color_0 , float border_color_1 , float border_color_2 ,
		float border_color_3 , float min_lod , float max_lod )
	{
		D3D11_SAMPLER_DESC desc;
		memset(&desc,0,sizeof(D3D11_SAMPLER_DESC));
		desc.Filter = filter;
		desc.AddressU = address_u;
		desc.AddressV = address_v;
		desc.AddressW = address_w;
		desc.MipLODBias = mip_lod_bias;
		desc.MaxAnisotropy = max_anisotroy;
		desc.ComparisonFunc = comparison_func;
		desc.BorderColor[0] = border_color_0;
		desc.BorderColor[1] = border_color_1;
		desc.BorderColor[2] = border_color_2;
		desc.BorderColor[3] = border_color_3;
		desc.MinLOD = min_lod;
		desc.MaxLOD = max_lod;


		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		HRESULT result = device->CreateSamplerState(&desc, &d3d_sample_state_);
		
		if(FAILED(result))
		{
			return false;
		}

		return true;
	}

	void H3DSamplerState::Set()
	{
		if( d3d_sample_state_ )
		{
			 H3DRenderer::GetSingletonPtr()->GetDeviceContext()->PSSetSamplers(0,1,&d3d_sample_state_);
		}
	}
}
