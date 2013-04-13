/********************************************************************
created:	2012/02/29
created:	29:2:2012   1:28
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_mesh.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_mesh_h__
#define h3d_mesh_h__

#include "h3d_vector3.h"
#include "h3d_vector4.h"
#include "h3d_matrix.h"
//   类
namespace Heaven3D
{

#define SDKMESH_FILE_VERSION 101
#define MAX_VERTEX_ELEMENTS 32
#define MAX_VERTEX_STREAMS 16
#define MAX_FRAME_NAME 100
#define MAX_MESH_NAME 100
#define MAX_SUBSET_NAME 100
#define MAX_MATERIAL_NAME 100
#define MAX_TEXTURE_NAME MAX_PATH
#define MAX_MATERIAL_PATH MAX_PATH
#define INVALID_FRAME ((UINT)-1)
#define INVALID_MESH ((UINT)-1)
#define INVALID_MATERIAL ((UINT)-1)
#define INVALID_SUBSET ((UINT)-1)
#define INVALID_ANIMATION_DATA ((UINT)-1)
#define INVALID_SAMPLER_SLOT ((UINT)-1)
#define ERROR_RESOURCE_VALUE 1

	template<typename TYPE> BOOL IsErrorResource( TYPE data )
	{
		if( ( TYPE )ERROR_RESOURCE_VALUE == data )
			return TRUE;
		return FALSE;
	}
	//--------------------------------------------------------------------------------------
	// Enumerated Types.  These will have mirrors in both D3D9 and D3D11
	//--------------------------------------------------------------------------------------
	enum SDKMESH_PRIMITIVE_TYPE
	{
		PT_TRIANGLE_LIST = 0,
		PT_TRIANGLE_STRIP,
		PT_LINE_LIST,
		PT_LINE_STRIP,
		PT_POINT_LIST,
		PT_TRIANGLE_LIST_ADJ,
		PT_TRIANGLE_STRIP_ADJ,
		PT_LINE_LIST_ADJ,
		PT_LINE_STRIP_ADJ,
		PT_QUAD_PATCH_LIST,
		PT_TRIANGLE_PATCH_LIST,
	};

	enum SDKMESH_INDEX_TYPE
	{
		IT_16BIT = 0,
		IT_32BIT,
	};

	enum FRAME_TRANSFORM_TYPE
	{
		FTT_RELATIVE = 0,
		FTT_ABSOLUTE,		//This is not currently used but is here to support absolute transformations in the future
	};

	//--------------------------------------------------------------------------------------
	// Structures.  Unions with pointers are forced to 64bit.
	//--------------------------------------------------------------------------------------
	struct SDKMESH_HEADER
	{
		//Basic Info and sizes
		UINT Version;
		BYTE IsBigEndian;
		UINT64 HeaderSize;
		UINT64 NonBufferDataSize;
		UINT64 BufferDataSize;

		//Stats
		UINT NumVertexBuffers;
		UINT NumIndexBuffers;
		UINT NumMeshes;
		UINT NumTotalSubsets;
		UINT NumFrames;
		UINT NumMaterials;

		//Offsets to Data
		UINT64 VertexStreamHeadersOffset;
		UINT64 IndexStreamHeadersOffset;
		UINT64 MeshDataOffset;
		UINT64 SubsetDataOffset;
		UINT64 FrameDataOffset;
		UINT64 MaterialDataOffset;
	};

	struct SDKMESH_VERTEX_BUFFER_HEADER
	{
		UINT64 NumVertices;
		UINT64 SizeBytes;
		UINT64 StrideBytes;
		
		union
		{
			UINT64 DataOffset;				//(This also forces the union to 64bits)
			ID3D11Buffer* pVB11;
		};
	};

	struct SDKMESH_INDEX_BUFFER_HEADER
	{
		UINT64 NumIndices;
		UINT64 SizeBytes;
		UINT IndexType;
		union
		{
			UINT64 DataOffset;				//(This also forces the union to 64bits)
			ID3D11Buffer* pIB11;
		};
	};

	struct SDKMESH_MESH
	{
		char Name[MAX_MESH_NAME];
		BYTE NumVertexBuffers;
		UINT VertexBuffers[MAX_VERTEX_STREAMS];
		UINT IndexBuffer;
		UINT NumSubsets;
		UINT NumFrameInfluences; //aka bones

		H3DVector3 BoundingBoxCenter;
		H3DVector3 BoundingBoxExtents;

		union
		{
			UINT64 SubsetOffset;	//Offset to list of subsets (This also forces the union to 64bits)
			UINT* pSubsets;	    //Pointer to list of subsets
		};
		union
		{
			UINT64 FrameInfluenceOffset;  //Offset to list of frame influences (This also forces the union to 64bits)
			UINT* pFrameInfluences;      //Pointer to list of frame influences
		};
	};

	struct SDKMESH_SUBSET
	{
		char Name[MAX_SUBSET_NAME];
		UINT MaterialID;
		UINT PrimitiveType;
		UINT64 IndexStart;
		UINT64 IndexCount;
		UINT64 VertexStart;
		UINT64 VertexCount;
	};

	struct SDKMESH_FRAME
	{
		char Name[MAX_FRAME_NAME];
		UINT Mesh;
		UINT ParentFrame;
		UINT ChildFrame;
		UINT SiblingFrame;
		H3DMatrix Matrix;
		UINT AnimationDataIndex;		//Used to index which set of keyframes transforms this frame
	};

	struct SDKMESH_MATERIAL
	{
		char    Name[MAX_MATERIAL_NAME];

		// Use MaterialInstancePath
		char    MaterialInstancePath[MAX_MATERIAL_PATH];

		// Or fall back to d3d8-type materials
		char    DiffuseTexture[MAX_TEXTURE_NAME];
		char    NormalTexture[MAX_TEXTURE_NAME];
		char    SpecularTexture[MAX_TEXTURE_NAME];

		H3DVector4 Diffuse;
		H3DVector4 Ambient;
		H3DVector4 Specular;
		H3DVector4 Emissive;
		FLOAT Power;

		union
		{
			UINT64 Force64_1;			//Force the union to 64bits
			ID3D11Texture2D* pDiffuseTexture11;
		};
		union
		{
			UINT64 Force64_2;			//Force the union to 64bits
			ID3D11Texture2D* pNormalTexture11;
		};
		union
		{
			UINT64 Force64_3;			//Force the union to 64bits
			ID3D11Texture2D* pSpecularTexture11;
		};

		union
		{
			UINT64 Force64_4;			//Force the union to 64bits
			ID3D11ShaderResourceView* pDiffuseRV11;
		};
		union
		{
			UINT64 Force64_5;		    //Force the union to 64bits
			ID3D11ShaderResourceView* pNormalRV11;
		};
		union
		{
			UINT64 Force64_6;			//Force the union to 64bits
			ID3D11ShaderResourceView* pSpecularRV11;
		};

	};

	struct SDKANIMATION_FILE_HEADER
	{
		UINT Version;
		BYTE IsBigEndian;
		UINT FrameTransformType;
		UINT NumFrames;
		UINT NumAnimationKeys;
		UINT AnimationFPS;
		UINT64 AnimationDataSize;
		UINT64 AnimationDataOffset;
	};

	struct SDKANIMATION_DATA
	{
		H3DVector3 Translation;
		H3DVector4 Orientation;
		H3DVector3 Scaling;
	};

	struct SDKANIMATION_FRAME_DATA
	{
		char FrameName[MAX_FRAME_NAME];
		union
		{
			UINT64 DataOffset;
			SDKANIMATION_DATA* pAnimationData;
		};
	};

	typedef void ( CALLBACK*LPCREATETEXTUREFROMFILE11 )( ID3D11Device* pDev, char* szFileName,
		ID3D11ShaderResourceView** ppRV, void* pContext );
	typedef void ( CALLBACK*LPCREATEVERTEXBUFFER11 )( ID3D11Device* pDev, ID3D11Buffer** ppBuffer,
		D3D11_BUFFER_DESC BufferDesc, void* pData, void* pContext );
	typedef void ( CALLBACK*LPCREATEINDEXBUFFER11 )( ID3D11Device* pDev, ID3D11Buffer** ppBuffer,
		D3D11_BUFFER_DESC BufferDesc, void* pData, void* pContext );
	
	struct SDKMESH_CALLBACKS11
	{
		LPCREATETEXTUREFROMFILE11 pCreateTextureFromFile;
		LPCREATEVERTEXBUFFER11 pCreateVertexBuffer;
		LPCREATEINDEXBUFFER11 pCreateIndexBuffer;
		void* pContext;
	};

	
}
#endif // h3d_mesh_h__
