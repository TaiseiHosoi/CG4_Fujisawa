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
    /// ���_�f�[�^�\����
    /// </summary>
    struct VertexPosUv {
        Vector3 pos; // xyz���W
        Vector2 uv;  // uv���W
    };

    /// <summary>
    /// �萔�o�b�t�@�p�f�[�^�\����
    /// </summary>
    struct ConstBufferData {
        Vector4 color; // �F (RGBA)
        Matrix4 mat;   // �R�c�ϊ��s��
    };

    struct ConstBufferDataB1
    {
        int blurTexNum; //�ڂ�������
        int breadth;    //�L��
    };

    static void Initialize(DirectXCommon* dxCommon);

    static void Finalize();

    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    static void CreatGraphicsPipelineState();

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    static void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h����</param>
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

    //���_�o�b�t�@�r���[�̍쐬
    static D3D12_VERTEX_BUFFER_VIEW vbView;
    static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[2];

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //�[�x�o�b�t�@
    static Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�̃f�X�N���v�^�q�[�v
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�̃f�X�N���v�^�q�[�v
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    static Input* input_;
    //�u���[�̃v���p�e�B
   static int blurTexNum_; //�ڂ�������
   static int breadth_;    //�L��
};

