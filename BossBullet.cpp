#include "BossBullet.h"
#include "FbxPlayer.h"
#include <random>
void BossBullet::Initialize(const Vector3& position)
{
#pragma region//乱数生成の下準備
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の設定
	std::uniform_real_distribution<float> accuaryDist(0.0, 2.0);
	std::uniform_real_distribution<float> wayDist(0, 4.0);
	std::uniform_real_distribution<float> timeDist(0, 30.0);

#pragma endregion
	//精度の決定
	homingAccuary_ = accuaryDist(engine);

	//回転行列を用意
	Matrix4 matRot = matRot.identity();
	Matrix4 matRot2 = matRot.identity();

	//回転ベクトルを用意
	Vector3 rotation = {
		MathFunc::Dig2Rad((wayDist(engine) - 2) * 20),
		MathFunc::Dig2Rad((wayDist(engine) - 2) * 20),
		0 };

	//回転ベクトルを用意
	Vector3 rotation2 = { 0,
		MathFunc::Dig2Rad((wayDist(engine) - 2) * 20),
		0 };
	//回転行列に回転ベクトルを反映
	matRot = MathFunc::Rotation(rotation, 6);
	matRot2 = MathFunc::Rotation(rotation2, 2);
	//アングルと回転行列の席を求める
	angle = MathFunc::bVelocity(angle, matRot2);

	kFireTimer = 0;
	/*kStartHomingTime = timeDist(engine);*/

#pragma region Initlialize等

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

	//アングルから弾が進むベクトルを算出
	velocity_ = {
		angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.y / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
	};
}

void BossBullet::Homing()
{
	//タイマーの減少
	kHomingTimer_--;

	//タイマーが0以下の時
	if (kHomingTimer_ > 0) {

		targetPos_ = player_->GetObject3d()->GetWorldTransform().translation_;

		////ホーミング精度に合わせてタイマーをリセット
		//kHomingTimer_ = homingAccuary_ * 5;
	}

	//弾のアングルと弾から敵までのベクトルの外積を出す
	Vector3 cross = angle.cross(targetPos_.sub(bulletObj_->worldTransform.translation_));

	//回転行列を用意
	Matrix4 matRot = matRot.identity();

	//回転ベクトルを用意
	Vector3 rotation = { 0 , 0 , 0 };

	//求まった外積の正負によって回転ベクトルを増減させる
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

	//回転行列に回転ベクトルを反映
	matRot = MathFunc::Rotation(rotation, 6);

	//アングルと回転行列の席を求める
	angle = MathFunc::bVelocity(angle, matRot);

	//アングルから弾が進むベクトルを算出
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
	//タイマーの減少
	kHomingTimer_--;

	//タイマーが0以下の時
	if (kHomingTimer_ > 0) {

		targetPos_ = player_->GetObject3d()->GetWorldTransform().translation_;


		//弾のアングルと弾から敵までのベクトルの外積を出す
		Vector3 cross = angle.cross(targetPos_.sub(bulletObj_->worldTransform.translation_));

		//回転行列を用意
		Matrix4 matRot = matRot.identity();

		//回転ベクトルを用意
		Vector3 rotation = { 0 , 0 , 0 };

		//求まった外積の正負によって回転ベクトルを増減させる
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

		//回転行列に回転ベクトルを反映
		matRot = MathFunc::Rotation(rotation, 6);

		//アングルと回転行列の席を求める
		angle = MathFunc::bVelocity(angle, matRot);

		////ホーミング精度に合わせてタイマーをリセット
		//kHomingTimer_ = homingAccuary_ * 5;
	}

	//アングルから弾が進むベクトルを算出
	velocity_ = {
		angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.y / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
	};

}
