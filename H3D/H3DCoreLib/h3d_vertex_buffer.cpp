#include "h3d_pch.h"
#include "h3d_def.h"
#include "h3d_debug.h"
#include "h3d_vertex_buffer.h"
#include "h3d_renderer.h"
//   类
namespace Heaven3D
{
	H3DVertexBuffer::H3DVertexBuffer(void)
	{
		vertex_buffer_ = 0;
		vertex_offset_ = 0;
		vertex_count_ = 0;
		vertex_stride_ = 0;
	}

	H3DVertexBuffer::~H3DVertexBuffer(void)
	{
		H3D_SAFE_RELEASE(vertex_buffer_);
	}

	bool H3DVertexBuffer::Create( uint32_t vertex_count, uint32_t vertex_stride, uint32_t vertex_offset ,  D3D11_USAGE usage , D3D_PRIMITIVE_TOPOLOGY primitive_topology ,  const void* vertex_data )
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = usage;
		desc.ByteWidth = vertex_count * vertex_stride;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0);
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertex_buffer_data;
		vertex_buffer_data.pSysMem = vertex_data;
		vertex_buffer_data.SysMemPitch = 0;
		vertex_buffer_data.SysMemSlicePitch = 0;

		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		HRESULT hr = device->CreateBuffer( &desc, (vertex_data ? &vertex_buffer_data : NULL ), &vertex_buffer_ );
		
		if( FAILED(hr) )
		{
			ASSERT( 0 && "Couldn't create vertex buffer");
			return false;
		}

		vertex_count_ = vertex_count;
		vertex_stride_ = vertex_stride;
		vertex_offset_ = vertex_offset;
		usage_ = usage;
		primitive_topology_ = primitive_topology;
		return true;
	}

	void H3DVertexBuffer::Set()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		uint32_t start_slot = 0;
		uint32_t buffer_num = 1;
		device_context->IASetVertexBuffers(0,1,&vertex_buffer_ ,&vertex_stride_ , &vertex_offset_ );
		device_context->IASetPrimitiveTopology( primitive_topology_ );
	}

	void H3DVertexBuffer::Draw()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->Draw( vertex_count_, 0 );
	}

	
}
