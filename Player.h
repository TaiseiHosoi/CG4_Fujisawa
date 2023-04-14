#pragma once
#include "Object3d.h"
#include "input.h"
#include "BaseCollider.h"

class Player
{
public:
	//�R���X�g���N�^
	Player();

	//�f�X�g���N�^
	~Player();

	//������
	void Initialize(Mesh* model);

	//�X�V
	void Update();

	void Draw();

	void OnCollision(const CollisionInfo& info);

	Object3d* GetObject3d();

private:
	//�ړ�
	void Move();

private:

	//����
	Input* input_ = Input::GetInstance();

	std::unique_ptr<Object3d>gameObject_;

	//�ړ����x
	const float kMoveSpeed_ = 0.5;
	//���񑬓x
	const float kTurnSpeed_ = MathFunc::Dig2Rad(45);
	//�ړ��x�N�g��
	Vector3 velocity_;
	//���@�̌���
	Vector3 faceAngle_ = {0 , 0 , 0};
	//�J�����̌���
	Vector3 cameraAngle_ = {0 , 0 , 0};

	BaseCollider* collider_ = nullptr;

	bool onGround = true;

	Vector3 fallVec;

};