/********************************************************************
	created:	2012/04/06
	created:	6:4:2012   0:09
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_3ds_model.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_3ds_model_h__
#define h3d_3ds_model_h__

#include "h3d_model.h"
#include "h3d_vector2.h"
#include "h3d_vector3.h"

namespace Heaven3D
{

#define MAX_3DS_MODEL_TEXTURES			100                // 最大的纹理数目
	// 基本块(Primary Chunk)，位于文件的开始
#define H3D_3DS_MODEL_PRIMARY			0x4D4D

	// 主块(Main Chunks)
#define H3D_3DS_MODEL_OBJECTINFO		0x3D3D        // 网格对象的版本号
#define H3D_3DS_MODEL_VERSION			0x0002        // .3ds文件的版本
#define H3D_3DS_MODEL_EDITKEYFRAME		0xB000        // 所有关键帧信息的头部

	// 对象的次级定义(包括对象的材质和对象）
#define H3D_3DS_MODEL_MATERIAL			0xAFFF        // 保存纹理信息
#define H3D_3DS_MODEL_OBJECT			0x4000        // 保存对象的面、顶点等信息

	// 材质的次级定义
#define H3D_3DS_MODEL_MATNAME			0xA000        // 保存材质名称
#define H3D_3DS_MODEL_MATDIFFUSE		0xA020        // 对象/材质的颜色
#define H3D_3DS_MODEL_MATMAP			0xA200        // 新材质的头部
#define H3D_3DS_MODEL_MATMAPFILE		0xA300        // 保存纹理的文件名

#define H3D_3DS_MODEL_OBJECT_MESH		0x4100        // 新的网格对象

	// H3D_3DS_MODEL_OBJECT_MESH的次级定义
#define H3D_3DS_MODEL_OBJECT_VERTICES	0x4110      // 对象顶点
#define H3D_3DS_MODEL_OBJECT_FACES		0x4120      // 对象的面
#define H3D_3DS_MODEL_OBJECT_MATERIAL   0x4130      // 对象的材质
#define H3D_3DS_MODEL_OBJECT_UV			0x4140      // 对象的UV纹理坐标

	// 面的结构定义
	struct H3D3dsModelFace
	{
		int32_t vert_index[3];      // 顶点索引
		int32_t coord_index[3];      // 纹理坐标索引
	};

	// 材质信息结构体
	struct H3D3dsModelMaterialInfo
	{
		char	texture_name_[255];      // 纹理名称
		char	texture_file_[255];      // 如果存在纹理映射，则表示纹理文件名称
		uint8_t color_[3];        // 对象的RGB颜色
		float	u_tile;        // u 重复
		float	v_tile;        // v 重复
		float	u_offset;       // u 纹理偏移
		float	v_offset;        // v 纹理偏移
	} ;

	// 对象信息结构体
	struct H3D3dsModelObject 
	{
		int32_t				verts_count_;      // 模型中顶点的数目
		int32_t				faces_count_;      // 模型中面的数目
		int32_t				texture_count_;      // 模型中纹理坐标的数目
		int32_t				material_id_;      // 纹理ID
		bool				has_texture_;      // 是否具有纹理映射
		char				texture_name_[255];      // 对象的名称
		H3DVector3*			verts_;      // 对象的顶点
		H3DVector3*			normals_;    // 对象的法向量
		H3DVector2*			tex_coord_;    // 纹理UV坐标
		H3D3dsModelFace*	faces_;        // 对象的面信息
	};

	// 模型信息结构体
	struct H3D3dsModel 
	{
		uint32_t							texture[MAX_3DS_MODEL_TEXTURES];
		int32_t								object_counts_;									// 模型中对象的数目
		int32_t								materail_counts_;									// 模型中材质的数目
		vector<H3D3dsModelMaterialInfo>		materails_;			// 材质链表信息
		vector<H3D3dsModelObject>			object;		// 模型中对象链表信息
	};

	struct H3D3dsModelIndex 
	{              
		unsigned short a, b, c, bVisible;  
	};

	// 保存块信息的结构
	struct H3D3dsModelChunk
	{
		uint16_t ID;          // 块的ID    
		uint32_t length;          // 块的长度
		uint32_t bytesRead;          // 需要读的块数据的字节数
	};

	// H3D3dsModelLoader类处理所有的装入代码
	class H3D3dsModelLoader
	{
	public:
		H3D3dsModelLoader();                // 初始化数据成员
		// 装入3ds文件到模型结构中
		bool LoadFile(H3D3dsModel *model, char *file_name);
		
	private:
		// 读一个字符串
		int32_t GetString(char *);
		// 读下一个块
		void ReadChunk(H3D3dsModelChunk *);
		// 读下一个块
		void ProcessNextChunk(H3D3dsModel *model, H3D3dsModelChunk *);
		// 读下一个对象块
		void ProcessNextObjectChunk(H3D3dsModel *model, H3D3dsModelObject *object, H3D3dsModelChunk *);
		// 读下一个材质块
		void ProcessNextMaterialChunk(H3D3dsModel *model, H3D3dsModelChunk *);
		// 读对象颜色的RGB值
		void ReadColorChunk(H3D3dsModelMaterialInfo *materail, H3D3dsModelChunk *chunk);
		// 读对象的顶点
		void ReadVertices(H3D3dsModelObject *object, H3D3dsModelChunk *);
		// 读对象的面信息
		void ReadVertexIndices(H3D3dsModelObject *object, H3D3dsModelChunk *);
		// 读对象的纹理坐标
		void ReadUVCoordinates(H3D3dsModelObject *object, H3D3dsModelChunk *);
		// 读赋予对象的材质名称
		void ReadObjectMaterial(H3D3dsModel *model, H3D3dsModelObject *object, H3D3dsModelChunk *previous_chunk);
		// 计算对象顶点的法向量
		void ComputeNormals(H3D3dsModel *model);
		// 关闭文件，释放内存空间
		void CleanUp();
	private:
		FILE*								file_pointer_;
		H3D3dsModelChunk*					current_chunk_;
		H3D3dsModelChunk*					temp_chunk_;
		uint32_t							texture[MAX_3DS_MODEL_TEXTURES];
		int32_t								object_counts_;				// 模型中对象的数目
		int32_t								materail_counts_;			// 模型中材质的数目
		vector<H3D3dsModelMaterialInfo>		materails_;					// 材质链表信息
		vector<H3D3dsModelObject>			object_;					// 模型中对象链表信息
	};
}

#endif // h3d_3ds_model_h__