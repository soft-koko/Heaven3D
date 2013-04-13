/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   1:58
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_renderer.h
	author:		Xiong Xinke
	
	purpose:	代表了从渲染场景创建的纹理图。该纹理图像来保存渲染场景，
				从而产生各种特效：倒影、阴影、内部电视机
*********************************************************************/
#ifndef h3d_renderer_h__
#define h3d_renderer_h__

#include "h3d_singleton.h"
#include "h3d_matrix.h"
#include "h3d_depth_stencil_zone.h"
#include "h3d_def.h"

namespace Heaven3D
{
	class H3DMatrix;

	class H3DRenderer : public H3DSingleton<H3DRenderer>
	{
	public:
		H3DRenderer();
		~H3DRenderer();

		/** 
		* @brief Initialize 
		* 
		* Detailed description.
		* @param[in] screen_width 
		* @param[in] screen_height 
		* @param[in] vsync 
		* @param[in] hwnd 
		* @param[in] full_screen 
		* @param[in] screen_depth 
		* @param[in] screen_near 
		* @return	 bool  
		*/
		bool Initialize(int screen_width, int screen_height, bool vsync,HWND hwnd, bool full_screen,
			float screen_depth, float screen_near);
		
		/** 
		* @brief Shutdown 
		* 
		* Detailed description.
		* @return		void  
		*/
		void Shutdown();

		/** 
		* @brief BeginScene 
		* 
		* Detailed description.
		* @param[in] red 
		* @param[in] green 
		* @param[in] blue 
		* @param[in] alpha 
		* @return		void  
		*/
		void BeginScene(float red, float green, float blue, float alpha);
		
		/** 
		* @brief EndScene 
		* 
		* Detailed description.
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void EndScene()
		{
			swap_chain_->Present( sync_interval_ , 0 );
		}

		/** 
		* @brief GetDevice 
		* 
		* Detailed description.
		* @return		H3D_INLINE ID3D11Device*  
		*/
		H3D_INLINE ID3D11Device* GetDevice()
		{
			return d3d_device_;
		}

		/** 
		* @brief GetDeviceContext 
		* 
		* Detailed description.
		* @return		H3D_INLINE ID3D11DeviceContext*  
		*/
		H3D_INLINE ID3D11DeviceContext* GetDeviceContext()
		{
			return d3d_device_context_;
		}

		/** 
		* @brief GetProjectionMatrix 
		* 
		* Detailed description.
		* @param[in] projection_matrix 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void GetProjectionMatrix(H3DMatrix& projection_matrix)
		{
			projection_matrix = projection_matrix_;
		}

		/** 
		* @brief GetWorldMatrix 
		* 
		* Detailed description.
		* @param[in] world_matrix 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void GetWorldMatrix(H3DMatrix& world_matrix)
		{
			world_matrix = world_matrix_;
		}

		/** 
		* @brief GetOrthoMatrix 
		* 
		* Detailed description.
		* @param[in] ortho_matrix 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void GetOrthoMatrix(H3DMatrix& ortho_matrix)
		{
			ortho_matrix = ortho_matrix_;
		}

		/** 
		* @brief TurnZBufferOn 
		* 
		* Detailed description.
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void TurnZBufferOn()
		{
			d3d_device_context_->OMSetDepthStencilState(depth_stencil_state_, 1);
		}

		/** 
		* @brief GetDepthStencilZone 
		* 
		* Detailed description.
		* @return	H3D_INLINE  H3DDepthStencilZone&
		*/
		H3D_INLINE H3DDepthStencilZone& GetDepthStencilZone()
		{
			return depth_stencil_zone_;
		}

		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

	private:
		bool						vsync_enabled_;
		uint32_t					sync_interval_;
		IDXGISwapChain*				swap_chain_;
		ID3D11Device*				d3d_device_;
		ID3D11DeviceContext*		d3d_device_context_;
		ID3D11RenderTargetView*		back_buffer_view_;
		ID3D11DepthStencilState*	depth_stencil_state_;
		ID3D11RasterizerState*		rasterizer_state_;
		H3DDepthStencilZone			depth_stencil_zone_;

		H3DMatrix					projection_matrix_;
		H3DMatrix					world_matrix_;
		H3DMatrix					ortho_matrix_;

		ID3D11DepthStencilState*	depth_disable_stencil_state_;
		ID3D11BlendState*			alpha_enable_blending_state_;
		ID3D11BlendState*			alpha_disable_blending_state_;
	};;

#define Renderer	(H3DRenderer::GetSingletonPtr())
}

#endif // h3d_renderer_h__
