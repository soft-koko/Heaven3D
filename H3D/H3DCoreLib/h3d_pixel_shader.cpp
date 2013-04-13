#include "h3d_pch.h"
#include "h3d_def.h"
#include "h3d_renderer.h"
#include "h3d_string.h"
#include "h3d_pixel_shader.h"

namespace Heaven3D
{
	H3DPixelShader::H3DPixelShader()
	{
		pixel_shader_ = NULL;
	}

	H3DPixelShader::~H3DPixelShader()
	{
		H3D_SAFE_RELEASE(pixel_shader_);
	}

	bool H3DPixelShader::Create(const TCHAR* shader_file , const TCHAR* main_func_name , const TCHAR* profile )
	{
		ID3D10Blob* err_msg = NULL;
		ID3D10Blob* ps_buffer = NULL;

		err_msg = 0;
		ps_buffer = 0;

		// 编译shader代码
		HRESULT result = D3DX11CompileFromFile( shader_file, NULL, NULL, 
			main_func_name, profile
			, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
			&ps_buffer, &err_msg, NULL );

		if(FAILED(result))
		{
			if(err_msg)
			{
				OutputDebugStringA((char*)(err_msg->GetBufferPointer()));
				OutputDebugStringA("\n");
			}
			else
			{
				OutputDebugString(_T("Missing Shader File\n"));
			}

			return false;
		}

		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		result = device->CreatePixelShader(
			ps_buffer->GetBufferPointer(), ps_buffer->GetBufferSize(), NULL, &pixel_shader_);

		if(FAILED(result))
		{
			return false;
		}

		return true;
	}

	void H3DPixelShader::Set()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		device_context->PSSetShader( pixel_shader_ , NULL , 0 );
	}

}
