#include "h3d_pch.h"
#include "h3d_shader_effect.h"
#include "h3d_vertex_shader.h"
#include "h3d_pixel_shader.h"
#include "h3d_renderer.h"
#include "h3d_string.h"

namespace Heaven3D
{
	void H3DShaderEffect::CreateVerexShader()
	{
		vertex_shader_ = new H3DVertexShader;
	}

	void H3DShaderEffect::CreatePixelShader()
	{
		pixel_shader_ = new H3DPixelShader;
	}

	H3DShaderEffect::H3DShaderEffect(void)
	{
		pixel_shader_ = NULL;
		vertex_shader_ = NULL;
		memset( const_buffer_ , NULL , sizeof(ID3D11Buffer*) * MAX_CONST_BUFFER_COUNT );
	}

	H3DShaderEffect::~H3DShaderEffect(void)
	{
		H3D_SAFE_DELETE(pixel_shader_);
		H3D_SAFE_DELETE(vertex_shader_);

		for( int i = 0 ; i < MAX_CONST_BUFFER_COUNT ; ++i )
		{
			H3D_SAFE_RELEASE(const_buffer_[i]);
		}
	}

	ID3D11Buffer* H3DShaderEffect::GetConstantBuffer( ConstBufferType buf_type )
	{
		if( MAX_CONST_BUFFER_COUNT == buf_type )
			return NULL;

		return const_buffer_[buf_type];
	}

	bool H3DShaderEffect::CreateConstBuffer( ConstBufferType buf_type , const D3D11_BUFFER_DESC& buf_desc )
	{
		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		HRESULT result = device->CreateBuffer( &buf_desc , NULL , &const_buffer_[buf_type]);
		return true;
	}

	// 设置shader的常量参数
	bool H3DShaderEffect::SetShaderConstParam(ShaderType shader_type , ConstBufferType buf_type , const void* param_data , const uint32_t param_data_size )
	{
		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		
		// 开始填充数据，锁定
		HRESULT result = device_context->Map( const_buffer_[buf_type], 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
		
		if(FAILED(result))
		{
			return false;
		}

		memcpy( mapped_resource.pData , param_data , param_data_size );
		// 数据填充完毕，解锁
		device_context->Unmap(const_buffer_[buf_type], 0);

		// 把常量buffer设置给vertex shader
		switch( shader_type )
		{
		case VERTEX_SHADER:
			device_context->VSSetConstantBuffers( (uint32_t)(buf_type), 1, &const_buffer_[buf_type]);
			break;
		case PIXEL_SHADER:
			device_context->PSSetConstantBuffers( (uint32_t)(MAX_PS_CONST_BUFFER_COUNT - buf_type), 1, &const_buffer_[buf_type]);
			break;
		case GEOMETRY_SHADER:
			device_context->GSGetConstantBuffers( (uint32_t)(MAX_GS_CONST_BUFFER_COUNT - buf_type), 1, &const_buffer_[buf_type]);
			break;
		case COMPUTE_SHADER:
			device_context->CSSetConstantBuffers( (uint32_t)(MAX_CS_CONST_BUFFER_COUNT - buf_type), 1, &const_buffer_[buf_type]);
			break;
		case HULL_SHADER:
			device_context->HSSetConstantBuffers( (uint32_t)(MAX_HS_CONST_BUFFER_COUNT - buf_type), 1, &const_buffer_[buf_type]);
			break;
		}

		return true;
	}
}
