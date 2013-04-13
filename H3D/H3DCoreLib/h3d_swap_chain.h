/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   22:41
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_swap_chain.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_swap_chain_h__
#define h3d_swap_chain_h__

namespace Heaven3D
{
	class H3DRenderer;

	class H3DSwapChain : public noncopyable
	{
	private:
		friend class H3DRenderer;
		H3DSwapChain();
	public:
		~H3DSwapChain();
	private:
		IDXGISwapChain*				m_pkDXSwapChain;
		ID3D11RenderTargetView*		m_pkRTView;
		ID3D11Texture2D*			m_pkDepthStencilTex;	// 该swap chain的深度和模板缓冲，如果有的话
		ID3D11DepthStencilView*		m_pkDepthStencilView;	// 
	};
}

#endif // h3d_swap_chain_h__