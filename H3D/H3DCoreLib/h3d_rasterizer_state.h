/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   18:45
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_rasterizer_state.h
	author:		Xiong Xinke
	
	purpose:	光栅器状态类
*********************************************************************/
#ifndef h3d_rasterizer_state_h__
#define h3d_rasterizer_state_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DRasterizerState
	{
	public:
		H3DRasterizerState();
		~H3DRasterizerState();
		
		/** 
		* @brief Create 
		* 
		* Detailed description.
		* @param[in] fill_mode 
		* @param[in] cull_mode 
		* @param[in] front_counter_clockwise 
		* @return	 bool  
		*/
		bool Create( D3D11_FILL_MODE fill_mode , D3D11_CULL_MODE cull_mode,bool front_counter_clockwise );

		/** 
		* @brief Apply 
		* 
		* Detailed description.
		* @return void  
		*/
		void Apply();
	private:
		ID3D11RasterizerState* state_;
	};
}

#endif // h3d_rasterizer_state_h__