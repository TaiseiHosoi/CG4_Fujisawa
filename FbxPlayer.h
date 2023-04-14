#pragma once
#include "Object3d.h"
#include "FBXObject3d.h"
#include "FBXModel.h"
#include "input.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class FbxPlayer
{
public:
	//�R���X�g���N�^
	FbxPlayer();

	//�f�X�g���N�^
	~FbxPlayer();

	//������
	void Initialize(FBXModel* model);

	//�X�V
	void Update();

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	FBXObject3d* GetObject3d();

private:
	//�ړ�
	void Move();

private:

	//����
	Input* input_ = nullptr;

	//�I�u�W�F�N�g
	std::unique_ptr<FBXObject3d> gameObject_;
	int SPHERE_COLISSION_NUM;	//�R���C�_�[�i�X�t�B�A�j�̐�
	std::vector<Matrix4>* collisionBonesMat;	//�����蔻��p�̃{�[���̃��[���h�s��
	std::vector<SphereCollider*> sphere;
	Vector3 spherePos = {};
	


	//�ړ����x
	const float kMoveSpeed_ = 0.5;
	//���񑬓x
	const float kTurnSpeed_ = MathFunc::Dig2Rad(10);
	//�ړ��x�N�g��
	Vector3 velocity_;
	//���@�̌���
	Vector3 faceAngle_ = {0 , 0 , 0};
	//�J�����̌���
	Vector3 cameraAngle_ = {0 , 0 , 0};
	//�����Ă��邩�t���O
	bool isRun = false;
	bool oldIsRun = false;
	//�u���[�L�A�j���[�V�����p�t���O
	bool isbrake = false;
	int brakeFlameCount = 0;
	//�A�j���[�V�������Ԗڂ�
	int animCT = 2;
	int oldAnimCT = 0;
	//�U���t���O
	bool isAtk = false;

	//�U���p�t���\m
	int atkCurrent = 0;

	//�h�䎞�s��
	//�����t���O�I���������J�ڃ��[�V�����������������t���O�I�t
	bool isCounter = false;	//�����t���O
	int atkMovePhase = 0;	//�U���p�t�F�[�Y
	int counterFrameCount = 0;	//�S�̃t���[��
	int nowCTFlame = 0;	//�A�j���[�V�����p�t���[���J�E���g
	float kAccumulateRotVel = 0;


};