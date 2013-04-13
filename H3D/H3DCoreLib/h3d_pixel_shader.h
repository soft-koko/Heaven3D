/********************************************************************
	created:	2012/03/05
	created:	5:3:2012   22:04
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_pixel_shader.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_pixel_shader_h__
#define h3d_pixel_shader_h__

#include "h3d_def.h"
//   类
namespace Heaven3D
{
	class H3DString;

	class H3DPixelShader
	{
	public:
		H3DPixelShader(void);
		~H3DPixelShader(void);

		bool Create(const TCHAR* shader_file , const TCHAR* main_func_name , const TCHAR* profile );

		H3D_INLINE ID3D11PixelShader* GetD3DPixelShader()
		{
			return pixel_shader_;
		}

		void Set();

	protected:
		ID3D11PixelShader*		pixel_shader_;
	};

}
#endif // h3d_pixel_shader_h__
