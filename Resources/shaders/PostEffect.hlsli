
//頂点シェーダーの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	// uv値
	float2 uv :TEXCOORD;
};

cbuffer  cbuff1 : register(b0) {
	int blurTexNum; //ぼかす枚数
	int breadth;    //広さ
}