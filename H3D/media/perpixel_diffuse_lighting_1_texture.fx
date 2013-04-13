//  逐像素的带一层纹理的漫反射光照计算shader
//  written by Xiong Xinke

//  常量缓冲区，存储CPU传递进来的外部参数
cbuffer MatrixBuffer
{
	matrix world_matrix;			//  世界矩阵，但实质上存储的是世界矩阵的逆矩阵
	matrix view_matrix;			//  当前摄像机的观察矩阵，但实质上存储的是观察矩阵的逆矩阵
	matrix projection_matrix;		//  当前的透视投影矩阵，但实质上存储的是透视投影矩阵的逆矩阵
};

//  顶点类型
struct VertexInputType
{
    float4 position : POSITION;  // 顶点在世界空间中的坐标
    float2 tex : TEXCOORD0;     // 顶点的第一层纹理坐标
    float3 normal : NORMAL;   // 顶点的法线
};

struct PixelInputType			// 顶点着色器的输出类型，即像素着色器的输入类型
{
    float4 position : SV_POSITION;	// 顶点经transform计算后的位置
    float2 tex : TEXCOORD0;		 // 顶点的第一层纹理坐标
    float3 normal : NORMAL;		// 顶点的法线
};

// Vertex Shader 主入口代码
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    
   // 把输入的顶点坐标扩展为四维，以便于进行矩阵计算
    input.position.w = 1.0f;

    // 根据传递进来的三个变换用矩阵，计算经过变换后的顶点位置
    output.position = mul(input.position, world_matrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, projection_matrix);
    
    // 纹理坐标不用变，直接传递
    output.tex = input.tex;
    
    // 把顶点的法线也做一个变换，但只用世界坐标做变换
    output.normal = mul(input.normal, (float3x3)world_matrix);
    output.normal = normalize(output.normal);

    return output;
}

// ==================================================

Texture2D shader_texture;			// 2D的纹理对象
SamplerState sample_type;		//  纹理采样器

cbuffer LightBuffer
{
    float4 diffuse_color;			// 光的漫反射颜色分量
    float3 light_direction;			// 光的方向
    float padding;
};

// Pixel Shader 主入口代码
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	return float4(1.f,1.f,1.f,1.f);
	float4 texture_color;
	float3 light_dir;
	float light_intensity;
	float4 color;

	// 根据纹理坐标得到该像素点的纹理颜色
	texture_color = shader_texture.Sample(sample_type, input.tex);

	// 取得传递进来的光照方向的反方向，用以计算
	light_dir = -light_direction;

	light_intensity = saturate(dot(input.normal, light_dir));
	color = saturate(diffuse_color * light_intensity);
	color = color * texture_color;  // 最终颜色 = 光照颜色 * 纹理颜色
	return color;
}
