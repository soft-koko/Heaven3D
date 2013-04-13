#ifndef h3d_mesh_container_h__
#define h3d_mesh_container_h__

#include "h3d_matrix.h"
#include "h3d_mesh.h"
//   类
namespace Heaven3D
{
	class H3DXModel
	{
	private:
		UINT m_NumOutstandingResources;
		bool m_bLoading;
		//BYTE*                         m_pBufferData;
		HANDLE m_hFile;
		HANDLE m_hFileMappingObject;
		vector<BYTE*> m_MappedPointers;
		ID3D11Device* m_pDev11;
		ID3D11DeviceContext* m_pDevContext11;

	protected:
		//These are the pointers to the two chunks of data loaded in from the mesh file
		BYTE* m_pStaticMeshData;
		BYTE* m_pHeapData;
		BYTE* m_pAnimationData;
		BYTE** m_ppVertices;
		BYTE** m_ppIndices;

		//Keep track of the path
		WCHAR                           m_strPathW[MAX_PATH];
		char                            m_strPath[MAX_PATH];

		//General mesh info
		SDKMESH_HEADER* m_pMeshHeader;
		SDKMESH_VERTEX_BUFFER_HEADER* m_pVertexBufferArray;
		SDKMESH_INDEX_BUFFER_HEADER* m_pIndexBufferArray;
		SDKMESH_MESH* m_pMeshArray;
		SDKMESH_SUBSET* m_pSubsetArray;
		SDKMESH_FRAME* m_pFrameArray;
		SDKMESH_MATERIAL* m_pMaterialArray;

		// Adjacency information (not part of the m_pStaticMeshData, so it must be created and destroyed separately )
		SDKMESH_INDEX_BUFFER_HEADER* m_pAdjacencyIndexBufferArray;

		//Animation (TODO: Add ability to load/track multiple animation sets)
		SDKANIMATION_FILE_HEADER* m_pAnimationHeader;
		SDKANIMATION_FRAME_DATA* m_pAnimationFrameData;
		H3DMatrix* m_pBindPoseFrameMatrices;
		H3DMatrix* m_pTransformedFrameMatrices;
		H3DMatrix* m_pWorldPoseFrameMatrices;

	protected:
		void LoadMaterials( ID3D11Device* pd3dDevice, SDKMESH_MATERIAL* pMaterials,
			UINT NumMaterials, SDKMESH_CALLBACKS11* pLoaderCallbacks=NULL );

		HRESULT CreateVertexBuffer( ID3D11Device* pd3dDevice,
			SDKMESH_VERTEX_BUFFER_HEADER* pHeader, void* pVertices,
			SDKMESH_CALLBACKS11* pLoaderCallbacks=NULL );

		HRESULT CreateIndexBuffer( ID3D11Device* pd3dDevice, SDKMESH_INDEX_BUFFER_HEADER* pHeader,
			void* pIndices, SDKMESH_CALLBACKS11* pLoaderCallbacks=NULL );

		//frame manipulation
		void                            TransformBindPoseFrame( UINT iFrame, H3DMatrix* pParentWorld );
		void                            TransformFrame( UINT iFrame, H3DMatrix* pParentWorld, double fTime );
		void                            TransformFrameAbsolute( UINT iFrame, double fTime );

		//Direct3D 11 rendering helpers
		void                            RenderMesh( UINT iMesh,
			bool bAdjacent,
			ID3D11DeviceContext* pd3dDeviceContext,
			UINT iDiffuseSlot,
			UINT iNormalSlot,
			UINT iSpecularSlot );
		void                            RenderFrame( UINT iFrame,
			bool bAdjacent,
			ID3D11DeviceContext* pd3dDeviceContext,
			UINT iDiffuseSlot,
			UINT iNormalSlot,
			UINT iSpecularSlot );

	public:
		H3DXModel();
		virtual                         ~H3DXModel();

		virtual HRESULT Create( ID3D11Device* pDev11, LPCTSTR szFileName, bool bCreateAdjacencyIndices=
			false, SDKMESH_CALLBACKS11* pLoaderCallbacks=NULL );

		virtual HRESULT Create( ID3D11Device* pDev11, BYTE* pData, UINT DataBytes,
			bool bCreateAdjacencyIndices=false, bool bCopyStatic=false,
			SDKMESH_CALLBACKS11* pLoaderCallbacks=NULL );

		virtual HRESULT LoadAnimation( WCHAR* szFileName );
		virtual void Destroy();

		//Frame manipulation
		void                            TransformBindPose( H3DMatrix* pWorld );
		void                            TransformMesh( H3DMatrix* pWorld, double fTime );


		//Direct3D 11 Rendering
		virtual void                    Render( ID3D11DeviceContext* pd3dDeviceContext,
			UINT iDiffuseSlot = INVALID_SAMPLER_SLOT,
			UINT iNormalSlot = INVALID_SAMPLER_SLOT,
			UINT iSpecularSlot = INVALID_SAMPLER_SLOT );
		virtual void                    RenderAdjacent( ID3D11DeviceContext* pd3dDeviceContext,
			UINT iDiffuseSlot = INVALID_SAMPLER_SLOT,
			UINT iNormalSlot = INVALID_SAMPLER_SLOT,
			UINT iSpecularSlot = INVALID_SAMPLER_SLOT );

		//Helpers (D3D11 specific)
		static D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType11( SDKMESH_PRIMITIVE_TYPE PrimType );
		DXGI_FORMAT                     GetIBFormat11( UINT iMesh );
		ID3D11Buffer* GetVB11( UINT iMesh, UINT iVB );
		ID3D11Buffer* GetIB11( UINT iMesh );
		SDKMESH_INDEX_TYPE GetIndexType( UINT iMesh ); 

		ID3D11Buffer* GetAdjIB11( UINT iMesh );


		//Helpers (general)
		char* GetMeshPathA();
		WCHAR* GetMeshPathW();
		UINT                            GetNumMeshes();
		UINT                            GetNumMaterials();
		UINT                            GetNumVBs();
		UINT                            GetNumIBs();

		ID3D11Buffer* GetVB11At( UINT iVB );
		ID3D11Buffer* GetIB11At( UINT iIB );

		BYTE* GetRawVerticesAt( UINT iVB );
		BYTE* GetRawIndicesAt( UINT iIB );
		SDKMESH_MATERIAL* GetMaterial( UINT iMaterial );
		SDKMESH_MESH* GetMesh( UINT iMesh );
		UINT                            GetNumSubsets( UINT iMesh );
		SDKMESH_SUBSET* GetSubset( UINT iMesh, UINT iSubset );
		UINT                            GetVertexStride( UINT iMesh, UINT iVB );
		UINT                            GetNumFrames();
		SDKMESH_FRAME*                  GetFrame( UINT iFrame );
		SDKMESH_FRAME*                  FindFrame( char* pszName );
		UINT64                          GetNumVertices( UINT iMesh, UINT iVB );
		UINT64                          GetNumIndices( UINT iMesh );
		H3DVector3                     GetMeshBBoxCenter( UINT iMesh );
		H3DVector3                     GetMeshBBoxExtents( UINT iMesh );
		UINT                            GetOutstandingResources();
		UINT                            GetOutstandingBufferResources();
		bool                            CheckLoadDone();
		bool                            IsLoaded();
		bool                            IsLoading();
		void                            SetLoading( bool bLoading );
		BOOL                            HadLoadingError();

		//Animation
		UINT                            GetNumInfluences( UINT iMesh );
		const H3DMatrix*               GetMeshInfluenceMatrix( UINT iMesh, UINT iInfluence );
		UINT                            GetAnimationKeyFromTime( double fTime );
		const H3DMatrix*               GetWorldMatrix( UINT iFrameIndex );
		const H3DMatrix*               GetInfluenceMatrix( UINT iFrameIndex );
		bool                            GetAnimationProperties( UINT* pNumKeys, FLOAT* pFrameTime );
	};
}
#endif // h3d_mesh_container_h__
