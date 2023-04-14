#pragma once
#include "Object3d.h"
#include "Matrix4.h"
#include "Vector3.h"
class FbxPlayer;

enum ShotPhase
{
	Fire,
	Homing,
};



class BossBullet
{
private:

	std::unique_ptr<Object3d> bulletObj_;
	std::unique_ptr<Mesh> bulletModel_;
	bool isDead_ = false;

	FbxPlayer* player_ = nullptr;

#pragma region �z�[�~���O�֘A

	float kBulletSpeed = 1.0f;

	//���x
	Vector3 velocity_;

	//�z�[�~���O���x
	int homingAccuary_ = 0;

	int kFireTimer = 0;

	int kStartHomingTime = 30;

	int kHomingTimer_ = 60;

	//�z�[�~���O�̊p�x������
	const int homingRotateValue_ = 5;


	Vector3 angle = { 0,0,1 };

	Vector3 targetPos_;

	int phase = ShotPhase::Fire;

#pragma endregion	

	float livingBullet = 180.0f;

public:
	void Initialize(const Vector3& position);

	void Update();

	void Draw();

	Vector3 GetPosition();

	void SetPlayer(FbxPlayer* player) { player_ = player; }

#pragma region �ˌ��֐�

	void Fire();

	void Homing();

	//�ォ��̃z�[�~���O�e
	void Fire2();
	void Homing2();

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	bool isDead() { return isDead_; }

#pragma endregion

};
