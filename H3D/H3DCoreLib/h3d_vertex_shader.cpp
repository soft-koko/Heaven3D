#include "h3d_pch.h"
#include "h3d_vertex_shader.h"
#include "h3d_renderer.h"
#include "h3d_string.h"

namespace Heaven3D
{
	H3DVertexShader::H3DVertexShader()
	{
		vertex_shader_ = NULL;
		input_layout_ = NULL;
	}

	H3DVertexShader::~H3DVertexShader()
	{
		H3D_SAFE_RELEASE(input_layout_);
		H3D_SAFE_RELEASE(vertex_shader_);
	}

	bool H3DVertexShader::Create(const TCHAR* shader_file , const TCHAR* main_func_name , const TCHAR* profile ,
		const D3D11_INPUT_ELEMENT_DESC* input_elem_desc , uint32_t input_elem_num )
	{
		HRESULT result = S_OK;
		ID3D10Blob* err_msg = NULL;
		ID3D10Blob* vscode_buffer = NULL;
		
		// 编译shader代码
		result = D3DX11CompileFromFile( shader_file, NULL, NULL, main_func_name, profile
			, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
			&vscode_buffer, &err_msg, NULL);
		
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

			H3D_SAFE_RELEASE(err_msg);
			return false;
		}

		void* vscode_buf_pointer = vscode_buffer->GetBufferPointer();
		uint32_t vscode_buf_size = vscode_buffer->GetBufferSize();

		ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
		result = device->CreateVertexShader(
			vscode_buf_pointer, vscode_buf_size, NULL, &vertex_shader_);
		
		if(FAILED(result))
		{
			return false;
		}

		// 创建input layout对象
		result = device->CreateInputLayout( input_elem_desc , input_elem_num , 
			vscode_buf_pointer, vscode_buf_size, &input_layout_);

		if(FAILED(result))
		{
			return false;
		}

		H3D_SAFE_RELEASE(err_msg);
		H3D_SAFE_RELEASE(vscode_buffer);

		return true;
	}

	void H3DVertexShader::Set()
	{
		ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();
		
		// 设置顶点布局
		device_context->IASetInputLayout( input_layout_ );

		// 设置着色器
		device_context->VSSetShader( vertex_shader_ , NULL, 0 );
	}
}
