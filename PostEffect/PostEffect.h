#pragma once
#include "DirectXCommon.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include"input.h"
class PostEffect
{
public:

    /// <summary>
    /// 頂点データ構造体
    /// </summary>
    struct VertexPosUv {
        Vector3 pos; // xyz座標
        Vector2 uv;  // uv座標
    };

    /// <summary>
    /// 定数バッファ用データ構造体
    /// </summary>
    struct ConstBufferData {
        Vector4 color; // 色 (RGBA)
        Matrix4 mat;   // ３Ｄ変換行列
    };

    struct ConstBufferDataB1
    {
        int blurTexNum; //ぼかす枚数
        int breadth;    //広さ
    };

    static void Initialize(DirectXCommon* dxCommon);

    static void Finalize();

    /// <summary>
    /// パイプライン生成
    /// </summary>
    static void CreatGraphicsPipelineState();

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    static void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンド処理</param>
    static void PostDrawScene();

private:

    static const float clearColor[4];

    static ID3D12Device* device_;

    static ID3D12GraphicsCommandList* commandList;

    static VertexPosUv vertices[4];

    static VertexPosUv* vertMap;

    static ConstBufferDataB1* constBuffDataB1;

    static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

    static Microsoft::WRL::ComPtr<ID3D12Resource> constBuffResourceB1;

    //頂点バッファビューの作成
    static D3D12_VERTEX_BUFFER_VIEW vbView;
    static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[2];

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    static Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV用のデスクリプタヒープ
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用のデスクリプタヒープ
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    static Input* input_;
    //ブラーのプロパティ
   static int blurTexNum_; //ぼかす枚数
   static int breadth_;    //広さ
};

