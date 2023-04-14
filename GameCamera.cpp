#include "GameCamera.h"
#include "Ease.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
float GameCamera::magnitude = 0.0f;
float GameCamera::duration = 0.0f;
bool GameCamera::isShake = 0.0f;

GameCamera::GameCamera(int window_width, int window_height, Input* input)
	: Camera(window_width, window_height)
{
	//���̓C���X�^���X�擾
	input_ = Input::GetInstance();

	//�J�����̏�����
	Vector3 eye = { 0.0f, 0.0f, -5.0f };
	Vector3 up = { 0, 1, 0 };
	Vector3 target = { 0, 0, 300.0f };
	this->SetEye(eye);
	this->SetUp(up);
	this->SetTarget(target);
}

void GameCamera::Initialize()
{
	
}

void GameCamera::Update()
{
	/*if (targetPos_) {
		Vector3  pos = {
			targetPos_->matWorld_.m[3][0] ,
			targetPos_->matWorld_.m[3][1] ,
			targetPos_->matWorld_.m[3][2]
		};
		SetTarget(pos);
	}

	if (eyePos_) {
		Vector3  pos = {
			eyePos_->matWorld_.m[3][0] ,
			eyePos_->matWorld_.m[3][1] ,
			eyePos_->matWorld_.m[3][2]
		};
		SetEye(pos);
	}*/

	if (isFollowPlayer_ == true) {

		if (input_->TriggerKey(DIK_F)) {	//�J�����̃��[�h�؂�ւ�
			if (cameraMode_ == 0) {
				cameraMode_ = 1;
			}
			else if (cameraMode_ == 1) {
				cameraMode_ = 0;
			}
			else {
				cameraMode_ = 0;
			}
		}

		//�J�����̈ʒu
		Vector3 eyeVec = followerPos_->translation_ - targetPos_->translation_;

		Vector3 eyePos = eyeVec;

		float mag = 1.0f;
		float eyeLen = eyePos.length();	//�x�N�g���̒���

		if (eyeLen > 1.0f) {	//���������̃x�N�g�����P�ʃx�N�g�����傫��������
			mag = 1.0f / eyeLen; //�x�N�g���̒�����1�ɂ���
		};

		eyePos.x *= mag;	//mag��������Ɛ��K�������
		eyePos.y *= mag;
		eyePos.z *= mag;

		if (cameraMode_ == 0) {
			if (cameraModeChangeCountTimer < MAX_CHANGE_TIMER) {
				cameraModeChangeCountTimer++;
			}
		}
		else if (cameraMode_ == 1) {
			if (cameraModeChangeCountTimer > 0) {
				cameraModeChangeCountTimer--;
			}
		}

		cameraDistance_ = Ease::InOutQuad(MIN_CAMERA_DISTANCE , MAX_CAMERA_DISTANCE , cameraModeChangeCountTimer , MAX_CHANGE_TIMER);
		cameraHeight_ = Ease::InOutQuad(3 , 6 , cameraModeChangeCountTimer , MAX_CHANGE_TIMER);


		Vector3 primalyCamera =
		{followerPos_->translation_.x + eyePos.x * cameraDistance_ ,//���@����������ʒu�ɃJ�������Z�b�g
			cameraHeight_ ,
			followerPos_->translation_.z + eyePos.z * cameraDistance_};

		float eyeVecAngle = atan2f(primalyCamera.x - targetPos_->translation_.x , primalyCamera.z - targetPos_->translation_.z);//�J���������炷�ۂɎg����

		float shiftLen = -5.0f;	//���炷��
		Vector3 shiftVec = {primalyCamera.x + sinf(eyeVecAngle + MathFunc::PI / 2) * shiftLen , primalyCamera.y , primalyCamera.z + cosf(eyeVecAngle + MathFunc::PI / 2) * shiftLen};

		ShakePrim();
		SetEye(shiftVec + loolAtPos);
		loolAtPos = { 0,0,0 };

		SetTarget({targetPos_->translation_.x,targetPos_->translation_.y+2.0f ,targetPos_->translation_.z });

	}
	//SetEye({0,5,-20});
	
	

	Camera::Update();
}

void GameCamera::SetTargetPos(WorldTransform* targetPos) {
	targetPos_ = targetPos;
}

WorldTransform* GameCamera::GetTargetPos()
{
	return targetPos_;
}

void GameCamera::RemoveTargetPos()
{
	targetPos_ = nullptr;
}

void GameCamera::SetEyePos(WorldTransform* eyePos) {
	eyePos_ = eyePos;
}

void GameCamera::SetFollowerPos(WorldTransform* wtf)
{
	followerPos_ = wtf;
}

WorldTransform* GameCamera::GetEyePos()
{
	return eyePos_;
}

void GameCamera::RemoveEyePos()
{
	eyePos_ = nullptr;
}

float GameCamera::randomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void GameCamera::ShakePrim()
{
	if (isShake == true) {

		if (elapsedTime < duration) {
			float offsetX = randomFloat(-1.0f, 1.0f) * magnitude;
			float offsetY = randomFloat(-1.0f, 1.0f) * magnitude;
			float offsetZ = randomFloat(-1.0f, 1.0f) * magnitude;

			loolAtPos = {offsetX, offsetY, offsetZ};

			elapsedTime += deltaTime;
		}
		else {
			elapsedTime = 0.0f;
			
			isShake = false;
		}

	}
	
}

void GameCamera::SetShakePrimST(float dura, float mag,bool isShakePrim)
{
	duration = dura;
	magnitude = mag;
	isShake = isShakePrim;
}
