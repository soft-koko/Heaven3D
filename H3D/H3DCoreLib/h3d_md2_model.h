/********************************************************************
	created:	2012/04/04
	created:	4:4:2012   15:39
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_md2_model.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_md2_model_h__
#define h3d_md2_model_h__

#include "h3d_def.h"
#include "h3d_model.h"

#define MD2_MAX_FRAMES		512		// 一个MD2文件中最大的帧数
#define MD2_MAX_TRIANGLES	8192	// 一个MD2文件中最大的三角形数
#define MD2_MAX_SKINS		32
#define MD2_MAX_SKINNAME	64
#define MD2_MAX_VERTS		4096

namespace Heaven3D
{
//	//MD2模型的每个三角形的顶点和纹理映射坐标的索引列表
//	struct H3DMD2ModelIndexList
//	{
//		int32_t	a_ , b_, c_;
//		float	ua_ , va_;
//		float	ub_ , vb_;
//		float	uc_ , vc_;
//	};
//
//	//MD2模型的帧列表,每一帧指向一个顶点列表
//	struct H3DMD2ModelFrameList
//	{
//		H3DVector3* vertex_list_;
//	};
//
//	//MD2模型中的三角形面片，用来保持这
//	//些顶点和纹理映射坐标在文件中索引
//	struct H3DMD2ModelTriangle
//	{
//		int16_t index_a_;
//		int16_t index_b_;
//		int16_t index_c_;
//		int16_t index_a_uv_;
//		int16_t index_b_uv_;
//		int16_t index_c_uv_;
//	};
//
//	//MD2模型中的三角形的顶点，其坐标是单字节的
//	struct H3DMD2ModelTriVertex
//	{
//		uint8_t x_ , y_ , z_;
//		uint8_t light_normal_index_;
//	};
//
//	//MD2用来保持动画帧的缩放(scale)，
//	//平移(translate)和缩放等信息
//	struct H3DMD2ModelAliasFrame
//	{
//		float					scale_x_;
//		float					scale_y_;
//		float					scale_z_;
//		float					translate_x_;
//		float					translate_y_;
//		float					translate_z_;
//		char					name_[16];
//		H3DMD2ModelTriVertex	triangle_vertex_[1];
//	};
//
//	//MD2的文件头数据，这个数据结构的成员
//	//顺序是要严格定义不能调乱的
//	struct H3DMD2ModelFileHeader
//	{
//		int32_t id_;		//识别号
//		int32_t version_;			//版本号
//		int32_t skin_width_;		//
//		int32_t skin_height_;		//
//		int32_t frame_size_;		//每帧的大小，以字节为单位
//		int32_t skin_count_;			//与模型相关联的皮肤数量
//		int32_t vertex_count_;		//模型的顶点数
//		int32_t uv_count_;			//纹理坐标的数量
//		int32_t triangle_count_;		//模型中三角形面片的数量
//		int32_t opengl_command_count_;	//OpenGL的命令数目
//		int32_t frame_count_;		//帧数目
//		int32_t skin_offset_;		//皮肤数据在文件中的偏移地址
//		int32_t texcoord_offset_;	//纹理映射坐标数据在文件中的偏移量
//		int32_t triangle_offset_; //三角形面片数据在文件中的偏移量
//		int32_t frame_offset_;     //帧数据在文件中的偏移量
//		int32_t opengl_command_offset_; //OpenGL命令在文件中的偏移量
//		int32_t end_offset_;		//文件结束位置在文件中的偏移量
//	};
//
//	struct H3DMD2ModelTriFloatVtx
//	{
//		H3DVector3	vtx_;
//		int			light_normal_index_;
//	};
//
//	struct H3DMD2ModelFrame
//	{
//		H3DVector3				min_;
//		H3DVector3				max_;
//		char					name_[16];
//		H3DMD2ModelTriFloatVtx	vtx_;
//	};
//
//	//MD2的FVF顶点模型
//	struct H3DMD2ModelVertex 
//	{
//		H3DVector3		pos_;
//		uint32_t		diffuse_color_;
//		H3DVector2		tex_coord_;
//	} ;
//
////#define FVF_MD2_MODELVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0) )
//
//	struct H3DMD2Mesh
//	{
//		vector<H3DMD2ModelVertex>	vertices_;
//	};
//
//	//MD2的动画帧
//	struct H3DMD2Animation
//	{
//		int32_t		start_;		//MD2动画的起始帧
//		int32_t		end_;			//MD2动画的结束帧
//		int32_t		index_;		//
//		int32_t		current_;		//
//		string	name_;
//	};
//
//
//	class H3DMD2Model : public H3DModel
//	{
//	private:
//		H3DMD2ModelIndexList*	index_list_;						 //  顶点索引
//		H3DMD2ModelFrameList*	frame_list_;						 //  帧索引列表
//		int32_t					frames_;							 //  帧数
//		int32_t					vertex_count_;						 //  每帧的顶点数
//		int32_t					triangle_count_;					 //  三角形面片数
//		int32_t					skin_count_;						 //  皮肤数
//		int32_t					uv_count_;							 //  纹理坐标数
//		int32_t					frame_size_;						 //  帧大小
//		H3DMD2Mesh				meshes_[MD2_MAX_FRAMES];			 //  每一个就对应一个mesh
//		H3DMD2ModelVertex		vertex_buffer_[MD2_MAX_TRIANGLES*3]; //  模型的顶点数
//		float					animation_speed_;					 //  动画的播放速度
//		vector<H3DMD2Animation>	animation_;
//	public:
//		H3DMD2Model();
//		~H3DMD2Model();
//		virtual bool CreateFromFile( const TCHAR* model_file_name );
//		virtual int32_t GetSubsetCount();
//		int32_t   ReturnCurrentAnim( int current );
//		float	  ReturnCurrentTime(int select,int next_frame);
//		int32_t	  Render( int current );
//		
//		H3D_INLINE int32_t GetFrameCount()
//		{
//			return frames_;
//		}
//
//		H3D_INLINE int32_t GetVertexCount()
//		{
//			return vertex_count_;
//		}
//
//		H3D_INLINE int32_t GetTriangleCount()
//		{
//			return triangle_count_;
//		}
//	};
}

#endif // h3d_md2_model_h__