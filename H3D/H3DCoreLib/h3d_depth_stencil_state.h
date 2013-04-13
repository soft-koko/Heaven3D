/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   18:28
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_depth_stencil_state.h
	author:		Xiong Xinke
	
	purpose:	深度缓冲区和模板缓冲区类
*********************************************************************/
#ifndef h3d_depth_stencil_state_h__
#define h3d_depth_stencil_state_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DDepthStencilState
	{
	public:
		H3DDepthStencilState();
		~H3DDepthStencilState();

		static void ZeroClearing();
		
		/** 
		* @brief Apply 
		* 
		* Detailed description.
		* @return		void  
		*/
		void Apply();

		/** 
		* @brief Create 
		* 
		* Detailed description.
		* @param[in] depth_enable 
		* @param[in] depth_write_mask 
		* @param[in] comparison_func 
		* @return		bool  
		*/
		bool Create(bool depth_enable , D3D11_DEPTH_WRITE_MASK depth_write_mask,D3D11_COMPARISON_FUNC comparison_func );
	protected:
		ID3D11DepthStencilState* state_;
	};
}

#endif // h3d_depth_stencil_state_h__