/********************************************************************
	created:	2012/03/14
	created:	14:3:2012   1:36
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_single_render_target_blend_state.h
	author:		Xiong Xinke
	
	purpose:	仅仅作用于一个render target的blend state
*********************************************************************/
#ifndef h3d_single_render_target_blend_state_h__
#define h3d_single_render_target_blend_state_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DSingleRenderTargetBlendState
	{
	public:
		H3DSingleRenderTargetBlendState(void);
		~H3DSingleRenderTargetBlendState(void);
		
		bool Create(BOOL blend_enable ,D3D11_BLEND src_blend ,
			D3D11_BLEND dest_blend , D3D11_BLEND_OP blend_op , 
			D3D11_BLEND src_blend_alpha , D3D11_BLEND dst_blend_alpha , 
			D3D11_BLEND_OP blend_op_alpha ,	uint8_t render_target_write_mask );

		void Set();


		H3D_INLINE void ChangeBlendFactor(float blend_factor1,float blend_factor2,
			              float blend_factor3,float blend_factor4)
		{
			blend_factor_[0] = blend_factor1;
			blend_factor_[1] = blend_factor2;
			blend_factor_[2] = blend_factor3;
			blend_factor_[3] = blend_factor4;
		}

		H3D_INLINE void ChangeSampleMask(uint32_t sample_mask)
		{
			sample_mask_ = sample_mask;
		}

	protected:
		ID3D11BlendState* d3d_blend_state_;
		float			  blend_factor_[4];
		uint32_t		  sample_mask_;
	};
}

#endif // h3d_single_render_target_blend_state_h__

