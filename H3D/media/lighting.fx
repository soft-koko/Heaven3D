//  �����صĴ�һ���������������ռ���shader
//  written by Xiong Xinke

//  �������������洢CPU���ݽ������ⲿ����
cbuffer MatrixBuffer
{
	matrix world_matrix;			//  ������󣬵�ʵ���ϴ洢�����������������
	matrix view_matrix;			//  ��ǰ������Ĺ۲���󣬵�ʵ���ϴ洢���ǹ۲����������
	matrix projection_matrix;		//  ��ǰ��͸��ͶӰ���󣬵�ʵ���ϴ洢����͸��ͶӰ����������
};

//  ��������
struct VertexInputType
{
    float4 position : POSITION;  // ����������ռ��е�����
    float2 tex : TEXCOORD0;     // ����ĵ�һ����������
    float3 normal : NORMAL;   // ����ķ���
};

struct PixelInputType			// ������ɫ����������ͣ���������ɫ������������
{
    float4 position : SV_POSITION;	// ���㾭transform������λ��
    float2 tex : TEXCOORD0;		 // ����ĵ�һ����������
    float3 normal : NORMAL;		// ����ķ���
};

// Vertex Shader ����ڴ���
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    
   // ������Ķ���������չΪ��ά���Ա��ڽ��о������
    input.position.w = 1.0f;

    // ���ݴ��ݽ����������任�þ��󣬼��㾭���任��Ķ���λ��
    output.position = mul(input.position, world_matrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, projection_matrix);
    
    // �������겻�ñ䣬ֱ�Ӵ���
    output.tex = input.tex;
    
    // �Ѷ���ķ���Ҳ��һ���任����ֻ�������������任
    output.normal = mul(input.normal, (float3x3)world_matrix);
    output.normal = normalize(output.normal);

    return output;
}

// ==================================================

Texture2D shader_texture;			// 2D���������
SamplerState sample_type;		//  ���������

cbuffer LightBuffer
{
    float4 diffuse_color;			// �����������ɫ����
    float3 light_direction;			// ��ķ���
    float padding;
};

// Pixel Shader ����ڴ���
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 texture_color;
	float3 light_dir;
	float light_intensity;
	float4 color;

	// ������������õ������ص��������ɫ
	texture_color = shader_texture.Sample(sample_type, input.tex);

	// ȡ�ô��ݽ����Ĺ��շ���ķ��������Լ���
	light_dir = -light_direction;

	light_intensity = saturate(dot(input.normal, light_dir));
	color = saturate(diffuse_color * light_intensity);
	color = color * texture_color;  // ������ɫ = ������ɫ * ������ɫ
	return color;
}