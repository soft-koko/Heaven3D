/********************************************************************
	created:	2012/09/26
	created:	26:9:2012   16:47
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_depth_stencil_zone.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_depth_stencil_zone_h__
#define h3d_depth_stencil_zone_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DDepthStencilZone
	{
	public:
		H3DDepthStencilZone(void);
		~H3DDepthStencilZone(void);

		/** 
		* @brief GetDepthStencilView 
		* 
		* 返回ds buffer view对象
		* @return	H3D_INLINE ID3D11DepthStencilView* 
		*/
		H3D_INLINE ID3D11DepthStencilView* GetDepthStencilView()
		{
			return depth_stencil_view_;
		}
	
		/** 
		* @brief Create 
		* 
		* Detailed description.
		* @param[in] texture_desc 
		* @param[in] depth_stencil_view_desc 
		* @return		bool  
		*/
		bool Create(const D3D11_TEXTURE2D_DESC* texture_desc , const D3D11_DEPTH_STENCIL_VIEW_DESC* depth_stencil_view_desc );

		/** 
		* @brief AttachToRenderTarget 
		* 
		* 把某系列渲染目标绑定到本缓冲区上
		* @param[in] render_target_view 
		* @param[in] view_count Defaults to 1.
		* @return	void  
		*/
		void AttachToRenderTarget( ID3D11RenderTargetView** render_target_view , unsigned int view_count = 1 );

		/** 
		* @brief H3DDepthStencilView::ClearView 
		* 
		* 清空缓冲区
		* @return	void  
		*/
		void ClearView();

		/** 
		* @brief H3DDepthStencilView::ClearDepthOnly 
		* 
		* 是否设置成只清空深度缓冲区
		* @param[in] only 
		* @return	void  
		*/
		void ClearDepthOnly( bool only );

		/** 
		* @brief H3DDepthStencilView::ClearStencilOnly 
		* 
		* 是否设置成只清空模板缓冲区
		* @param[in] only 
		* @return	void  
		*/
		void ClearStencilOnly( bool only );

		/** 
		* @brief Cleanup 
		* 
		* 释放清空DS缓冲区及其相关对象
		* @return	void  
		*/
		void Cleanup();
	protected:
		ID3D11DepthStencilView* depth_stencil_view_;
		ID3D11Texture2D*		depth_stencil_buffer_;
		unsigned int			clear_flags_;
		float					clear_depth_init_value_;		// 对深度缓冲区进行clear操作后要设置的初始值
		unsigned char			clear_stencil_init_value_;		// 对模板缓冲区进行clear操作后要设置的初始值

	};
}

#endif // h3d_depth_stencil_zone_h__
