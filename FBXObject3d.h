#pragma once
#include "FBXModel.h"
#include"fbx/FBXLoader.h"
#include"WorldTransform.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>





class FBXObject3d
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��p�j
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
	};
	//�{�[���̍ő吔
	static const int MAX_BONES = 320;

	//�萔�o�b�t�@�p�̃f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};


public: // �ÓI�����o�֐�
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	// setter
	static void SetDevice(ID3D12Device* device) { FBXObject3d::device = device; }
	static void SetCamera(Camera* camera) { FBXObject3d::camera = camera; }


private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �J����
	static Camera* camera;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;


public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	static std::unique_ptr<FBXObject3d>Create();


	/// <summary>
	/// �傫���̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetScale(const Vector3& scale) { this->wtf.scale_ = scale; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position) { this->wtf.translation_ = position; }

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rotate"></param>
	void SetRotate(const Vector3& rotate) { this->wtf.rotation_ = rotate; }

	/// <summary>
	/// ���f�����Z�b�g
	/// </summary>
	/// <param name="fbxmodel"></param>
	void SetModel(FBXModel* fbxmodel) { this->fbxmodel = fbxmodel; }

	/// <summary>
	/// �t���[���J�E���g�w��
	/// </summary>
	/// <param name="flame"></param>
	void SetFlame(int flame);

	/// <summary>
	/// �A�j���[�V�����Đ��p
	/// </summary>
	void AnimPlay();
	void AnimStop();
	void AnimIsRotateChange();

	/// <summary>
	/// �J�����̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	Camera GetCamera();

	/// <summary>
	/// �A�j���[�V�����^�C���Q�b�^�[
	/// </summary>
	/// <returns></returns>
	FbxTime GetCurrentTimer();
	FbxTime GetEndTime();
	bool GetIsAnimRot();
	int ConvertFbxTimeToInt(FbxTime time);	//FbxTime�^�ϐ���int�ɕϊ�

	/// <summary>
	/// ���[���h�g�����X�t�H�[���擾
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform();
	WorldTransform* GetWorldTransformPtr();

	/// <summary>
	/// �{�[���p
	/// </summary>
	std::vector<Matrix4> bonesMat;
	void ResizeBonesMat(std::vector<FBXModel::Bone> bones);	//�{�[���̃T�C�Y���N���X�^�[�{�[���ɍ��킹��
	bool isBonesWorldMatCalc = false;	//�{�[���̃��[���h���W��ł̌v�Z�����邩�ǂ���

	std::vector<Matrix4>* GetBonesMatPtr();	//�{�[���̃��[���h�s��|�C���^��n��
	void SetIsBonesWorldMatCalc(bool isCalc);	//�{�[���v�Z�t���O�̃Z�b�^�[

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation(int animationNum);

	//��ԃA�j���[�V�����J�E���g
	void AnimFlameInter(FbxTime nowCount, FbxTime maxCount);

	WorldTransform wtf;
	

protected: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	// �萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	//// ���[�J���X�P�[��
	//XMFLOAT3 scale = { 1,1,1 };
	//// X,Y,Z�����̃��[�J����]�p
	//XMFLOAT3 rotation = { 0,0,0 };
	//// ���[�J�����W
	//XMFLOAT3 position = { 0,0,0 };
	//// ���[�J�����[���h�ϊ��s��
	//XMMATRIX matWorld;
	// ���f��
	FBXModel* fbxmodel = nullptr;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	//�A�j���[�V�����t���[���w�莞�t���O
	bool isChangeFlame = false;
	//�A�j���[�V�����t���O
	bool isAnim = true;
	//�A�j���[�V�����J��Ԃ���
	bool animRot = true;
};