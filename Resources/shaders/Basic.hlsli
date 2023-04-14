//マテリアル
cbuffer ConstBufferDataMaterial : register(b0) {
	//色
	float4 color;
};



//3D変換行列
cbuffer ConstBufferDataTransform : register(b1) {
    matrix mat;//3D変換行列
}

//頂点シェーダの出力構造体
//頂点シェーダからピクセルシェーダへのやり取りに使用する
struct VSOutput
{
    //システム用頂点座標
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//法線ベクトル
    //uv値
    float2 uv  :TEXCOORD;
};


