#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{

	PSOutput output;
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);
	//// Lambert反射
	//float3 light = normalize(float3(1,-1,1)); // 右下奥　向きのライト
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	////// 陰影とテクスチャの色を合成
	////return shadecolor * texcolor;

	////影とテクスチャの色を合成
	//output.target0 = shadecolor * texcolor;
	//output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);
	//return output;

    float4 lightColor = { 1,1,1,1};
    float4 limColor = { 1,1,0,1 };
    float3 worldSpaceLightPos = { 100,300,-100 };

    //アンビエント
    float4 ambient = texcolor * 0.3 * lightColor;
    ambient.w = 1;

    //ディフューズ
    float intensity = saturate(dot(normalize(input.normal), worldSpaceLightPos));
    intensity = smoothstep(0.05, 0.06, intensity);
    float4 color = texcolor;
    float4 diffuse = color * intensity * lightColor;

    //スペキュラー
    float3 eyeDir = normalize(cameraPos - input.worldPos);
    float3 lightDir = normalize(worldSpaceLightPos);
    float3 normal = normalize(input.normal);
    float3 reflectDir = -lightDir + 2 * normal * dot(normal, lightDir);
    float4 specular = pow(saturate(dot(reflectDir, eyeDir)), 20);
    specular = smoothstep(0.05, 0.06, specular) * lightColor;

    //リムライト
    float rim = 1.0 - saturate(dot(eyeDir, normal));
    rim = smoothstep(0.7, 1.0, rim);

    float4 abs = ambient + diffuse + specular;
    float4 col = lerp(abs, limColor/*_RimColor*/, rim); //カラーが混ざらないようにラープでリムのカラー除外
    
    output.target0 = col;
    output.target1 = col;
    return output;


}