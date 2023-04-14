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
	gameObject_->SetIsBonesWorldMatCalc(true);	//ボーンワールド行列計算あり
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
	oldIsRun = isRun;	//前フレーム処理
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



#pragma region 攻撃
	if (input_->TriggerKey(DIK_X))
	{
		isAtk = true;
		
	}

	if (isAtk == true)
	{
		atkCurrent++;
		animCT = 1;
		if (atkCurrent > 30)
		{	//30フレ目でアニメーション終わり
			gameObject_.get()->AnimFlameInter(atkCurrent , 30);
			animCT = 5;
			isAtk = false;
			atkCurrent = 0;
		}
	}
#pragma endregion 攻撃

#pragma region 防御行動
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
		counterFrameCount++;	//カウンター行動が開始してからのフレームカウント

		int max2AnimCT = 14;	//サマーソルトの最大フレーム0-14
		int max3AnimCT = 35;	//回転して力をためる最大フレーム14-40
		int max4AnimCT = 55;
		int max5AnimCT = 100;
		int max6AnimCT = 135;
		if (counterFrameCount == 1)
		{	//初フレーム
			atkMovePhase = 1;	//行動フェイズ
			animCT = 2;	//アニメーション番号

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
			//終了時処理
			atkMovePhase = 0;
			counterFrameCount = 0;
			isCounter = false;
			animCT = 5;
			kAccumulateRotVel = 0.0f;

			if (gameObject_.get()->GetIsAnimRot() == false)
			{	//もし繰り返しアニメーションが無効だった場合
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
			//突進行列計算
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

#pragma endregion 防御行動

#pragma region コライド
	spherePos = {
		gameObject_->wtf.matWorld_.m[3][0] ,
		gameObject_->wtf.matWorld_.m[3][1] ,
		gameObject_->wtf.matWorld_.m[3][2]
	};
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i]->Update();
	}


#pragma endregion コライド

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
	//速度を0にする
	velocity_ = {0 , 0 , 0};

	//キー入力があったら
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{
		isRun = true;

		faceAngle_ -= cameraAngle_;

		//Z軸方向にの速度を入れる
		velocity_ = {0 , 0 , kMoveSpeed_};

		//W,Dを押していたら
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{

			//45度方向に向くように回転させる
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

		//W,Aを押していたら
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A))
		{

			//135度方向に向くように回転させる
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

		//S,Dを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D))
		{

			//315度方向に向くように回転させる
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

		//S,Aを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A))
		{

			//225度方向に向くように回転させる
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

		//Wを押していたら
		else if (input_->PushKey(DIK_W))
		{

			//0度方向に向くように回転させる
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

		//Sを押していたら
		else if (input_->PushKey(DIK_S))
		{

			//180度方向に向くように回転させる
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

		//Dを押していたら
		else if (input_->PushKey(DIK_D))
		{

			//90度方向に向くように回転させる
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

		//Aを押していたら
		else if (input_->PushKey(DIK_A))
		{
			//270度方向に向くように回転させる
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