#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "GameCamera.h"

/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
class ParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataMaterial {
		XMFLOAT4 color; // �F (RGBA)
	};

	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Matrix4 mat;
		Matrix4 matBillboard;	// �r���{�[�h�s��
	};

	//�p�[�e�B�N���ꗱ
	struct Particle {
		//DirectX���ȗ�
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//���W
		XMFLOAT3 position = {};
		//���x
		XMFLOAT3 velocity = {};
		//�����x
		XMFLOAT3 accel = {};
		//���݃t���[��
		int frame = 0;
		//�ߋ��t���[��
		int num_frame = 0;

		//�X�P�[��
		float scale = 1.0f;
		//�����l
		float s_scale = 1.0f;
		//�ŏI�l
		float e_scale = 0.0f;

		XMFLOAT4 color; // �F (RGBA)
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	//static const int vertexCount = 30;//���_��
	static const int vertexCount = 1024;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static ParticleManager* Create();

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetTarget(XMFLOAT3 target);

	/*/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3 move);*/

	/// <summary>
	/// �x�N�g���ɂ�鎋�_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveEyeVector(XMFLOAT3 move);
private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// ���_�f�[�^�z��
	static VertexPos vertices[vertexCount];
	//�r���{�[�h�s��
	static XMMATRIX matBillboard;
	//Y�����r���{�[�h�s��
	static XMMATRIX matBillboardY;
	//�p�[�e�B�N���z��
	std::forward_list<Particle>particles;

private:// �ÓI�����o�֐�
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static void LoadTexture();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

public: // �����o�֐�
	static void LoadTexture(const std::string& fileName);
	bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	///	<param name="life">��������</param>
	///	<param name="position">�������W</param>
	///	<param name="velocity">���x</param>
	///	<param name="accel">�����x</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velociy, XMFLOAT3 accel, float start_scale, float end_scale);

	static void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	static Camera* camera;
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };

	ConstBufferDataMaterial* constMapMaterial = nullptr;
};