#include "h3d_pch.h"
#include "h3d_3ds_model.h"

namespace Heaven3D
{
	// 构造函数的功能是初始化H3D3dsModelChunk数据
	H3D3dsModelLoader::H3D3dsModelLoader()
	{
		current_chunk_ = new H3D3dsModelChunk;        // 初始化并为当前的块分配空间
		temp_chunk_ = new H3D3dsModelChunk;          // 初始化一个临时块并分配空间
	}

	// 打开一个3ds文件，读出其中的内容，并释放内存
	bool H3D3dsModelLoader::LoadFile(H3D3dsModel *model, char *file_name)
	{
		char message_string[255] = {0};

		// 打开一个3ds文件
		file_pointer_ = fopen(file_name, "rb");

		// 确保所获得的文件指针合法
		if(!file_pointer_) 
		{
			sprintf(message_string, "Unable to find the file: %s!", file_name);
			MessageBox(NULL, message_string, "Error", MB_OK);
			return false;
		}

		// 当文件打开之后，首先应该将文件最开始的数据块读出以判断是否是一个3ds文件
		// 如果是3ds文件的话，第一个块ID应该是H3D_3DS_MODEL_PRIMARY

		// 将文件的第一块读出并判断是否是3ds文件
		ReadChunk(current_chunk_);

		// 确保是3ds文件
		if (current_chunk_->ID != H3D_3DS_MODEL_PRIMARY)
		{
			sprintf(message_string, "Unable to load H3D_3DS_MODEL_PRIMARY chuck from file: %s!", file_name);
			MessageBox(NULL, message_string, "Error", MB_OK);
			return false;
		}

		// 现在开始读入数据，ProcessNextChunk()是一个递归函数

		// 通过调用下面的递归函数，将对象读出
		ProcessNextChunk(model, current_chunk_);

		// 在读完整个3ds文件之后，计算顶点的法线
		ComputeNormals(model);

		// 释放内存空间
		CleanUp();

		return true;
	}

	// 下面的函数释放所有的内存空间，并关闭文件
	void H3D3dsModelLoader::CleanUp()
	{
		fclose(file_pointer_);            // 关闭当前的文件指针
		delete current_chunk_;            // 释放当前块
		delete temp_chunk_;              // 释放临时块
	}

	// 下面的函数读出3ds文件的主要部分
	void H3D3dsModelLoader::ProcessNextChunk(H3D3dsModel *model, H3D3dsModelChunk *previous_chunk)
	{
		H3D3dsModelObject new_object = {0};          // 用来添加到对象链表
		H3D3dsModelMaterialInfo new_material = {0};        // 用来添加到材质链表
		uint32_t version = 0;          // 保存文件版本
		int32_t buffer[50000] = {0};          // 用来跳过不需要的数据

		current_chunk_ = new H3D3dsModelChunk;        // 为新的块分配空间    

		// 下面每读一个新块，都要判断一下块的ID，如果该块是需要的读入的，则继续进行
		// 如果是不需要读入的块，则略过

		// 继续读入子块，直到达到预定的长度
		while( previous_chunk->bytesRead < previous_chunk->length )
		{
			// 读入下一个块
			ReadChunk(current_chunk_);

			// 判断块的ID号
			switch (current_chunk_->ID)
			{
			case H3D_3DS_MODEL_VERSION:              // 文件版本号

				// 在该块中有一个无符号短整型数保存了文件的版本

				// 读入文件的版本号，并将字节数添加到bytesRead变量中
				current_chunk_->bytesRead += fread(&version, 1, current_chunk_->length - current_chunk_->bytesRead, file_pointer_);

				// 如果文件版本号大于3，给出一个警告信息
				if (version > 0x03)
					MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
				break;

			case H3D_3DS_MODEL_OBJECTINFO:            // 网格版本信息

				// 读入下一个块
				ReadChunk(temp_chunk_);

				// 获得网格的版本号
				temp_chunk_->bytesRead += fread(&version, 1, temp_chunk_->length - temp_chunk_->bytesRead, file_pointer_);

				// 增加读入的字节数
				current_chunk_->bytesRead += temp_chunk_->bytesRead;

				// 进入下一个块
				ProcessNextChunk(model, current_chunk_);
				break;

			case H3D_3DS_MODEL_MATERIAL:              // 材质信息

				// 材质的数目递增
				model->materail_counts_++;

				// 在纹理链表中添加一个空白纹理结构
				model->materails_.push_back(new_material);

				// 进入材质装入函数
				ProcessNextMaterialChunk(model, current_chunk_);
				break;

			case H3D_3DS_MODEL_OBJECT:              // 对象的名称

				// 该块是对象信息块的头部，保存了对象了名称

				// 对象数递增
				model->object_counts_++;

				// 添加一个新的tObject节点到对象链表中
				model->object.push_back(new_object);

				// 初始化对象和它的所有数据成员
				memset(&(model->object[model->object_counts_ - 1]), 0, sizeof(H3D3dsModelObject));

				// 获得并保存对象的名称，然后增加读入的字节数
				current_chunk_->bytesRead += GetString(model->object[model->object_counts_ - 1].texture_name_);

				// 进入其余的对象信息的读入
				ProcessNextObjectChunk(model, &(model->object[model->object_counts_ - 1]), current_chunk_);
				break;

			case H3D_3DS_MODEL_EDITKEYFRAME:

				// 跳过关键帧块的读入，增加需要读入的字节数
				current_chunk_->bytesRead += fread(buffer, 1, current_chunk_->length - current_chunk_->bytesRead, file_pointer_);
				break;

			default: 

				// 跳过所有忽略的块的内容的读入，增加需要读入的字节数
				current_chunk_->bytesRead += fread(buffer, 1, current_chunk_->length - current_chunk_->bytesRead, file_pointer_);
				break;
			}

			// 增加从最后块读入的字节数
			previous_chunk->bytesRead += current_chunk_->bytesRead;
		}

		// 释放当前块的内存空间
		delete current_chunk_;
		current_chunk_ = previous_chunk;
	}

	// 下面的函数处理所有的文件中对象的信息
	void H3D3dsModelLoader::ProcessNextObjectChunk(H3D3dsModel *model, H3D3dsModelObject *object, H3D3dsModelChunk *previous_chunk)
	{
		int32_t buffer[50000] = {0};          // 用于读入不需要的数据

		// 对新的块分配存储空间
		current_chunk_ = new H3D3dsModelChunk;

		// 继续读入块的内容直至本子块结束
		while (previous_chunk->bytesRead < previous_chunk->length)
		{
			// 读入下一个块
			ReadChunk(current_chunk_);

			// 区别读入是哪种块
			switch (current_chunk_->ID)
			{
			case H3D_3DS_MODEL_OBJECT_MESH:          // 正读入的是一个新块

				// 使用递归函数调用，处理该新块
				ProcessNextObjectChunk(model, object, current_chunk_);
				break;

			case H3D_3DS_MODEL_OBJECT_VERTICES:        // 读入是对象顶点
				ReadVertices(object, current_chunk_);
				break;

			case H3D_3DS_MODEL_OBJECT_FACES:          // 读入的是对象的面
				ReadVertexIndices(object, current_chunk_);
				break;

			case H3D_3DS_MODEL_OBJECT_MATERIAL:        // 读入的是对象的材质名称

				// 该块保存了对象材质的名称，可能是一个颜色，也可能是一个纹理映射。同时在该块中也保存了
				// 纹理对象所赋予的面

				// 下面读入对象的材质名称
				ReadObjectMaterial(model, object, current_chunk_);      
				break;

			case H3D_3DS_MODEL_OBJECT_UV:            // 读入对象的UV纹理坐标

				// 读入对象的UV纹理坐标
				ReadUVCoordinates(object, current_chunk_);
				break;

			default: 

				// 略过不需要读入的块
				current_chunk_->bytesRead += fread(buffer, 1, current_chunk_->length - current_chunk_->bytesRead, file_pointer_);
				break;
			}

			// 添加从最后块中读入的字节数到前面的读入的字节中
			previous_chunk->bytesRead += current_chunk_->bytesRead;
		}

		// 释放当前块的内存空间，并把当前块设置为前面块
		delete current_chunk_;
		current_chunk_ = previous_chunk;
	}

	// 下面的函数处理所有的材质信息
	void H3D3dsModelLoader::ProcessNextMaterialChunk(H3D3dsModel *model, H3D3dsModelChunk *previous_chunk)
	{
		int32_t buffer[50000] = {0};          // 用于读入不需要的数据

		// 给当前块分配存储空间
		current_chunk_ = new H3D3dsModelChunk;

		// 继续读入这些块，知道该子块结束
		while (previous_chunk->bytesRead < previous_chunk->length)
		{
			// 读入下一块
			ReadChunk(current_chunk_);

			// 判断读入的是什么块
			switch (current_chunk_->ID)
			{
			case H3D_3DS_MODEL_MATNAME:              // 材质的名称

				// 读入材质的名称
				current_chunk_->bytesRead += fread(model->materails_[model->materail_counts_ - 1].texture_name_, 1, current_chunk_->length - current_chunk_->bytesRead, file_pointer_);
				break;

			case H3D_3DS_MODEL_MATDIFFUSE:            // 对象的R G B颜色
				ReadColorChunk(&(model->materails_[model->materail_counts_ - 1]), current_chunk_);
				break;

			case H3D_3DS_MODEL_MATMAP:              // 纹理信息的头部

				// 进入下一个材质块信息
				ProcessNextMaterialChunk(model, current_chunk_);
				break;

			case H3D_3DS_MODEL_MATMAPFILE:            // 材质文件的名称

				// 读入材质的文件名称
				current_chunk_->bytesRead += fread(model->materails_[model->materail_counts_ - 1].texture_file_, 1, current_chunk_->length - current_chunk_->bytesRead, file_pointer_);
				break;

			default: 

				// 掠过不需要读入的块
				current_chunk_->bytesRead += fread(buffer, 1, current_chunk_->length - current_chunk_->bytesRead, file_pointer_);
				break;
			}

			// 添加从最后块中读入的字节数
			previous_chunk->bytesRead += current_chunk_->bytesRead;
		}

		// 删除当前块，并将当前块设置为前面的块
		delete current_chunk_;
		current_chunk_ = previous_chunk;
	}

	// 下面函数读入块的ID号和它的字节长度
	void H3D3dsModelLoader::ReadChunk(H3D3dsModelChunk *chunk)
	{
		// 读入块的ID号，占用了2个字节。块的ID号象H3D_3DS_MODEL_OBJECT或H3D_3DS_MODEL_MATERIAL一样，说明了在块中所包含的内容
		chunk->bytesRead = fread(&chunk->ID, 1, 2, file_pointer_);

		// 然后读入块占用的长度，包含了四个字节
		chunk->bytesRead += fread(&chunk->length, 1, 4, file_pointer_);
	}

	// 下面的函数读入一个字符串
	int32_t H3D3dsModelLoader::GetString(char* buffer)
	{
		int32_t index = 0;

		// 读入一个字节的数据
		fread(buffer, 1, 1, file_pointer_);

		// 直到结束
		while (*(buffer + index++) != 0) 
		{
			// 读入一个字符直到NULL
			fread(buffer + index, 1, 1, file_pointer_);
		}

		// 返回字符串的长度
		return strlen(buffer) + 1;
	}

	// 下面的函数读入RGB颜色
	void H3D3dsModelLoader::ReadColorChunk(H3D3dsModelMaterialInfo *materail, H3D3dsModelChunk *chunk)
	{
		// 读入颜色块信息
		ReadChunk(temp_chunk_);

		// 读入RGB颜色
		temp_chunk_->bytesRead += fread(materail->color_, 1, temp_chunk_->length - temp_chunk_->bytesRead, file_pointer_);

		// 增加读入的字节数
		chunk->bytesRead += temp_chunk_->bytesRead;
	}

	// 下面的函数读入顶点索引
	void H3D3dsModelLoader::ReadVertexIndices(H3D3dsModelObject *object, H3D3dsModelChunk *previous_chunk)
	{
		unsigned short index = 0;          // 用于读入当前面的索引

		// 读入该对象中面的数目
		previous_chunk->bytesRead += fread(&object->faces_count_, 1, 2, file_pointer_);

		// 分配所有面的存储空间，并初始化结构
		object->faces_ = new H3D3dsModelFace [object->faces_count_];
		memset(object->faces_, 0, sizeof(H3D3dsModelFace) * object->faces_count_);

		// 遍历对象中所有的面
		for(int32_t i = 0; i < object->faces_count_; i++)
		{
			for(int32_t j = 0; j < 4; j++)
			{
				// 读入当前面的第一个点 
				previous_chunk->bytesRead += fread(&index, 1, sizeof(index), file_pointer_);

				if(j < 3)
				{
					// 将索引保存在面的结构中
					object->faces_[i].vert_index[j] = index;
				}
			}
		}
	}

	// 下面的函数读入对象的UV坐标
	void H3D3dsModelLoader::ReadUVCoordinates(H3D3dsModelObject *object, H3D3dsModelChunk *previous_chunk)
	{
		// 为了读入对象的UV坐标，首先需要读入UV坐标的数量，然后才读入具体的数据

		// 读入UV坐标的数量
		previous_chunk->bytesRead += fread(&object->texture_count_, 1, 2, file_pointer_);

		// 分配保存UV坐标的内存空间
		object->tex_coord_ = new H3DVector2 [object->texture_count_];

		// 读入纹理坐标
		previous_chunk->bytesRead += fread(object->tex_coord_, 1, previous_chunk->length - previous_chunk->bytesRead, file_pointer_);
	}

	// 读入对象的顶点
	void H3D3dsModelLoader::ReadVertices(H3D3dsModelObject *object, H3D3dsModelChunk *previous_chunk)
	{
		// 在读入实际的顶点之前，首先必须确定需要读入多少个顶点。

		// 读入顶点的数目
		previous_chunk->bytesRead += fread(&(object->verts_count_), 1, 2, file_pointer_);

		// 分配顶点的存储空间，然后初始化结构体
		object->verts_ = new H3DVector3 [object->verts_count_];
		memset(object->verts_, 0, sizeof(H3DVector3) * object->verts_count_);

		// 读入顶点序列
		previous_chunk->bytesRead += fread(object->verts_, 1, previous_chunk->length - previous_chunk->bytesRead, file_pointer_);

		// 现在已经读入了所有的顶点。
		// 因为3D Studio Max的模型的Z轴是指向上的，因此需要将y轴和z轴翻转过来。
		// 具体的做法是将Y轴和Z轴交换，然后将Z轴反向。

		// 遍历所有的顶点
		for(int32_t i = 0; i < object->verts_count_; i++)
		{
			// 保存Y轴的值
			float fTempY = object->verts_[i].y;

			// 设置Y轴的值等于Z轴的值
			object->verts_[i].y = object->verts_[i].z;

			// 设置Z轴的值等于-Y轴的值 
			object->verts_[i].z = -fTempY;
		}
	}

	// 下面的函数读入对象的材质名称
	void H3D3dsModelLoader::ReadObjectMaterial(H3D3dsModel *model, H3D3dsModelObject *object, H3D3dsModelChunk *previous_chunk)
	{
		char strMaterial[255] = {0};      // 用来保存对象的材质名称
		int32_t buffer[50000] = {0};        // 用来读入不需要的数据

		// 材质或者是颜色，或者是对象的纹理，也可能保存了象明亮度、发光度等信息。

		// 下面读入赋予当前对象的材质名称
		previous_chunk->bytesRead += GetString(strMaterial);

		// 遍历所有的纹理
		for(int32_t i = 0; i < model->materail_counts_; i++)
		{
			//如果读入的纹理与当前的纹理名称匹配
			if(strcmp(strMaterial, model->materails_[i].texture_name_) == 0)
			{
				// 设置材质ID
				object->material_id_ = i;

				// 判断是否是纹理映射，如果texture_file_是一个长度大于1的字符串，则是纹理
				if(strlen(model->materails_[i].texture_file_) > 0) {
					//载入纹理
					//BuildTexture(model->materails_[i].texture_file_, model->texture[object->material_id_]);
					// 设置对象的纹理映射标志
					object->has_texture_ = true;
				}  
				break;
			}
			else
			{
				// 如果该对象没有材质，则设置ID为-1
				object->material_id_ = -1;
			}
		}

		previous_chunk->bytesRead += fread(buffer, 1, previous_chunk->length - previous_chunk->bytesRead, file_pointer_);
	}      

	// 下面的函数用于计算对象的法向量
	void H3D3dsModelLoader::ComputeNormals(H3D3dsModel *model)
	{
		H3DVector3 vector1, vector2, normal, poly_vector[3];

		// 如果模型中没有对象，则返回
		if(model->object_counts_ <= 0)
			return;

		// 遍历模型中所有的对象
		for(int32_t index = 0; index < model->object_counts_; index++)
		{
			// 获得当前的对象
			H3D3dsModelObject *object = &(model->object[index]);

			// 分配需要的存储空间
			H3DVector3 *normals_    = new H3DVector3[object->faces_count_];
			H3DVector3 *temp_normals  = new H3DVector3[object->faces_count_];
			object->normals_    = new H3DVector3[object->verts_count_];

			// 遍历对象的所有面
			for(int32_t i=0; i < object->faces_count_; i++)
			{                        
				poly_vector[0] = object->verts_[object->faces_[i].vert_index[0]];
				poly_vector[1] = object->verts_[object->faces_[i].vert_index[1]];
				poly_vector[2] = object->verts_[object->faces_[i].vert_index[2]];

				// 计算面的法向量
				vector1 = poly_vector[0] - poly_vector[2];    // 获得多边形的矢量
				vector2 = poly_vector[2] - poly_vector[1];    // 获得多边形的第二个矢量
				normal.CrossProduct(vector1, vector2);    // 获得两个矢量的叉积
				temp_normals[i] = normal;					// 保存非规范化法向量
				normal.Normalize();//						// 规范化获得的叉积
				normals_[i] = normal;            // 将法向量添加到法向量列表中
			}

			// 下面求顶点法向量
			H3DVector3 sum_vector(0.0, 0.0, 0.0);
			H3DVector3 zero_vector = sum_vector;
			int32_t shared=0;
			// 遍历所有的顶点
			for( int32_t i = 0; i < object->verts_count_; i++)      
			{
				for (int32_t j = 0; j < object->faces_count_; j++)  // 遍历所有的三角形面
				{                        // 判断该点是否与其它的面共享
					if (object->faces_[j].vert_index[0] == i || 
						object->faces_[j].vert_index[1] == i || 
						object->faces_[j].vert_index[2] == i)
					{
						sum_vector += temp_normals[j];
						shared++;                
					}
				} 

				object->normals_[i] = sum_vector / float(-shared);

				// 规范化最后的顶点法向
				object->normals_[i].Normalize();  

				sum_vector = zero_vector;                
				shared = 0;                    
			}

			// 释放存储空间，开始下一个对象
			delete [] temp_normals;
			delete [] normals_;
		}
	}
}