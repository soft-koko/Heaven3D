/********************************************************************
	created:	2012/03/13
	created:	13:3:2012   0:23
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_sampler_state.h
	author:		Xiong Xinke
	
	purpose:	纹理采样器状态对象
*********************************************************************/
#ifndef h3d_sampler_state_h__
#define h3d_sampler_state_h__

namespace Heaven3D
{
	class H3DSamplerState
	{
	public:
		H3DSamplerState(void);
		~H3DSamplerState(void);
		
		bool Create(D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR ,
			D3D11_TEXTURE_ADDRESS_MODE address_u = D3D11_TEXTURE_ADDRESS_WRAP , 
			D3D11_TEXTURE_ADDRESS_MODE address_v = D3D11_TEXTURE_ADDRESS_WRAP , 
			D3D11_TEXTURE_ADDRESS_MODE address_w = D3D11_TEXTURE_ADDRESS_WRAP , 
			float mip_lod_bias = 0.f , uint32_t max_anisotroy = 1 ,
			D3D11_COMPARISON_FUNC comparison_func = D3D11_COMPARISON_ALWAYS,
			float border_color_0 = 0.f , float border_color_1 =0.f, float border_color_2 = 0.f,
			float border_color_3 = 0.f, float min_lod = 0.f , float max_lod = D3D11_FLOAT32_MAX );
		
		bool Create( const D3D11_SAMPLER_DESC* desc );
		void Set();
	protected:
		ID3D11SamplerState*	d3d_sample_state_;
	};

}

#endif // h3d_sampler_state_h__
