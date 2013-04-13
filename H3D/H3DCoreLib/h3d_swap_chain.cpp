#include "h3d_pch.h"
#include "h3d_swap_chain.h"
#include "h3d_def.h"
//   类
namespace Heaven3D
{
	H3DSwapChain::H3DSwapChain():
	m_pkDXSwapChain(0),
	m_pkRTView(0),
	m_pkDepthStencilTex(0),
	m_pkDepthStencilView(0)
	{
	}

	H3DSwapChain::~H3DSwapChain()
	{
		H3D_SAFE_RELEASE(m_pkRTView);
		H3D_SAFE_RELEASE(m_pkDepthStencilTex);
		H3D_SAFE_RELEASE(m_pkDepthStencilView);
		H3D_SAFE_RELEASE(m_pkDXSwapChain);
	}
}