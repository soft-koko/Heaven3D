#include "h3d_pch.h"
#include "h3d_def.h"
#include "h3d_index_buffer.h"
#include "h3d_renderer.h"
//   类
namespace Heaven3D
{
	H3DIndexBuffer::H3DIndexBuffer(void)
	{
		index_count_ = 0;
		index_buffer_ = NULL;
	}

	H3DIndexBuffer::~H3DIndexBuffer(void)
	{
		H3D_SAFE_RELEASE(index_buffer_);
	}

	bool H3DIndexBuffer::Create(uint32_t index_count , const uint32_t* indices_data, D3D11_USAGE usage )
	{
		D3D11_BUFFER_DESC		desc;
		D3D11_SUBRESOURCE_DATA	index_data;

		desc.Usage = usage;
		desc.ByteWidth = sizeof(uint32_t) * index_count;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		index_data.pSysMem = indices_data;
		index_data.SysMemPitch = 0;
		index_data.SysMemSlicePitch = 0;

		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		HRESULT result = device->CreateBuffer(&desc, &index_data, &index_buffer_);

		if(FAILED(result))
		{
			return false;
		}

		index_count_ = index_count;
		usage_ = usage;
		return true;
	}

	void H3DIndexBuffer::Set()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->IASetIndexBuffer( index_buffer_, DXGI_FORMAT_R32_UINT, 0 );
	}
}
