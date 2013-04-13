/********************************************************************
	created:	2012/04/07
	created:	7:4:2012   0:05
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_model_loader.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_model_loader_h__
#define h3d_model_loader_h__

namespace Heaven3D
{
	class H3DModelLoader
	{
	public:
		virtual bool LoadFromFile( const TCHAR* model_file_name ) = 0;
		virtual int32_t GetSubsetCount() = 0;

		// 获取顶点个数
		virtual uint32_t GetVertexCount() = 0;

		// 获取顶点索引个数
		virtual uint32_t GetIndexCount() = 0;

		virtual void ResetModelInfo() = 0;
	};
}
#endif // h3d_model_loader_h__
