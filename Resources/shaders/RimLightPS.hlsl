#include "RimLight.hlsli"

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0); //0番スロットに設定されたサンプラー


float4 main(VertexShaderOutput input) : SV_TARGET
{


float rim = 1 - (saturate(dot(CameraPosition, input.Normal)) / m_emission);

float4 texcolor = float4(tex.Sample(smp, input.uv));

float3 Emission = m_color * pow(rim, m_pow);

	float4 colors = float4(texcolor.rgb * Emission, texcolor.a * m_color.a);

return colors;
}