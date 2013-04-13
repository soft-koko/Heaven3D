/********************************************************************
	created:	2012/03/06
	created:	6:3:2012   0:06
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_shader_effect.h
	author:		Xiong Xinke
	
	purpose:	封装了ps，vs和相关的常量buffer的类，类似于MS的effect文件
*********************************************************************/
#ifndef h3d_shader_effect_h__
#define h3d_shader_effect_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DPixelShader;
	class H3DVertexShader;
	class H3DString;

	class H3DShaderEffect
	{
	public:
		enum ShaderType
		{
			VERTEX_SHADER,
			PIXEL_SHADER,
			GEOMETRY_SHADER,
			COMPUTE_SHADER,
			HULL_SHADER
		};
		// 矩阵的类型，用来指定constant buffer，存一些做变换时要用到的外部参数
		enum ConstBufferType		
		{
			//--------------------------------- 顶点着色器用到的参数在这里写

			WORLD_MATRIX = 0,
			VIEW_MATRIX,
			PROJ_MATRIX,
			WORLD_VIEW_MATRIX,
			VIEW_PROJ_MATRIX,
			WVP_MATRIX,
			WORLD_MATRIX_ARRAY,
			BONE_INFLUENCE_COUNT,
			AMBIENT_MATERRIAL_COLOR,
			DIFFUSE_MATERRIAL_COLOR,
			EMISSIVE_MATERRIAL_COLOR,
			SPECULAR_MATERRIAL_COLOR,
			SPECULAR_MATERRIAL_POWER,
			POINT_LIGHT_POS0,
			POINT_LIGHT_VEC0,
			POINT_LIGHT_COLOR0,
			MAX_VS_CONST_BUFFER_COUNT,  // vertex shader用到的常量buffer类型都写在这个值的前面

			//--------------------------------- pixel shader用到的参数在这里写
			MAX_PS_CONST_BUFFER_COUNT,  // pixel shader用到的常量buffer类型都写在这个值的前面

			//--------------------------------- geometry shader用到的参数在这里写
			MAX_GS_CONST_BUFFER_COUNT,  // geometry shader用到的常量buffer类型都写在这个值的前面

			//--------------------------------- compute shader用到的参数在这里写
			MAX_CS_CONST_BUFFER_COUNT,  // compute shader用到的常量buffer类型都写在这个值的前面

			//--------------------------------- hull shader用到的参数在这里写
			MAX_HS_CONST_BUFFER_COUNT,  // hull shader用到的常量buffer类型都写在这个值的前面
			MAX_CONST_BUFFER_COUNT		// 这个值一定是会放在最后的
		};


	public:
		H3DShaderEffect(void);
		~H3DShaderEffect(void);

		bool CreateConstBuffer( ConstBufferType buf_type , const D3D11_BUFFER_DESC& buf_desc );

		H3D_INLINE H3DVertexShader* GetVertexShader()
		{
			return vertex_shader_;
		}

		H3D_INLINE H3DPixelShader* GetPixelShader()
		{
			return pixel_shader_;
		}

		ID3D11Buffer* GetConstantBuffer( ConstBufferType buf_type );

		bool SetShaderConstParam(ShaderType shader_type , ConstBufferType buf_type 
			, const void* param_data , const uint32_t param_data_size );

		// 从一个Microsoft Effect文件中创建出shader
		bool CreateFromEffectFile( const H3DString& effect_file );

		void CreateVerexShader();
		
		void CreatePixelShader();
		
	protected:
		H3DPixelShader*		pixel_shader_;
		H3DVertexShader*	vertex_shader_;
		ID3D11Buffer*		const_buffer_[MAX_CONST_BUFFER_COUNT];
	};
}

#endif // h3d_shader_effect_h__
