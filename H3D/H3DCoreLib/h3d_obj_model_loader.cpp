/********************************************************************
created:	2012/04/06
created:	6:4:2012   1:18
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_obj_model_loader.cpp
author:		Xiong Xinke

purpose:	
*********************************************************************/
#include "h3d_pch.h"
#include "h3d_obj_model_loader.h"

namespace Heaven3D
{
	H3DObjModelLoader::H3DObjModelLoader()
	{
		ResetModelInfo();
	}

	void H3DObjModelLoader::ResetModelInfo()
	{
		righthand_coord_sys_ = false;
		subset_count_ = 0;
		vertices_.clear();
		indices_.clear();
	}

	H3DObjModelLoader::~H3DObjModelLoader()
	{

	}

	uint32_t H3DObjModelLoader::GetVertexCount()
	{
		return (uint32_t)vertices_.size();
	}

	uint32_t H3DObjModelLoader::GetIndexCount()
	{
		return (uint32_t)indices_.size();
	}

	int32_t H3DObjModelLoader::GetSubsetCount()
	{
		return subset_count_;
	}

	bool H3DObjModelLoader::LoadFromFile( const TCHAR* model_file_name )
	{
		ResetModelInfo();
		HRESULT hr = 0;

		H3DIFStream					input_file(model_file_name);	
		H3DString					mesh_mat_lib;	// 模型所使用到的材质库的名字

		//std::vector<uint32_t>		indices;
		std::vector<H3DVector3>		vertex_pos;
		std::vector<H3DVector3>		vertex_normal;
		std::vector<H3DVector2>		vertex_tex_coord;
		std::vector<H3DString>		mesh_materials;
		std::vector<int32_t>		vertex_pos_index;
		std::vector<int32_t>		vertex_normal_index;
		std::vector<int32_t>		vertex_texcoord_index;

		bool has_tex_coord = false;
		bool has_normal = false;

		H3DString mesh_material_temp;
		int vertex_pos_index_temp;
		int vertex_normal_index_temp;
		int vertex_texcoord_index_temp;

		TCHAR check_char;				// 用来检测文件中的字节流的变量
		H3DString face;					// Holds the string containing our face vertices
		int vertex_index = 0;			// 记录顶点索引个数
		int triangle_count = 0;			// 总的三角形个数
		uint32_t total_vertics = 0;
		int mesh_triangles = 0;

		// 如果文件不打开就报错返回
		if( !input_file.is_open() )
		{
			H3DString message = _T("Could not open: ");
			message += model_file_name;
			::MessageBox(0, message.c_str(),_T("Error"), MB_OK);
			return false;
		}

		while(input_file.peek() != EOF )
		{			
			check_char = input_file.get();	//Get next char
			switch(check_char)
			{		
			case _T('#'):
				{
					check_char = input_file.get();
					while(check_char != _T('\n'))
						check_char = input_file.get();
				}
				break;
			case _T('v'):	// 获取到顶点描述
				{
					check_char = input_file.get();

					if(check_char == _T(' '))	//v -顶点位置
					{
						float vz, vy, vx;
						input_file >> vx >> vy >> vz;
						//如果模型使用右手坐标系就反转Z轴
						vertex_pos.push_back( righthand_coord_sys_ ?H3DVector3( vx, vy, vz * -1.0f) : H3DVector3( vx, vy, vz) );
					}

					if(check_char == _T('t'))	//vt - 顶点纹理坐标
					{			
						float vtcu, vtcv;
						input_file >> vtcu >> vtcv;	
						//  如果模型使用右手坐标系就反转纹理的v轴	
						vertex_tex_coord.push_back( righthand_coord_sys_ ?H3DVector2(vtcu, 1.0f-vtcv) : H3DVector2(vtcu, vtcv));
						has_tex_coord = true;
					}

					if(check_char == _T('n'))	//vn - 顶点法线
					{
						float vnx, vny, vnz;
						input_file >> vnx >> vny >> vnz;
						vertex_normal.push_back( righthand_coord_sys_ ? H3DVector3( vnx, vny, vnz * -1.0f ) : H3DVector3( vnx, vny, vnz ));	// 如果模型使用右手坐标系就反转Z轴
						has_normal = true;
					}
				}
				break;
			case _T('g'):	//g - 模型分块
				{
					check_char = input_file.get();
					if(check_char == _T(' '))
					{
						subset_index_start_.push_back(vertex_index);		//Start index for this subset
						subset_count_++;
					}
				}
				break;
			case _T('f'):	//f - 获取到面的索引值
				{
					check_char = input_file.get();
					if( check_char != ' ' )
						break;

					face.clear();
					H3DString vertex_define;	// 暂存顶点描述字符串的string
					triangle_count = 0;
					check_char = input_file.get();

					while(check_char != _T('\n'))
					{
						face += check_char;				 
						check_char = input_file.get();
						if(check_char == _T(' '))		// 碰到空格了，就表示描述一个三角形完成
							triangle_count++;			// 三角形数量加1
					}

					// 最后一个字符为空格的话，三角形数量需要减去1
					if( face[face.length()-1] == _T(' '))
						triangle_count--;				//Each space adds to our triangle count

					triangle_count -= 1;		//Ever vertex in the face AFTER the first two are new faces
					H3DStringStream ss(face);

					if(face.length() <= 0)		// 如果描述一个面的字符串存在，表示面存在
						break;

					int first_vertex_index, last_vertex_index;	//Holds the first and last vertice's index

					for(int i = 0; i < 3; ++i)		//First three vertices (first triangle)
					{
						ss >> vertex_define;		// 读入顶点的信息（位置，纹理坐标，法线）Get vertex definition (vPos/vTexCoord/vNorm)

						H3DString vertex_part;
						int which_part = 0;		//(vPos, vTexCoord, or vNorm)

						// 分析包含了位置，纹理坐标，法线信息的顶点信息
						for( size_t j = 0; j < vertex_define.length(); ++j)
						{
							if( vertex_define.GetChar(j) != _T('/'))	//If there is no divider "/", add a char to our vertex_part
								vertex_part += vertex_define.GetChar(j);

							//If the current char is a divider "/", or its the last character in the string
							if(vertex_define.GetChar(j) == _T('/') || j ==  vertex_define.length()-1)
							{
								H3DIStringStream string_to_int(vertex_part);	//Used to convert wstring to int

								if(which_part == 0)	//If vPos
								{
									string_to_int >> vertex_pos_index_temp;
									vertex_pos_index_temp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1

									//Check to see if the vert pos was the only thing specified
									if(j == vertex_define.length()-1)
									{
										vertex_normal_index_temp = 0;
										vertex_texcoord_index_temp = 0;
									}
								}

								else if(which_part == 1)	//If vTexCoord
								{
									if( !vertex_part.empty() )	//Check to see if there even is a tex coord
									{
										string_to_int >> vertex_texcoord_index_temp;
										vertex_texcoord_index_temp -= 1;	//subtract one since c++ arrays start with 0, and obj start with 1
									}
									else	//If there is no tex coord, make a default
										vertex_texcoord_index_temp = 0;

									//If the cur. char is the second to last in the string, then
									//there must be no normal, so set a default normal
									if(j == vertex_define.length()-1)
										vertex_normal_index_temp = 0;

								}								
								else if(which_part == 2)	//If vNorm
								{
									H3DIStringStream string_to_int(vertex_part);

									string_to_int >> vertex_normal_index_temp;
									vertex_normal_index_temp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1
								}

								vertex_part = _T("");	//Get ready for next vertex part
								which_part++;	//Move on to next vertex part					
							}
						}

						//Check to make sure there is at least one subset
						if(subset_count_ == 0)
						{
							subset_index_start_.push_back(vertex_index);		//Start index for this subset
							subset_count_++;
						}

						//Avoid duplicate vertices
						bool vertex_already_exist = false;
						if(total_vertics >= 3)	//Make sure we at least have one triangle to check
						{
							//Loop through all the vertices
							for( uint32_t check_index = 0; check_index < total_vertics; ++check_index)
							{
								//If the vertex position and texture coordinate in memory are the same
								//As the vertex position and texture coordinate we just now got out
								//of the obj file, we will set this faces vertex index to the vertex's
								//index value in memory. This makes sure we don't create duplicate vertices
								if(vertex_pos_index_temp == vertex_pos_index[check_index] && !vertex_already_exist)
								{
									if(vertex_texcoord_index_temp == vertex_texcoord_index[check_index])
									{
										indices_.push_back(check_index);		//Set index for this vertex
										vertex_already_exist = true;		//If we've made it here, the vertex already exists
									}
								}
							}
						}

						//If this vertex is not already in our vertex arrays, put it there
						if(!vertex_already_exist)
						{
							vertex_pos_index.push_back(vertex_pos_index_temp);
							vertex_texcoord_index.push_back(vertex_texcoord_index_temp);
							vertex_normal_index.push_back(vertex_normal_index_temp);
							total_vertics++;	//We created a new vertex
							indices_.push_back(total_vertics-1);	//Set index for this vertex
						}							

						//If this is the very first vertex in the face, we need to
						//make sure the rest of the triangles use this vertex
						if(i == 0)
						{
							first_vertex_index = indices_[vertex_index];	//The first vertex index of this FACE
						}

						//If this was the last vertex in the first triangle, we will make sure
						//the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
						if(i == 2)
						{								
							last_vertex_index = indices_[vertex_index];	//The last vertex index of this TRIANGLE
						}
						vertex_index++;	//Increment index count
					}

					mesh_triangles++;	//One triangle down

					//If there are more than three vertices in the face definition, we need to make sure
					//we convert the face to triangles. We created our first triangle above, now we will
					//create a new triangle for every new vertex in the face, using the very first vertex
					//of the face, and the last vertex from the triangle before the current triangle
					for(int l = 0; l < triangle_count-1; ++l)	//Loop through the next vertices to create new triangles
					{
						//First vertex of this triangle (the very first vertex of the face too)
						indices_.push_back(first_vertex_index);			//Set index for this vertex
						vertex_index++;

						//Second Vertex of this triangle (the last vertex used in the tri before this one)
						indices_.push_back(last_vertex_index);			//Set index for this vertex
						vertex_index++;

						//Get the third vertex for this triangle
						ss >> vertex_define;

						H3DString vertex_part;
						int which_part = 0;

						//Parse this string (same as above)
						for(size_t j = 0; j < vertex_define.length(); ++j)
						{
							if(  _T('/') != vertex_define.GetChar(j) )
								vertex_part +=vertex_define.GetChar(j);

							if( vertex_define.GetChar(j) == _T('/') || j ==  vertex_define.length()-1)
							{
								H3DIStringStream string_to_int(vertex_part);

								if(which_part == 0)
								{
									string_to_int >> vertex_pos_index_temp;
									vertex_pos_index_temp -= 1;

									//Check to see if the vert pos was the only thing specified
									if(j == vertex_define.length()-1)
									{
										vertex_texcoord_index_temp = 0;
										vertex_normal_index_temp = 0;
									}
								}
								else if(which_part == 1)
								{
									if( !vertex_part.empty() )
									{
										string_to_int >> vertex_texcoord_index_temp;
										vertex_texcoord_index_temp -= 1;
									}
									else
										vertex_texcoord_index_temp = 0;
									if(j == vertex_define.length()-1)
										vertex_normal_index_temp = 0;

								}								
								else if(which_part == 2)
								{
									H3DIStringStream string_to_int(vertex_part);
									string_to_int >> vertex_normal_index_temp;
									vertex_normal_index_temp -= 1;
								}

								vertex_part = _T("");
								which_part++;							
							}
						}					

						//Check for duplicate vertices
						bool vertex_already_exist = false;
						if(total_vertics >= 3)	//Make sure we at least have one triangle to check
						{
							for(uint32_t check_index = 0; check_index < total_vertics; ++check_index)
							{
								if(vertex_pos_index_temp == vertex_pos_index[check_index] && !vertex_already_exist)
								{
									if(vertex_texcoord_index_temp == vertex_texcoord_index[check_index])
									{
										indices_.push_back(check_index);			//Set index for this vertex
										vertex_already_exist = true;		//If we've made it here, the vertex already exists
									}
								}
							}
						}

						if(!vertex_already_exist)
						{
							vertex_pos_index.push_back(vertex_pos_index_temp);
							vertex_texcoord_index.push_back(vertex_texcoord_index_temp);
							vertex_normal_index.push_back(vertex_normal_index_temp);
							total_vertics++;					//New vertex created, add to total verts
							indices_.push_back(total_vertics-1);		//Set index for this vertex
						}

						//Set the second vertex for the next triangle to the last vertex we got		
						last_vertex_index = indices_[vertex_index];	//The last vertex index of this TRIANGLE

						mesh_triangles++;	//New triangle defined
						vertex_index++;		
					}


				}
				break;

			case _T('m'):	//mtllib - material library filename
				{
					check_char = input_file.get();
					if(check_char != _T('t'))
						break;

					check_char = input_file.get();
					if(check_char != _T('l'))
						break;

					check_char = input_file.get();
					if(check_char != _T('l'))
						break;

					check_char = input_file.get();
					if(check_char != _T('i'))
						break;

					check_char = input_file.get();
					if(check_char != _T('b'))
						break;

					check_char = input_file.get();
					if(check_char != _T(' '))
						break;

					//Store the materials libraries file name
					input_file >> mesh_mat_lib;
				}
				break;

			case _T('u'):	//usemtl - which materials_ to use
				{
					check_char = input_file.get();
					if(check_char != _T('s'))
						break;

						check_char = input_file.get();
					if(check_char != _T('e'))
						break;

					check_char = input_file.get();
					if(check_char != _T('m'))
						break;

					check_char = input_file.get();
					if(check_char != _T('t'))
						break;

					check_char = input_file.get();
					if(check_char != _T('l'))
						break;

					check_char = input_file.get();
					if(check_char != _T(' '))
						break;

					mesh_material_temp = _T("");	//Make sure this is cleared
					input_file >> mesh_material_temp; //Get next type (string)
					mesh_materials.push_back(mesh_material_temp);
				}
				break;

			default:				
				break;
			}
		}

		subset_index_start_.push_back(vertex_index); //There won't be another index start after our last subset, so set it here

		//sometimes "g" is defined at the very top of the file, then again before the first group of faces.
		//This makes sure the first subset does not conatain "0" indices.
		if(subset_index_start_[1] == 0)
		{
			subset_index_start_.erase(subset_index_start_.begin()+1);
			subset_count_--;
		}

		//Make sure we have a default for the tex coord and normal
		//if one or both are not specified
		if(!has_normal)
			vertex_normal.push_back(H3DVector3(0.0f, 0.0f, 0.0f));
		if(!has_tex_coord)
			vertex_tex_coord.push_back(H3DVector2(0.0f, 0.0f));

		//Close the obj file, and open the mtl file
		input_file.close();
		//mesh_mat_lib = "E:\\Work\\Heaven3D\\media\\spaceCompound.mtl";
		H3DIFStream input_mat_file;
		input_mat_file.open(mesh_mat_lib.c_str());

		H3DString last_string_read;
		int material_count = materials_.size();	//total materials

		//kdset - If our diffuse color was not set, we can use the ambient color (which is usually the same)
		//If the diffuse color WAS set, then we don't need to set our diffuse color to ambient
		bool kdset = false;

		if( !input_mat_file.is_open() )
		{
			H3DString message = _T("Could not open: ");
			message += mesh_mat_lib;
			MessageBox(0, message.c_str(),_T("Error"), MB_OK);
			return false;
		}

		while(input_mat_file.peek() != EOF )
		{
			check_char = input_mat_file.get();	//Get next char

			switch (check_char)
			{
				//Check for comment
			case _T('#'):
				check_char = input_mat_file.get();
				while(check_char != _T('\n'))
					check_char = input_mat_file.get();
				break;

				//Set diffuse color
			case _T('K'):
				check_char = input_mat_file.get();
				if(check_char == _T('d'))	//Diffuse Color
				{
					check_char = input_mat_file.get();	//remove space

					input_mat_file >> materials_[material_count-1].diffuse_color_.x;
					input_mat_file >> materials_[material_count-1].diffuse_color_.y;
					input_mat_file >> materials_[material_count-1].diffuse_color_.z;

					kdset = true;
				}

				//Ambient Color (We'll store it in diffuse if there isn't a diffuse already)
				if(check_char == _T('a'))	
				{					
					check_char = input_mat_file.get();	//remove space
					if(!kdset)
					{
						input_mat_file >> materials_[material_count-1].diffuse_color_.x;
						input_mat_file >> materials_[material_count-1].diffuse_color_.y;
						input_mat_file >> materials_[material_count-1].diffuse_color_.z;
					}
				}
				break;

				//Check for transparency
			case _T('T'):
				{
					check_char = input_mat_file.get();
					if(check_char != _T('r'))
						break;

					check_char = input_mat_file.get();	//remove space
					float Transparency;
					input_mat_file >> Transparency;
					materials_[material_count-1].diffuse_color_.w = Transparency;

					if(Transparency > 0.0f)
						materials_[material_count-1].transparent_ = true;
				}
				break;
				//Some obj files specify d for transparency
			case _T('d'):
				{
					check_char = input_mat_file.get();
					if(check_char != _T(' '))
						break;

					float Transparency;
					input_mat_file >> Transparency;

					//'d' - 0 being most transparent, and 1 being opaque, opposite of Tr
					Transparency = 1.0f - Transparency;
					materials_[material_count-1].diffuse_color_.w = Transparency;

					if(Transparency > 0.0f)
						materials_[material_count-1].transparent_ = true;					
				}
				break;

				//Get the diffuse map (texture)
			case _T('m'):
				{
					check_char = input_mat_file.get();
					if(check_char != _T('a'))
						break;

					check_char = input_mat_file.get();
					if(check_char != _T('p'))
						break;

					check_char = input_mat_file.get();
					if(check_char != _T('_'))
						break;

					//map_Kd - Diffuse map
					check_char = input_mat_file.get();
					if(check_char == _T('K'))
					{
						check_char = input_mat_file.get();
						if(check_char != _T('d'))
							break;

						H3DString file_name_path;
						input_mat_file.get();	//Remove whitespace between map_Kd and file

						//Get the file path - We read the pathname char by char since
						//pathnames can sometimes contain spaces, so we will read until
						//we find the file extension
						bool tex_file_path_end = false;

						while(!tex_file_path_end)
						{
							check_char = input_mat_file.get();
							file_name_path += check_char;

							if(check_char == _T('.'))
							{
								for(int i = 0; i < 3; ++i)
									file_name_path += input_mat_file.get();

								tex_file_path_end = true;
							}							
						}

						//check if this texture has already been loaded
						bool already_loaded = false;
						for(size_t i = 0; i < texture_name_array_.size(); ++i)
						{
							if(file_name_path == texture_name_array_[i])
							{
								already_loaded = true;
								materials_[material_count-1].texture_array_index_ = i;
								materials_[material_count-1].has_texture_ = true;
							}
						}

						if( !already_loaded )
						{
							texture_name_array_.push_back(file_name_path.c_str());
							materials_[material_count-1].texture_array_index_ = texture_name_array_.size()-1;
							materials_[material_count-1].has_texture_ = true;
						}

					}
					else if(check_char == 'd') //map_d - alpha map
					{
						//Alpha maps are usually the same as the diffuse map
						//So we will assume that for now by only enabling
						//transparency for this materials_, as we will already
						//be using the alpha channel in the diffuse map
						materials_[material_count-1].transparent_ = true;
					}
				}
				break;
			case _T('n'):	//newmtl - Declare new materials_
				{
					check_char = input_mat_file.get();
					if(check_char != _T('e'))
						break;

					check_char = input_mat_file.get();
					if(check_char !=  _T('w'))
						break;

					check_char = input_mat_file.get();
					if(check_char !=  _T('m'))
						break;

					check_char = input_mat_file.get();
					if(check_char !=  _T('t'))
						break;

					check_char = input_mat_file.get();
					if(check_char !=  _T('l'))
						break;

					check_char = input_mat_file.get();
					if(check_char != ' ')
						break;

					H3DObjModelMaterial tmp_mat;
					materials_.push_back(tmp_mat);
					input_mat_file >> materials_[material_count].mat_name_;
					materials_[material_count].transparent_ = false;
					materials_[material_count].has_texture_ = false;
					materials_[material_count].texture_array_index_ = 0;
					material_count++;
					kdset = false;
				}
				break;

			default:
				break;
			}
		}



		input_mat_file.close();

		//Set the subsets materials_ to the index value
		//of the its materials_ in our materials_ array
		for(int i = 0; i < subset_count_; ++i)
		{
			bool hasMat = false;
			for(size_t j = 0; j < materials_.size(); ++j)
			{
				if(mesh_materials[i] == materials_[j].mat_name_)
				{
					subset_material_array_.push_back(j);
					hasMat = true;
				}
			}
			if(!hasMat)
				subset_material_array_.push_back(0); //Use first materials_ in array
		}

		//std::vector<H3DObjModelVertex> vertices;
		H3DObjModelVertex tempVert;

		//Create our vertices using the information we got 
		//from the file and store them in a vector
		for(uint32_t j = 0 ; j < total_vertics; ++j)
		{
			tempVert.pos = vertex_pos[vertex_pos_index[j]];
			tempVert.normal = vertex_normal[vertex_normal_index[j]];
			tempVert.tex_coord = vertex_tex_coord[vertex_texcoord_index[j]];
			vertices_.push_back(tempVert);
		}

		//////////////////////Compute Normals///////////////////////////
		//If computeNormals was set to true then we will create our own
		//normals, if it was set to false we will use the obj files normals
		if(compute_normals_)
		{
			std::vector<H3DVector3> tempNormal;

			//normalized and unnormalized normals
			H3DVector3 unnormalized = H3DVector3(0.0f, 0.0f, 0.0f);

			//Used to get vectors (sides) from the position of the verts
			float vecX, vecY, vecZ;

			//Two edges of our triangle
			H3DVector3 edge1(0.0f, 0.0f, 0.0f);
			H3DVector3 edge2(0.0f, 0.0f, 0.0f);

			//Compute face normals
			for(int i = 0; i < mesh_triangles; ++i)
			{
				//Get the vector describing one edge of our triangle (edge 0,2)
				vecX = vertices_[indices_[(i*3)]].pos.x - vertices_[indices_[(i*3)+2]].pos.x;
				vecY = vertices_[indices_[(i*3)]].pos.y - vertices_[indices_[(i*3)+2]].pos.y;
				vecZ = vertices_[indices_[(i*3)]].pos.z - vertices_[indices_[(i*3)+2]].pos.z;		
				edge1.Set(vecX, vecY, vecZ);	//Create our first edge

				//Get the vector describing another edge of our triangle (edge 2,1)
				vecX = vertices_[indices_[(i*3)+2]].pos.x - vertices_[indices_[(i*3)+1]].pos.x;
				vecY = vertices_[indices_[(i*3)+2]].pos.y - vertices_[indices_[(i*3)+1]].pos.y;
				vecZ = vertices_[indices_[(i*3)+2]].pos.z - vertices_[indices_[(i*3)+1]].pos.z;		
				edge2.Set(vecX, vecY, vecZ);	//Create our second edge

				//Cross multiply the two edge vectors to get the un-normalized face normal
				//XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
				unnormalized.CrossProduct(edge1,edge2);
				tempNormal.push_back(unnormalized);			//Save unormalized normal (for normal averaging)
			}

			//Compute vertex normals (normal Averaging)
			H3DVector3 normalSum = H3DVector3(0.0f, 0.0f, 0.0f);
			int facesUsing = 0;

			//Go through each vertex
			for(uint32_t i = 0; i < total_vertics; ++i)
			{
				//Check which triangles use this vertex
				for(int j = 0; j < mesh_triangles; ++j)
				{
					if(indices_[j*3] == i || indices_[(j*3)+1] == i || indices_[(j*3)+2] == i)
					{
						normalSum += tempNormal[j];/*
												   tX = XMVectorGetX(normalSum) + tempNormal[j].x;
												   tY = XMVectorGetY(normalSum) + tempNormal[j].y;
												   tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;
												   normalSum = XMVectorSet(tX, tY, tZ, 0.0f);*/	//If a face is using the vertex, add the unormalized face normal to the normalSum
						facesUsing++;
					}
				}

				normalSum /= float(facesUsing);//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
				normalSum.Normalize();//Normalize the normalSum vector
				vertices_[i].normal = normalSum;//Store the normal in our current vertex
				normalSum.Set(0.0f, 0.0f, 0.0f);//Clear normalSum and facesUsing for next vertex
				facesUsing = 0;
			}
		}
#if defined(DEBUG) || defined(_DEBUG)
		/*
		vector<H3DObjModelVertex>		vertices_;
		vector<uint32_t>				indices_;
		vector<int32_t>					subset_index_start_;
		vector<int32_t>					subset_material_array_;
		vector<H3DObjModelMaterial>		materials_;
		vector<H3DString>				texture_name_array_;
		int32_t							subset_count_;			// 模型可划分为几个子集
		bool							righthand_coord_sys_;	// 模型是以右手坐标系存储的吗
		bool							compute_normals_;
		*/
		TCHAR msg_buff[256];
	
		for( size_t i = 0 ; i < vertices_.size() ; ++i )
		{
			memset( msg_buff,0 ,sizeof(TCHAR)*256 );
			_stprintf( msg_buff , _T("vertices[%d].pos = (%f,%f,%f)\n"), i ,vertices_[i].pos.x,vertices_[i].pos.y,vertices_[i].pos.z );
			OutputDebugString(msg_buff);

			memset( msg_buff,0 ,sizeof(TCHAR)*256 );
			_stprintf( msg_buff , _T("vertices[%d].tex_coord = (%f,%f)\n"), i ,vertices_[i].tex_coord.x,vertices_[i].tex_coord.y );
			OutputDebugString(msg_buff);

			memset( msg_buff,0 ,sizeof(TCHAR)*256 );
			_stprintf( msg_buff , _T("vertices[%d].normal = (%f,%f,%f)\n\n"), i ,vertices_[i].normal.x,vertices_[i].normal.y,vertices_[i].normal.z );
			OutputDebugString(msg_buff);
		}

		OutputDebugString(_T("\n==============================\n"));

		for(size_t i = 0 ; i < indices_.size() ; ++i )
		{
			memset( msg_buff,0 ,sizeof(TCHAR)*256 );
			_stprintf( msg_buff , _T("indices[%d] = %d\n"), i ,indices_[i]);
			OutputDebugString(msg_buff);
		}

		OutputDebugString(_T("\n==============================\n"));

		for(size_t i = 0 ; i < subset_index_start_.size() ; ++i )
		{
			memset( msg_buff,0 ,sizeof(TCHAR)*256 );
			_stprintf( msg_buff , _T("subset_index_start[%d] = %d\n"), i ,subset_index_start_[i]);
			OutputDebugString(msg_buff);
		}

		OutputDebugString(_T("\n==============================\n"));

		for(size_t i = 0 ; i < texture_name_array_.size() ; ++i )
		{
			memset( msg_buff,0 ,sizeof(TCHAR)*256 );
			_stprintf( msg_buff , _T("texture_name_array[%d] = %s\n"), i ,texture_name_array_[i].c_str());
			OutputDebugString(msg_buff);
		}

		OutputDebugString(_T("\n==============================\n"));

		for(size_t i = 0 ; i < materials_.size() ; ++i )
		{
			memset( msg_buff,0 ,sizeof(TCHAR)*256 );
			_stprintf( msg_buff , _T("materials[%d] = %s , diffuse = (%f,%f,%f,%f) , texture array index = %d , has_texture : %s , transparent = %s \n"),
				i ,materials_[i].mat_name_.c_str(),materials_[i].diffuse_color_.x,materials_[i].diffuse_color_.y,
				materials_[i].diffuse_color_.z,materials_[i].diffuse_color_.w, materials_[i].texture_array_index_,
				materials_[i].has_texture_?_T("Y"):_T("N"), materials_[i].transparent_?_T("Y"):_T("N") );
			OutputDebugString(msg_buff);
		}

#endif
		return true;
	}
}