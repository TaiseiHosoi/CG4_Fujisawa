#include "FbxPlayer.h"
#include "Ease.h"
#include "GameCamera.h"


FbxPlayer::FbxPlayer()
{

}

FbxPlayer::~FbxPlayer()
{

}

void FbxPlayer::Initialize(FBXModel* fbxModel)
{

	input_ = Input::GetInstance();

	gameObject_ = FBXObject3d::Create();
	gameObject_->SetModel(fbxModel);
	gameObject_->SetIsBonesWorldMatCalc(true);	//�{�[�����[���h�s��v�Z����
	gameObject_->Update();


	SPHERE_COLISSION_NUM = 1;
	sphere.resize(SPHERE_COLISSION_NUM);
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		sphere[i]->SetBasisPos(&spherePos);
		sphere[i]->SetRadius(1.5f);
		sphere[i]->Update();
		sphere[i]->SetAttribute(COLLISION_ATTR_ALLIES);

	}





}

void FbxPlayer::Update()
{
	oldIsRun = isRun;	//�O�t���[������
	oldAnimCT = animCT;

	cameraAngle_.y =
		atan2(gameObject_.get()->GetCamera().GetTarget().x - gameObject_.get()->GetCamera().GetEye().x ,
			  gameObject_.get()->GetCamera().GetTarget().z - gameObject_.get()->GetCamera().GetEye().z);

	if (isCounter == false)
	{
		Move();
		if (isRun == true)
		{
			nowCTFlame += 1;
			if (nowCTFlame >= 20)
			{
				nowCTFlame = 0;
			}
			gameObject_.get()->AnimFlameInter(nowCTFlame , 20);
			animCT = 0;

		}
		else
		{
		}

		if (oldIsRun == true && isRun == false)
		{
			isbrake = true;
		}

		if (isbrake == true)
		{
			animCT = 5;
			brakeFlameCount++;
			if (brakeFlameCount > 10)
			{
				isbrake = false;
				brakeFlameCount = 0;
				animCT = 5;
			}
		}
	}



#pragma region �U��
	if (input_->TriggerKey(DIK_X))
	{
		isAtk = true;
		
	}

	if (isAtk == true)
	{
		atkCurrent++;
		animCT = 1;
		if (atkCurrent > 30)
		{	//30�t���ڂŃA�j���[�V�����I���
			gameObject_.get()->AnimFlameInter(atkCurrent , 30);
			animCT = 5;
			isAtk = false;
			atkCurrent = 0;
		}
	}
#pragma endregion �U��

#pragma region �h��s��
	if (input_->TriggerKey(DIK_C) && isCounter == false)
	{
		isCounter = true;
		nowCTFlame = 0;
		GameCamera::SetShakePrimST(6.0f, 5.0f, true);
	}

	if (isCounter == true)
	{
		if (gameObject_.get()->GetIsAnimRot() == true)
		{
			gameObject_.get()->AnimIsRotateChange();
		}
		counterFrameCount++;	//�J�E���^�[�s�����J�n���Ă���̃t���[���J�E���g

		int max2AnimCT = 14;	//�T�}�[�\���g�̍ő�t���[��0-14
		int max3AnimCT = 35;	//��]���ė͂����߂�ő�t���[��14-40
		int max4AnimCT = 55;
		int max5AnimCT = 100;
		int max6AnimCT = 135;
		if (counterFrameCount == 1)
		{	//���t���[��
			atkMovePhase = 1;	//�s���t�F�C�Y
			animCT = 2;	//�A�j���[�V�����ԍ�

		}
		else if (counterFrameCount == max2AnimCT)
		{
			atkMovePhase = 2;
			nowCTFlame = 0;
			animCT = 3;
		}
		else if (counterFrameCount == max3AnimCT)
		{
			atkMovePhase = 3;

		}
		else if (counterFrameCount == max4AnimCT)
		{
			atkMovePhase = 4;
			nowCTFlame = 10;
			animCT = 3;
			gameObject_.get()->wtf.rotation_.x = 0;


		}
		else if (counterFrameCount == max5AnimCT)
		{
			atkMovePhase = 5;
			nowCTFlame = 10;
			animCT = 2;

		}
		else if (counterFrameCount == max6AnimCT)
		{
			//�I��������
			atkMovePhase = 0;
			counterFrameCount = 0;
			isCounter = false;
			animCT = 5;
			kAccumulateRotVel = 0.0f;

			if (gameObject_.get()->GetIsAnimRot() == false)
			{	//�����J��Ԃ��A�j���[�V�����������������ꍇ
				gameObject_.get()->AnimIsRotateChange();
			}
		}

		//
		if (atkMovePhase == 0)
		{
			gameObject_.get()->wtf.translation_.y = 0;
		}
		else if (atkMovePhase == 1)
		{
			if (nowCTFlame < 10)
			{
				nowCTFlame += 1;
			}
			gameObject_.get()->wtf.translation_.y = Ease::InQuad(7.0 , 0.0 , max2AnimCT , counterFrameCount);
			gameObject_.get()->AnimFlameInter(nowCTFlame , 10);

		}
		else if (atkMovePhase == 2)
		{
			if (nowCTFlame < 15)
			{
				nowCTFlame += 1;
			}
			kAccumulateRotVel += 0.05f;
			//gameObject_.get()->wtf.translation_.y = Ease::InQuad(8.0, 7.0, max2AnimCT, counterFrameCount - max2AnimCT);
			gameObject_.get()->wtf.rotation_.x += kAccumulateRotVel;
			gameObject_.get()->AnimFlameInter(nowCTFlame , 30);
		}
		else if (atkMovePhase == 3)
		{

			kAccumulateRotVel += 0.14f;
			gameObject_.get()->wtf.rotation_.x += kAccumulateRotVel;
			gameObject_.get()->AnimFlameInter(10 , 20);
		}
		else if (atkMovePhase == 4)
		{
			Vector3 kATKSpeedVel = {0 , 0 , 1.0f};
			//�ːi�s��v�Z
			Matrix4 tackleMat = MathFunc::Rotation(Vector3(0.0f , cameraAngle_.y , 0.0f) , 2);
			Vector3 kTackleVel = MathFunc::bVelocity(kATKSpeedVel , tackleMat);
			gameObject_.get()->wtf.rotation_.x += kAccumulateRotVel;
			gameObject_.get()->wtf.translation_ += kTackleVel;
			gameObject_.get()->AnimFlameInter(10 , 20);

		}
		else if (atkMovePhase == 5)
		{
			if (nowCTFlame < 60)
			{
				nowCTFlame += 1;
			}

			gameObject_.get()->wtf.translation_.y = Ease::InQuad(10.0 , 5.0 , max6AnimCT - max5AnimCT , max6AnimCT - counterFrameCount);
			gameObject_.get()->wtf.translation_.x -= cameraAngle_.x * 0.1f;
			gameObject_.get()->wtf.translation_.z -= cameraAngle_.z * 0.1f;

			gameObject_.get()->AnimFlameInter(nowCTFlame , 60);
		}

	}

#pragma endregion �h��s��

#pragma region �R���C�h
	spherePos = {
		gameObject_->wtf.matWorld_.m[3][0] ,
		gameObject_->wtf.matWorld_.m[3][1] ,
		gameObject_->wtf.matWorld_.m[3][2]
	};
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i]->Update();
	}


#pragma endregion �R���C�h

	ImGui::Begin("fbxPlayer");

	ImGui::InputFloat("jikipos:y" , &gameObject_->wtf.translation_.y);
	ImGui::InputInt("atkMovePhase" , &atkMovePhase);
	ImGui::InputInt("currentFlame" , &counterFrameCount);
	ImGui::InputInt("currentFlame" , &counterFrameCount);



	ImGui::End();

	gameObject_->Update();

	if (oldAnimCT != animCT)
	{
		gameObject_.get()->PlayAnimation(animCT);
	}

}

void FbxPlayer::Draw(ID3D12GraphicsCommandList* cmdList)
{

	gameObject_->Draw(cmdList);

}

FBXObject3d* FbxPlayer::GetObject3d()
{
	return gameObject_.get();
}

void FbxPlayer::Move()
{
	//���x��0�ɂ���
	velocity_ = {0 , 0 , 0};

	//�L�[���͂���������
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{
		isRun = true;

		faceAngle_ -= cameraAngle_;

		//Z�������ɂ̑��x������
		velocity_ = {0 , 0 , kMoveSpeed_};

		//W,D�������Ă�����
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{

			//45�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(45))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
					faceAngle_.y <= MathFunc::Dig2Rad(225))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(45) ||
						faceAngle_.y >= MathFunc::Dig2Rad(225))
					{
						faceAngle_.y = MathFunc::Dig2Rad(45);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
						faceAngle_.y <= MathFunc::Dig2Rad(225))
					{
						faceAngle_.y = MathFunc::Dig2Rad(45);
					}
				}
			}

		}

		//W,A�������Ă�����
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A))
		{

			//135�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(315))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(125) &&
					faceAngle_.y <= MathFunc::Dig2Rad(315))
				{

					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(125) ||
						faceAngle_.y >= MathFunc::Dig2Rad(315))
					{
						faceAngle_.y = MathFunc::Dig2Rad(315);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(125) &&
						faceAngle_.y <= MathFunc::Dig2Rad(315))
					{
						faceAngle_.y = MathFunc::Dig2Rad(315);
					}
				}
			}

		}

		//S,D�������Ă�����
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D))
		{

			//315�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(135))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(315) &&
					faceAngle_.y >= MathFunc::Dig2Rad(135))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(135) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(135);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(135) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(135);
					}
				}
			}

		}

		//S,A�������Ă�����
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A))
		{

			//225�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(225))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
					faceAngle_.y <= MathFunc::Dig2Rad(225))
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(225) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(225);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(225) >= faceAngle_.y &&
						faceAngle_.y >= MathFunc::Dig2Rad(45))
					{
						faceAngle_.y = MathFunc::Dig2Rad(225);
					}

				}
			}

		}

		//W�������Ă�����
		else if (input_->PushKey(DIK_W))
		{

			//0�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(0))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(180))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(0) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(0);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(360) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(0);
					}
				}
			}
		}

		//S�������Ă�����
		else if (input_->PushKey(DIK_S))
		{

			//180�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(180))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(180))
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(180) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(180);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(180) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(180);
					}
				}
			}
		}

		//D�������Ă�����
		else if (input_->PushKey(DIK_D))
		{

			//90�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(90))
			{
				if (faceAngle_.y != MathFunc::Dig2Rad(90))
				{
					if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
						faceAngle_.y <= MathFunc::Dig2Rad(270))
					{
						faceAngle_.y -= kTurnSpeed_;

						if (faceAngle_.y <= MathFunc::Dig2Rad(90) ||
							MathFunc::Dig2Rad(270) <= faceAngle_.y)
						{
							faceAngle_.y = MathFunc::Dig2Rad(90);
						}

					}
					else
					{
						faceAngle_.y += kTurnSpeed_;

						if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
							faceAngle_.y <= MathFunc::Dig2Rad(270))
						{
							faceAngle_.y = MathFunc::Dig2Rad(90);
						}
					}
				}
			}
		}

		//A�������Ă�����
		else if (input_->PushKey(DIK_A))
		{
			//270�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(270))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
					faceAngle_.y <= MathFunc::Dig2Rad(270))
				{
					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(90) ||
						MathFunc::Dig2Rad(270) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(270);
					}

				}
				else
				{
					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
						MathFunc::Dig2Rad(270) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(270);
					}
				}
			}
		}

		if (MathFunc::Dig2Rad(360) < faceAngle_.y)
		{
			faceAngle_.y += -MathFunc::Dig2Rad(360);
		}
		if (faceAngle_.y < 0)
		{
			faceAngle_.y += MathFunc::Dig2Rad(360);
		}

		faceAngle_ += cameraAngle_;
	}
	else
	{
		isRun = false;
	}
	gameObject_->wtf.rotation_ = faceAngle_;

	gameObject_->wtf.UpdateMatWorld();

	velocity_ = MathFunc::MatVector(velocity_ , gameObject_->wtf.matWorld_);

	gameObject_->wtf.translation_ += velocity_;
}