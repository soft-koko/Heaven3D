/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   2:02
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_vertex_buffer.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_vertex_buffer_h__
#define h3d_vertex_buffer_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DVertexBuffer
	{
	public:
		H3DVertexBuffer(void);
		~H3DVertexBuffer(void);

		bool Create(uint32_t vertex_count, uint32_t vertex_stride, uint32_t vertex_offset , 
			D3D11_USAGE usage ,  D3D_PRIMITIVE_TOPOLOGY primitive_topology , const void* vertex_data);

		H3D_INLINE uint32_t GetVertexOffset()
		{
			return vertex_offset_;
		}

		H3D_INLINE ID3D11Buffer* GetVertexBuffer()
		{
			return vertex_buffer_;
		}

		H3D_INLINE uint32_t GetVertexCount()
		{
			return vertex_count_;
		}

		H3D_INLINE uint32_t GetVertexStride()
		{
			return vertex_stride_;
		}

		H3D_INLINE D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology()
		{
			return primitive_topology_;
		}

		H3D_INLINE ID3D11Buffer* GetD3DBuffer()
		{
			return vertex_buffer_;
		}

		// 设置vb到device context中
		void Set();

		void Draw();

	protected:
		ID3D11Buffer*			vertex_buffer_;
		uint32_t				vertex_count_;		// 顶点的个数
		uint32_t				vertex_stride_;		// 每个顶点的大小
		uint32_t				vertex_offset_;
		D3D11_USAGE				usage_;
		D3D_PRIMITIVE_TOPOLOGY	primitive_topology_;	// primitive toplogy
	};
}

#endif // h3d_vertex_buffer_h__

