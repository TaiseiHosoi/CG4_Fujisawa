//float4x4 World;
//float4x4 View;
//float4x4 Projection;

// TODO: add effect parameters here.
//float Kr = 0.5f;
//float Power = 2.0;
//float4 RimColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
////float3 CameraPosition;
//float3 LightDirection = normalize(float3(0, 0, 1));




//
// VertexShaderOutput
// 
struct VertexShaderOutput
{
	float4 Position : SV_POSITION;

	// TODO: add vertex shader outputs such as colors and texture
	// coordinates here. These values will automatically be interpolated
	// over the triangle, and provided as input to your pixel shader.
	float2 uv : TEXCOORD0;
	float3 Normal : TEXCOORD1;
};

//ワールド行列
cbuffer ConstBufferDataTransform:register(b0)
{
	matrix mat;//3D変換行列
}
//3D変換行列
cbuffer ConstBufferDataCamera : register(b1)
{
	float3 CameraPosition;
}
//3D変換行列
cbuffer ConstBufferRim : register(b2)
{
	float4 m_color: packoffset(c0);
	float m_pow : packoffset(c1);
	float m_emission : packoffset(c1.y);
}