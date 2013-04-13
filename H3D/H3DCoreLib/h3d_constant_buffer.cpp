#include "h3d_pch.h"
#include "h3d_constant_buffer.h"
#include "h3d_def.h"
#include "h3d_renderer.h"
//   类
namespace Heaven3D
{
	H3DConstantBuffer::H3DConstantBuffer(void)
	{
		d3d_const_buffer_ = NULL;
		start_slot_ = 0;
	}

	H3DConstantBuffer::~H3DConstantBuffer(void)
	{
		H3D_SAFE_RELEASE(d3d_const_buffer_);
	}

	bool H3DConstantBuffer::UpdateBufferData(const void* src_data , uint32_t src_data_size , uint32_t sub_resource,D3D11_MAP d3d_map  , uint32_t map_flag )
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		HRESULT result = device_context->Map( d3d_const_buffer_, sub_resource, 
												d3d_map, map_flag , &mapped_resource);
		
		if(FAILED(result))
		{
			return false;
		}

		memcpy( mapped_resource.pData , src_data , src_data_size );
		device_context->Unmap(d3d_const_buffer_, sub_resource );
		return true;
	}

	bool H3DConstantBuffer::Create( const void* src_data , uint32_t buf_size , D3D11_USAGE buf_usage )
	{
		H3D_SAFE_RELEASE(d3d_const_buffer_);

		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();

		D3D11_BUFFER_DESC bd;
		memset( &bd , 0 , sizeof(D3D11_BUFFER_DESC));
		bd.Usage = buf_usage;
		bd.ByteWidth = buf_size;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = (buf_usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0);

		D3D11_SUBRESOURCE_DATA initial_data;
		initial_data.pSysMem = src_data;
		initial_data.SysMemPitch = 0;
		initial_data.SysMemSlicePitch = 0;
		
		HRESULT hr = device->CreateBuffer( &bd, &initial_data, &d3d_const_buffer_ );
		
		if( FAILED( hr ) )
			return false;

		return true;
	}

	void H3DConstantBuffer::UpdateSubresource(const void* src_data , uint32_t src_row_pitch ,  uint32_t src_depth_pitch)
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->UpdateSubresource( d3d_const_buffer_, 0, NULL, src_data , src_row_pitch , src_depth_pitch );
	}

	void H3DConstantBuffer::SetForVertexShader()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->VSSetConstantBuffers(start_slot_, 1, &d3d_const_buffer_ );
	}

	void H3DConstantBuffer::SetForPixelShader()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->PSSetConstantBuffers(start_slot_,1,&d3d_const_buffer_);
	}
}
