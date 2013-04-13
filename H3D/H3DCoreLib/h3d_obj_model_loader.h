/********************************************************************
	created:	2012/04/06
	created:	6:4:2012   1:18
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_obj_model_loader.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_obj_model_loader_h__
#define h3d_obj_model_loader_h__

#include "h3d_model_loader.h"
#include "h3d_vector2.h"
#include "h3d_vector3.h"
#include "h3d_vector4.h"
#include "h3d_string.h"
#include "h3d_texture.h"

namespace Heaven3D
{
	struct H3DObjModelVertex	//Overloaded Vertex Structure
	{
		H3DObjModelVertex(){}
		H3DObjModelVertex(	float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x,y,z), tex_coord(u, v), normal(nx, ny, nz){}
		H3DObjModelVertex(const H3DVector3& pos , 
			const H3DVector2& tex_coord , const H3DVector3& normal)
		{
			this->pos = pos;
			this->tex_coord = tex_coord;
			this->normal = normal;
		}

		H3DVector3 pos;
		H3DVector2 tex_coord;
		H3DVector3 normal;
	};

	struct H3DObjModelMaterial
	{
		H3DString	mat_name_;				// 材质的颜色
		H3DVector4	diffuse_color_;			// 漫反射颜色，如果有的话
		int32_t		texture_array_index_;
		bool		has_texture_;
		bool		transparent_;
	};
	/*
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },  
	{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	*/

	class H3DObjModelLoader : public H3DModelLoader
	{
	public:
		static D3D11_INPUT_ELEMENT_DESC obj_model_vertex_input_element[];
	public:
		H3DObjModelLoader();
		virtual ~H3DObjModelLoader();
		virtual bool LoadFromFile( const TCHAR* model_file_name );
		virtual int32_t GetSubsetCount();
		virtual uint32_t GetVertexCount();
		virtual uint32_t GetIndexCount();
		virtual void ResetModelInfo();

		H3D_INLINE const vector<H3DString>& GetTextureNameArray()
		{
			return texture_name_array_;
		}

		H3D_INLINE const vector<H3DObjModelMaterial>& GetMaterials()
		{
			return materials_;
		}

		H3D_INLINE const vector<int32_t>& GetSubsetMaterialArray()
		{
			return subset_material_array_;
		}

		H3D_INLINE const vector<int32_t>& GetSubsetIndexStart()
		{
			return subset_index_start_;
		}

		H3D_INLINE const vector<H3DObjModelVertex>& GetVertices()
		{
			return vertices_;
		}

		H3D_INLINE const vector<uint32_t>& GetIndices()
		{
			return indices_;
		}
	private:
		bool LoadVertex(H3DIFStream& input_file );
	private:
		vector<H3DObjModelVertex>		vertices_;
		vector<uint32_t>				indices_;
		vector<int32_t>					subset_index_start_;
		vector<int32_t>					subset_material_array_;
		vector<H3DObjModelMaterial>		materials_;
		vector<H3DString>				texture_name_array_;
		int32_t							subset_count_;			// 模型可划分为几个子集
		bool							righthand_coord_sys_;	// 模型是以右手坐标系存储的吗
		bool							compute_normals_;
	};
}

#endif // h3d_obj_model_loader_h__