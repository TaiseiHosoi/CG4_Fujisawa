#include "Obj.hlsli"

Texture2D<float4>tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;

	float3 light = normalize(float3(1,-1,1));	//右下奥　向きのライト
	float light_diffuse = saturate(dot(-light, input.normal));
	float4 shade_color;
	shade_color = float4(m_ambient,1);//アンビエント項

	float4 md = float4(m_diffuse, 1);

	shade_color += md * light_diffuse;//ディフューズ項
	float4 texcolor = tex.Sample(smp, input.uv);
	//return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);

	//影とテクスチャの色を合成
	output.target0 = shade_color * texcolor;
	output.target1 = float4(1 - (shade_color * texcolor).rgb, 1);
	return output;
}