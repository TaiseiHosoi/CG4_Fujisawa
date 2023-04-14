#include "BossBullet.h"
#include "FbxPlayer.h"
#include <random>
void BossBullet::Initialize(const Vector3& position)
{
#pragma region//���������̉�����
	//�����V�[�h������
	std::random_device seed_gen;
	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	std::mt19937_64 engine(seed_gen());
	//�����͈͂̐ݒ�
	std::uniform_real_distribution<float> accuaryDist(0.0, 2.0);
	std::uniform_real_distribution<float> wayDist(0, 4.0);
	std::uniform_real_distribution<float> timeDist(0, 30.0);

#pragma endregion
	//���x�̌���
	homingAccuary_ = accuaryDist(engine);

	//��]�s���p��
	Matrix4 matRot = matRot.identity();
	Matrix4 matRot2 = matRot.identity();

	//��]�x�N�g����p��
	Vector3 rotation = {
		MathFunc::Dig2Rad((wayDist(engine) - 2) * 20),
		MathFunc::Dig2Rad((wayDist(engine) - 2) * 20),
		0 };

	//��]�x�N�g����p��
	Vector3 rotation2 = { 0,
		MathFunc::Dig2Rad((wayDist(engine) - 2) * 20),
		0 };
	//��]�s��ɉ�]�x�N�g���𔽉f
	matRot = MathFunc::Rotation(rotation, 6);
	matRot2 = MathFunc::Rotation(rotation2, 2);
	//�A���O���Ɖ�]�s��̐Ȃ����߂�
	angle = MathFunc::bVelocity(angle, matRot2);

	kFireTimer = 0;
	/*kStartHomingTime = timeDist(engine);*/

#pragma region Initlialize��

	bulletModel_ = Mesh::LoadFormOBJ("cube", true);
	bulletObj_ = Object3d::Create();

	bulletObj_->SetModel(bulletModel_.get());

	bulletObj_->worldTransform.translation_ = position;


#pragma endregion
}

void BossBullet::Update()
{
	livingBullet--;

	if (livingBullet <= 0)
	{
		SetIsDead(true);
		livingBullet = 180.0f;
	}

	switch (phase)
	{
	case ShotPhase::Fire:
		Fire2();
		break;

	case ShotPhase::Homing:

		/*if (player_->GetIsDead() == false)
		{
			Homing2();
		}*/

		break;
	}

	bulletObj_->worldTransform.translation_ += velocity_;

	bulletObj_->Update();
}

void BossBullet::Draw()
{
	bulletObj_->Draw();
}

Vector3 BossBullet::GetPosition()
{
	return Vector3();
}

void BossBullet::Fire()
{
	kFireTimer++;

	if (kStartHomingTime <= kFireTimer) {
		phase = ShotPhase::Homing;
	}

	//�A���O������e���i�ރx�N�g�����Z�o
	velocity_ = {
		angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.y / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
	};
}

void BossBullet::Homing()
{
	//�^�C�}�[�̌���
	kHomingTimer_--;

	//�^�C�}�[��0�ȉ��̎�
	if (kHomingTimer_ > 0) {

		targetPos_ = player_->GetObject3d()->GetWorldTransform().translation_;

		////�z�[�~���O���x�ɍ��킹�ă^�C�}�[�����Z�b�g
		//kHomingTimer_ = homingAccuary_ * 5;
	}

	//�e�̃A���O���ƒe����G�܂ł̃x�N�g���̊O�ς��o��
	Vector3 cross = angle.cross(targetPos_.sub(bulletObj_->worldTransform.translation_));

	//��]�s���p��
	Matrix4 matRot = matRot.identity();

	//��]�x�N�g����p��
	Vector3 rotation = { 0 , 0 , 0 };

	//���܂����O�ς̐����ɂ���ĉ�]�x�N�g���𑝌�������
	if (cross.x < 0) {
		rotation.x -= MathFunc::Dig2Rad(homingRotateValue_);
	}
	else if (0 < cross.x) {
		rotation.x += MathFunc::Dig2Rad(homingRotateValue_);
	}

	if (cross.y < 0) {
		rotation.y -= MathFunc::Dig2Rad(homingRotateValue_);
	}
	else if (0 < cross.y) {
		rotation.y += MathFunc::Dig2Rad(homingRotateValue_);
	}

	if (cross.z < 0) {
		rotation.z -= MathFunc::Dig2Rad(homingRotateValue_);
	}
	else if (0 < cross.z) {
		rotation.z += MathFunc::Dig2Rad(homingRotateValue_);
	}

	//��]�s��ɉ�]�x�N�g���𔽉f
	matRot = MathFunc::Rotation(rotation, 6);

	//�A���O���Ɖ�]�s��̐Ȃ����߂�
	angle = MathFunc::bVelocity(angle, matRot);

	//�A���O������e���i�ރx�N�g�����Z�o
	velocity_ = {
		angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.y / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
	};

}

void BossBullet::Fire2()
{
	kFireTimer++;

	if (kStartHomingTime <= kFireTimer) {
		phase = ShotPhase::Homing;
	}

	velocity_ = {
	angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
	1 * kBulletSpeed,
	angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
	};

}

void BossBullet::Homing2()
{
	//�^�C�}�[�̌���
	kHomingTimer_--;

	//�^�C�}�[��0�ȉ��̎�
	if (kHomingTimer_ > 0) {

		targetPos_ = player_->GetObject3d()->GetWorldTransform().translation_;


		//�e�̃A���O���ƒe����G�܂ł̃x�N�g���̊O�ς��o��
		Vector3 cross = angle.cross(targetPos_.sub(bulletObj_->worldTransform.translation_));

		//��]�s���p��
		Matrix4 matRot = matRot.identity();

		//��]�x�N�g����p��
		Vector3 rotation = { 0 , 0 , 0 };

		//���܂����O�ς̐����ɂ���ĉ�]�x�N�g���𑝌�������
		if (cross.x < 0) {
			rotation.x -= MathFunc::Dig2Rad(homingRotateValue_);
		}
		else if (0 < cross.x) {
			rotation.x += MathFunc::Dig2Rad(homingRotateValue_);
		}

		if (cross.y < 0) {
			rotation.y -= MathFunc::Dig2Rad(homingRotateValue_);
		}
		else if (0 < cross.y) {
			rotation.y += MathFunc::Dig2Rad(homingRotateValue_);
		}

		if (cross.z < 0) {
			rotation.z -= MathFunc::Dig2Rad(homingRotateValue_);
		}
		else if (0 < cross.z) {
			rotation.z += MathFunc::Dig2Rad(homingRotateValue_);
		}

		//��]�s��ɉ�]�x�N�g���𔽉f
		matRot = MathFunc::Rotation(rotation, 6);

		//�A���O���Ɖ�]�s��̐Ȃ����߂�
		angle = MathFunc::bVelocity(angle, matRot);

		////�z�[�~���O���x�ɍ��킹�ă^�C�}�[�����Z�b�g
		//kHomingTimer_ = homingAccuary_ * 5;
	}

	//�A���O������e���i�ރx�N�g�����Z�o
	velocity_ = {
		angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.y / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
	};

}
