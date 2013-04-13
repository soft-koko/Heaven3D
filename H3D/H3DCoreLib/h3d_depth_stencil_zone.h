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
		* ����ds buffer view����
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
		* ��ĳϵ����ȾĿ��󶨵�����������
		* @param[in] render_target_view 
		* @param[in] view_count Defaults to 1.
		* @return	void  
		*/
		void AttachToRenderTarget( ID3D11RenderTargetView** render_target_view , unsigned int view_count = 1 );

		/** 
		* @brief H3DDepthStencilView::ClearView 
		* 
		* ��ջ�����
		* @return	void  
		*/
		void ClearView();

		/** 
		* @brief H3DDepthStencilView::ClearDepthOnly 
		* 
		* �Ƿ����ó�ֻ�����Ȼ�����
		* @param[in] only 
		* @return	void  
		*/
		void ClearDepthOnly( bool only );

		/** 
		* @brief H3DDepthStencilView::ClearStencilOnly 
		* 
		* �Ƿ����ó�ֻ���ģ�建����
		* @param[in] only 
		* @return	void  
		*/
		void ClearStencilOnly( bool only );

		/** 
		* @brief Cleanup 
		* 
		* �ͷ����DS������������ض���
		* @return	void  
		*/
		void Cleanup();
	protected:
		ID3D11DepthStencilView* depth_stencil_view_;
		ID3D11Texture2D*		depth_stencil_buffer_;
		unsigned int			clear_flags_;
		float					clear_depth_init_value_;		// ����Ȼ���������clear������Ҫ���õĳ�ʼֵ
		unsigned char			clear_stencil_init_value_;		// ��ģ�建��������clear������Ҫ���õĳ�ʼֵ

	};
}

#endif // h3d_depth_stencil_zone_h__
