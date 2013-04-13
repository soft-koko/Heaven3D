/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   2:01
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_index_buffer.h
	author:		Xiong Xinke
	
	purpose:	索引缓冲区类
*********************************************************************/
#ifndef h3d_index_buffer_h__
#define h3d_index_buffer_h__

namespace Heaven3D
{
	class H3DIndexBuffer
	{
	public:
		H3DIndexBuffer(void);
		~H3DIndexBuffer(void);
		bool Create(uint32_t index_count , const uint32_t* indices_data , D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
	
		H3D_INLINE ID3D11Buffer* GetIndexBuffer()
		{
			return index_buffer_;
		}

		H3D_INLINE uint32_t GetIndexCount()
		{
			return index_count_;
		}

		void Set();

	protected:
		uint32_t		index_count_;
		D3D11_USAGE		usage_;
		ID3D11Buffer*	index_buffer_;
	};
}

#endif // h3d_index_buffer_h__

