/********************************************************************
	created:	2012/03/05
	created:	5:3:2012   21:58
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_vertex_shader.h
	author:		Xiong Xinke
	
	purpose:	顶点着色器类
*********************************************************************/
#ifndef h3d_vertex_shader_h__
#define h3d_vertex_shader_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DString;

	class H3DVertexShader
	{
	public:
		H3DVertexShader(void);
		~H3DVertexShader(void);

		bool Create(const TCHAR* shader_file , const TCHAR* main_func_name , const TCHAR* profile ,
			const D3D11_INPUT_ELEMENT_DESC* input_elem_desc , uint32_t input_elem_num );

		H3D_INLINE ID3D11VertexShader* GetD3DVertexShader()
		{
			return vertex_shader_;
		}

		H3D_INLINE ID3D11InputLayout* GetD3DInputLayout()
		{
			return input_layout_;
		}

		void Set();

	protected:
		ID3D11VertexShader*		vertex_shader_;
		ID3D11InputLayout*		input_layout_;
	};
}

#endif // h3d_vertex_shader_h__

