/********************************************************************
	created:	2012/03/11
	created:	11:3:2012   1:52
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_constant_buffer.h
	author:		Xiong Xinke
	
	purpose:	常量缓冲区类
*********************************************************************/
#ifndef h3d_constant_buffer_h__
#define h3d_constant_buffer_h__

#include "h3d_def.h"

namespace Heaven3D
{
	class H3DConstantBuffer
	{
	public:
		H3DConstantBuffer(void);
		~H3DConstantBuffer(void);
		
		/** 
		* @brief GetD3DBuffer 
		* 
		* Detailed description.
		* @return		H3D_INLINE ID3D11Buffer*  
		*/
		H3D_INLINE ID3D11Buffer* GetD3DBuffer()
		{
			return d3d_const_buffer_;
		}

		/** 
		* @brief SetStartSlot 
		* 
		* Detailed description.
		* @param[in] start_slot 
		* @return		H3D_INLINE void  
		*/
		H3D_INLINE void SetStartSlot(uint32_t start_slot )
		{
			start_slot_ = start_slot;
		}
	
	public:
		/** 
		* @brief SetForVertexShader 
		* 
		* Detailed description. 把本const buffer设置给vertex shader使用
		* @return		void  
		*/
		void SetForVertexShader();

		/** 
		* @brief SetForPixelShader 
		* 
		* Detailed description. 把本const buffer设置给pixel shader使用
		* @return  void  
		*/
		void SetForPixelShader();

		/** 
		* @brief Create 
		* 
		* Detailed description.
		* @param[in] src_data 用以初始化填充const buffer的源数据首指针
		* @param[in] buf_size 用以初始化填充const buffer的源数据的大小
		* @param[in] buf_usage Defaults to D3D11_USAGE_DEFAULT.
		* @return	 bool  创建成功返回true，否则返回false
		*/
		bool Create( const void* src_data , uint32_t buf_size , D3D11_USAGE buf_usage = D3D11_USAGE_DEFAULT );

		/** 
		* @brief UpdateSubresource 
		* 
		* Detailed description.
		* @param[in] src_data 
		* @param[in] src_row_pitch Defaults to 0.
		* @param[in] src_depth_pitch Defaults to 0.
		* @return	 void  
		*/
		void UpdateSubresource(const void* src_data , uint32_t src_row_pitch = 0 ,  uint32_t src_depth_pitch = 0);

		/** 
		* @brief UpdateBufferData 
		* 
		* Detailed description.		
		* @param[in] src_data							用以更新填充const buffer的源数据首指针
		* @param[in] src_data_size						用以更新填充const buffer的源数据的大小
		* @param[in] sub_resource Defaults to 0.
		* @param[in] d3d_map Defaults to D3D11_MAP_WRITE_DISCARD.
		* @param[in] map_flag Defaults to 0.
		* @return    bool  
		*/
		bool UpdateBufferData(const void* src_data , uint32_t src_data_size , uint32_t sub_resource = 0,D3D11_MAP d3d_map = D3D11_MAP_WRITE_DISCARD , uint32_t map_flag = 0 );
	protected:
		ID3D11Buffer*	d3d_const_buffer_;
		uint32_t		start_slot_;
	};

}

#endif // h3d_constant_buffer_h__
