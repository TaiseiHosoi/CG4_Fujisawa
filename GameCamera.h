#pragma once
#include"Camera.h"
#include"Input.h"
#include "Object3d.h"

class GameCamera :
	public Camera
{
public:
	//�R���X�g���N�^
	GameCamera(int window_width, int window_height, Input* input);
	void Initialize();
	void Update();
	void SetTargetPos(WorldTransform* targetPos);
	WorldTransform* GetTargetPos();
	void RemoveTargetPos();
	void SetEyePos(WorldTransform* eyePos);
	void SetFollowerPos(WorldTransform* wtf);
	WorldTransform* GetEyePos();
	void RemoveEyePos();

	//�V�F�C�N�p
	float randomFloat(float min, float max);	//�����_���������o��
	void ShakePrim();
	static void SetShakePrimST(float dura, float mag, bool isShakePrim);
	


private:
	Input* input_ = nullptr;
	WorldTransform* targetPos_ = nullptr;
	WorldTransform* eyePos_ = nullptr;
	WorldTransform* followerPos_ = nullptr;
	bool isFollowPlayer_ = true;

	//�J�������[�h(tekito-)
	int cameraMode_ = 0;
	//�J���������֌W
	const float  MAX_CAMERA_DISTANCE = 20.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	int cameraModeChangeCountTimer = 30;
	float cameraHeight_ = 6;
	//ShakePrim�p�ϐ�
	static float magnitude;	//�V�F�C�N�̋���
	static float duration;//�V�F�C�N���N��������
	static bool isShake;
	float elapsedTime = 0.0f;	
	const float deltaTime = 0.016;	//1.0f��1�b(60FPS)�ƍl�����ۂ�1F
	
	Vector3 loolAtPos = {};

};

