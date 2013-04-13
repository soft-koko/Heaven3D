#include "h3d_pch.h"
#include "h3d_md2_model.h"

namespace Heaven3D
{


	//H3DMD2Model::H3DMD2Model()
	//{
	//	index_list_ = NULL;
	//	frame_list_ = NULL;
	//	animation_speed_ = 5.f;
	//}

	//H3DMD2Model::~H3DMD2Model()
	//{
	//	H3D_SAFE_DELETE_ARRAY(index_list_);

	//	if( frame_list_ )
	//	{
	//		for( int i = 0 ; i < frames_ ; ++i )
	//		{
	//			H3D_SAFE_DELETE_ARRAY(frame_list_[i].vertex_list_);
	//		}

	//		H3D_SAFE_DELETE_ARRAY(frame_list_);
	//	}
	//}

	///**
	//* @name  H3DMD2Model::LoadFile
	//* @brief     载入MD2模型文件
	//* @return    int
	//* @param     const char* szFileName
	//* @remark    
	//**/
	//bool H3DMD2Model::CreateFromFile( const TCHAR* model_file_name )
	//{
	//	FILE*					md2_file = NULL;
	//	H3DMD2ModelFileHeader	md2_file_head;
	//	H3DVector2				md2_tex_coord[MD2_MAX_VERTS];
	//	char					md2_skins[MD2_MAX_SKINS][64];
	//	unsigned char			buffer[MD2_MAX_VERTS*4+128];

	//	md2_file = _tfopen( model_file_name , _T("rb"));

	//	if( NULL == md2_file )
	//	{
	//		MessageBox( 0 , _T("Can not open MD2 file"), _T("Error") , MB_OK|MB_ICONERROR );
	//		return 0;
	//	}

	//	  读取MD2文件头信息块
	//	fread( &md2_file_head, 1, sizeof(H3DMD2ModelFileHeader), md2_file );

	//	  要重新计算每帧数据的大小
	//	  md2_file_head.frame_size_ =
	//	  读取MD2文件的帧数，顶点数。还有三角形面片数
	//	frames_ = md2_file_head.frame_count_;
	//	vertex_count_ = md2_file_head.vertex_count_;
	//	triangle_count_ = md2_file_head.triangle_count_;
	//	skin_count_ = md2_file_head.skin_count_;
	//	uv_count_ = md2_file_head.uv_count_;
	//	  根据三角形的面数和帧数,创建
	//	index_list_ = new H3DMD2ModelIndexList[ triangle_count_ ];
	//	frame_list_ = new H3DMD2ModelFrameList[ frames_ ];

	//	  给每一帧都开辟顶点缓冲区
	//	for( int i = 0 ; i < frames_ ; ++i )
	//	{
	//		frame_list_[i].vertex_list_ = new H3DVector3[vertex_count_];
	//	}

	//	  读取skin数据和纹理映射坐标数据
	//	fread( md2_skins, 1, skin_count_ * MD2_MAX_SKINNAME, md2_file );
	//	fread( md2_tex_coord , 1, uv_count_ * sizeof(H3DVector2), md2_file );

	//	  根据三角形的面数读取三角形面片信息并填充
	//	H3DMD2ModelTriangle	triangle;

	//	  检测最大的纹理映射坐标，看看有没有超出的
	//	int max_tex_u , max_tex_v;
	//	for( i = 0 ; i < triangle_count_ ; ++i )
	//	{
	//		  读取三角形面片的顶点数据,并且填充三角形的顶点和纹理映射坐标的索引列表
	//		fread( &triangle, 1, sizeof(H3DMD2ModelTriangle), md2_file );

	//		  要注意磁盘的数据存储顺序和内存中的顺序的不同
	//		index_list_[i].a_ = triangle.index_c_;
	//		index_list_[i].b_ = triangle.index_b_;
	//		index_list_[i].c_ = triangle.index_a_;

	//		index_list_[i].ua_ = md2_tex_coord[triangle.index_c_uv_].x;
	//		index_list_[i].va_ = md2_tex_coord[triangle.index_c_uv_].y;
	//		index_list_[i].ub_ = md2_tex_coord[triangle.index_b_uv_].x;
	//		index_list_[i].vb_ = md2_tex_coord[triangle.index_b_uv_].y;
	//		index_list_[i].uc_ = md2_tex_coord[triangle.index_a_uv_].x;
	//		index_list_[i].vc_ = md2_tex_coord[triangle.index_a_uv_].y;

	//		max_tex_u = max( max_tex_u, md2_tex_coord[triangle.index_a_uv_].x );
	//		max_tex_u = max( max_tex_u, md2_tex_coord[triangle.index_b_uv_].x );
	//		max_tex_u = max( max_tex_u, md2_tex_coord[triangle.index_c_uv_].x );
	//		max_tex_v = max( max_tex_v, md2_tex_coord[triangle.index_a_uv_].y );
	//		max_tex_v = max( max_tex_v, md2_tex_coord[triangle.index_b_uv_].y );
	//		max_tex_v = max( max_tex_v, md2_tex_coord[triangle.index_c_uv_].y );
	//	}

	//	使纹理坐标有效
	//	for( i = 0 ; i < triangle_count_ ; ++i )
	//	{
	//		index_list_[i].ua_ /= max_tex_u;
	//		index_list_[i].ub_ /= max_tex_u;
	//		index_list_[i].uc_ /= max_tex_u;
	//		index_list_[i].va_ /= max_tex_v;
	//		index_list_[i].vb_ /= max_tex_v;
	//		index_list_[i].vc_ /= max_tex_v;
	//	}

	//	H3DMD2Animation	animation;
	//	string			last_anim_name;
	//	int				current_frame = 0;
	//	int				last_frame = 0;

	//	   读取所有动画的顶点数
	//	H3DMD2ModelAliasFrame* out = NULL;
	//	frame_size_ = md2_file_head.frame_size_;

	//	for( i = 0 ; i < frames_ ; ++i )
	//	{
	//		out = (H3DMD2ModelAliasFrame*)buffer;
	//		  根据每帧的大小读取相关的数据
	//		fread( out , 1 , frame_size_ , md2_file );

	//		string anim_name = out->name_;

	//		for( int j = 0 ; j < anim_name.length() ; ++j )
	//		{
	//			 如果索引j所指的字符为数字字符，并
	//			 且它的位置在字符串倒数第2个之后
	//			if( isdigit( anim_name[j]) && 
	//				j >= anim_name.length() - 2 )
	//			{
	//				anim_name.erase(j, anim_name.length() - j);
	//				break;
	//			}
	//		}

	//		 如果这动画名字和上一帧的名字不同的话，
	//		 或者我们在这个model的最后的一帧动画
	//		if( anim_name.compare(last_anim_name) || i == frames_ - 1)
	//		{
	//			  如果这一帧不是第一帧的话
	//			if(last_anim_name.compare(""))
	//			{
	//				  把上一帧的名字复制到本帧中
	//				animation.name_ = last_anim_name;

	//				 Set the last frame of this animation to i
	//				animation.end_ = i;
	//				animation.index_ = last_frame;
	//				current_frame = 0;
	//				last_frame++;
	//				animation_.push_back(animation);
	//			}

	//			  把当前获取的帧编号加1后作为动画的起始编号
	//			animation.start_ = current_frame + i;
	//			animation.current_ = animation.start_;
	//		}

	//		last_anim_name = anim_name;
	//		current_frame++;

	//		for( j = 0; j < vertex_count_; j++ ) 
	//		{
	//			frame_list_[i].vertex_list_[j].x = (out->triangle_vertex_[j]).x_ * out->scale_x_ + out->translate_x_;
	//			frame_list_[i].vertex_list_[j].y = (out->triangle_vertex_[j]).y_ * out->scale_y_ + out->translate_y_;
	//			frame_list_[i].vertex_list_[j].z = (out->triangle_vertex_[j]).z_ * out->scale_z_ + out->translate_z_;
	//		}
	//	}

	//	fclose( md2_file );
	//	return true;
	//	return Init();
	//}

	///**
	//* @name  H3DMD2Model::GetFrameCount
	//* @brief     
	//* @return    int 
	//* @remark    
	//**/
	//int H3DMD2Model::GetFrameCount()
	//{
	//	return frames_;
	//}

	///**
	//* @name  H3DMD2Model::GetVertexCount
	//* @brief     获取模型的顶点数
	//* @return    int 
	//* @remark    
	//**/
	//int H3DMD2Model::GetVertexCount()
	//{
	//	return vertex_count_;
	//}

	///**
	//* @name  H3DMD2Model::GetTriangleCount
	//* @brief     获取模型的三角形面片数
	//* @return    int 
	//* @remark    
	//**/
	//int H3DMD2Model::GetTriangleCount()
	//{
	//	return triangle_count_;
	//}


	///**
	//* @name  H3DMD2Model::Init
	//* @brief     把MD2的相关数据转用DX的数据结构存储
	//* @return    int 
	//* @remark    
	//**//*
	//int H3DMD2Model::Init()
	//{
	//把每一帧顶点等转换成D3D的mesh
	//for( int i = 0 ; i < GetFrameCount() ; ++i )
	//{
	//H3DMD2ModelVertex	model_vertex;
	//D3DXCOLOR		diffuse_color(1.0f,1.0f,1.0f,1.0f);//顶点漫反射的颜色

	//把每一帧的三角形的顶点都存储进自定义的mesh中去
	//for( int j = 0 ; j < GetTriangleCount() ; ++j )
	//{
	//注意。MD2文件的坐标轴和D3D的有所不同，所以要注意赋值的对应
	// D3D.X = MD2.X D3D.Y = MD2.Z D3D.Z = MD2.Y

	//存储三角形顶点A
	//model_vertex.pos_.x = 
	//frame_list_[i].vertex_list_[index_list_[j].a_].m_fX;
	//model_vertex.pos_.y = 
	//frame_list_[i].vertex_list_[index_list_[j].a_].m_fZ;
	//model_vertex.pos_.z = 
	//frame_list_[i].vertex_list_[index_list_[j].a_].m_fY;

	//model_vertex.tex_coord_.x = index_list_[j].ua_;
	//model_vertex.tex_coord_.y = index_list_[j].va_;

	//model_vertex.diffuse_color_ = diffuse_color;

	//meshes_[i].vertices_.push_back( model_vertex );
	//-------------------------------------------

	//存储三角形顶点B
	//model_vertex.pos_.x = 
	//frame_list_[i].vertex_list_[index_list_[j].b_].m_fX;
	//model_vertex.pos_.y = 
	//frame_list_[i].vertex_list_[index_list_[j].b_].m_fZ;
	//model_vertex.pos_.z = 
	//frame_list_[i].vertex_list_[index_list_[j].b_].m_fY;

	//model_vertex.tex_coord_.x = index_list_[j].ub_;
	//model_vertex.tex_coord_.y = index_list_[j].vb_;

	//model_vertex.diffuse_color_ = diffuse_color;

	//meshes_[i].vertices_.push_back( model_vertex );

	//----------------------------------------------------

	//存储三角形顶点C
	//model_vertex.pos_.x = 
	//frame_list_[i].vertex_list_[index_list_[j].c_].m_fX;
	//model_vertex.pos_.y = 
	//frame_list_[i].vertex_list_[index_list_[j].c_].m_fZ;
	//model_vertex.pos_.z = 
	//frame_list_[i].vertex_list_[index_list_[j].c_].m_fY;

	//model_vertex.tex_coord_.x = index_list_[j].uc_;
	//model_vertex.tex_coord_.y = index_list_[j].vc_;

	//model_vertex.diffuse_color_ = diffuse_color;

	//meshes_[i].vertices_.push_back( model_vertex );
	//}
	//}

	//return 1;
	//}
	//*/
	///**
	//* @name  H3DMD2Model::ReturnCurrentAnim
	//* @brief     得到当前要播放的动画编号
	//* @return    int 
	//* @param     int current
	//* @remark    
	//**/
	//int H3DMD2Model::ReturnCurrentAnim( int current )
	//{
	//	遍历所有动画的名字
	//	for (int i = 0 ; i < animation_.size() ; ++i )
	//	{
	//		if (animation_[i].index_ == current )
	//			return i;
	//	}

	//	return -1;	
	//}

	///**
	//* @name  H3DMD2Model::ReturnCurrentTime
	//* @brief     得到当前的时刻
	//* @return    float 
	//* @param     int select
	//* @param     int next_frame
	//* @remark    
	//**/
	//float H3DMD2Model::ReturnCurrentTime(int select,int next_frame)
	//{
	//	static float elapsed_time		= 0.0f;
	//	static float last_time	= 0.0f;

	//	获取当前的时刻
	//	float time = GetTickCount();

	//	elapsed_time = time - last_time;

	//	float t = elapsed_time / (1000.0f / animation_speed_ );

	//	到了换帧的时间
	//	if (elapsed_time >= (1000.0f / animation_speed_) )
	//	{
	//		切换到下一帧
	//		animation_[select].current_ = next_frame;
	//		last_time = time;
	//	}

	//	return t;
	//}

	///**
	//* @name  H3DMD2Model::Render
	//* @brief     渲染模型
	//* @return    bool 
	//* @param     int current
	//* @remark    
	//**/
	//void H3DMD2Model::Render( int current )
	//{
	//	int select = ReturnCurrentAnim( current );

	//	if( select == -1 ) 
	//		return select;

	//	int frame = animation_[select].current_;
	//	int next_frame = (animation_[select].current_ + 1) %
	//		animation_[select].end_;

	//	if( next_frame == 0 ) 
	//		next_frame =  animation_[select].start_;

	//	float t = ReturnCurrentTime( select , next_frame );

	//	int max_point = GetTriangleCount()*3;

	//	H3DMD2ModelVertex p1,p2;

	//	g_pD3DDevice->SetVertexShader( FVF_MD2_MODELVERTEX );

	//	for(int j = 0; j < max_point; j++)
	//	{
	//		p1 = meshes_[frame].vertices_[j];
	//		p2 = meshes_[next_frame].vertices_[j];

	//		p1.pos_ += (p2.pos_ - p1.pos_) * t;
	//		vertex_buffer_[j] = p1;
	//	}

	//	   指定顶点
	//	HRESULT hr = g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
	//		GetTriangleCount(),		 
	//		(BYTE**)&vertex_buffer_[0],
	//		sizeof(H3DMD2ModelVertex));

	//	return (SUCCEEDED(hr)); 
	//}
}