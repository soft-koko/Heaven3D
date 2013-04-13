/********************************************************************
	created:	2012/04/02
	created:	2:4:2012   22:21
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_model.h
	author:		Xiong Xinke
	
	purpose:	模型类的抽象基类
*********************************************************************/
#ifndef h3d_model_h__
#define h3d_model_h__

namespace Heaven3D
{
	class H3DModel
	{
	public:
		virtual bool CreateFromFile( const TCHAR* model_file_name ) = 0;
		virtual int32_t GetSubsetCount() = 0;
	};
}

#endif // h3d_model_h__
