#include "Boss.h"
#include "FbxPlayer.h"
void Boss::Initialize(DirectXCommon* dxcomon)
{
	dxcomon_ = dxcomon;
	input_ = Input::GetInstance();

	FBXObject3d::SetDevice(dxcomon_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//ボスのFBX読み込み
	bossFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("Boss"));
	bossFbxO_ = std::make_unique<FBXObject3d>();
	bossFbxO_->Initialize();
	bossFbxO_->SetModel(bossFbxM_.get());
	bossFbxO_->PlayAnimation(0);
	bossFbxO_->SetPosition({ 0,0,30 });
	bossFbxO_->SetIsBonesWorldMatCalc(true);	//ボーンワールド行列計算あり
	bossFbxO_->Update();

	bossModel_ = Mesh::LoadFormOBJ("enemy", true);
	bossObj_ = Object3d::Create();

	bossObj_->SetModel(bossModel_.get());
	bossObj_->worldTransform.translation_ = { 0,0,30 };
	
	//testobj
	bossObj_ = Object3d::Create();
	ico_ = Mesh::LoadFormOBJ("ico", true);
	//testObj_->SetModel(ico_.get());

	//当たり判定用
	SPHERE_COLISSION_NUM = bossFbxO_->GetBonesMatPtr()->size();
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);
	bossFbxO_.get()->isBonesWorldMatCalc = true;	//ボーンの行列を取得するか


	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = bossFbxO_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->Update();
		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMIES);
		//test


	}
}

void Boss::Update()
{

	srand(time(nullptr));
	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->isDead(); });

	for (std::unique_ptr<BossBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


	if (mode > 1)
	{
		mode = 0;
	}


	switch (mode)
	{
	case BattleMode::Run:
		RunAway();
		if (input_->TriggerKey(DIK_RETURN))
		{
			mode++;
			Reset();
		}
		break;

	case BattleMode::Ground:

		if (isAttack == false)
		{
			attackValue = AttackValue::nasi;
			attackTimer--;
		}
		if (attackTimer <= 0)
		{
			isAttack = true;
			attackValue = rand() % 2 + 1;
			/*attackValue = AttackValue::TACKLE;
			attackValue = AttackValue::FIRE;*/
			attackTimer = 240.0f;
		}


		if (attackValue == AttackValue::FIRE)
		{
			if (isFire == false)
			{
				isFire = true;
			}
			Shot();

		}

		else if (attackValue == AttackValue::TACKLE)
		{
			Tackle();
		}

		if (input_->TriggerKey(DIK_RETURN))
		{
			mode++;
			Reset();
		}
		if (input_->TriggerKey(DIK_0))
		{
			if (isFire == false)
			{
				isFire = true;
			}
			else
			{
				isFire = false;
			}
		}
		break;
	}
	/*Tackle();

	Move();*/

	//bossObj_->Update();
	bossFbxO_->Update();

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = bossFbxO_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->Update();
	}
}

void Boss::Draw()
{
	/*if (bossObj_->position.z < 60)
	{
		bossObj_->Draw();
	}*/
	//bossObj_->Draw();
	bossFbxO_->Draw(dxcomon_->GetCommandList());

	for (std::unique_ptr<BossBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}

}

void Boss::RunAway()
{
	float speed = 1.0f;

	bossObj_->worldTransform.translation_.z += speed;
	bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

}

void Boss::Move()
{
	if (input_->PushKey(DIK_SPACE))
	{
		float speed = 0.5f;

		Vector3 playerPos = player_->GetObject3d()->GetWorldTransform().translation_;

		Vector3 bossPos = bossObj_->GetPosition();

		distance = playerPos - bossPos;

		//向きをキャラのいる方向を常に正面にする
		angle = -(atan2(distance.x, distance.z) + 3.141592f / 2);

		bossObj_->worldTransform.rotation_.y = (angle + 3.141592f / 2) * -1;;

		//一定の距離まで近づく処理
		if ((5.0f + 5.0f) * (5.0f + 5.0f) <= (bossObj_->worldTransform.translation_.x - player_->GetObject3d()->GetWorldTransform().translation_.x) * (bossObj_->worldTransform.translation_.x - player_->GetObject3d()->GetWorldTransform().translation_.x) +
			(bossObj_->worldTransform.translation_.y - player_->GetObject3d()->GetWorldTransform().translation_.y) * (bossObj_->worldTransform.translation_.y - player_->GetObject3d()->GetWorldTransform().translation_.y) +
			(bossObj_->worldTransform.translation_.z - player_->GetObject3d()->GetWorldTransform().translation_.z) * (bossObj_->worldTransform.translation_.z - player_->GetObject3d()->GetWorldTransform().translation_.z))
		{

			normDis = distance.nomalize();
			distance.x = normDis.x * speed;
			distance.z = normDis.z * speed;

			bossObj_->worldTransform.translation_.x += distance.x;
			bossObj_->worldTransform.translation_.z += distance.z;

		}
	}

}

void Boss::Shot()
{
	Vector3 playerPos;
	Vector3 bulletPos;
	Vector3 distance;

	distance - playerPos - bulletPos;

	bulletTimer--;
	if (bulletTimer <= 0)
	{
		if (isFire == true)
		{
			bulletPos = GetPosition();

			// 弾を生成し初期化
			std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
			newBullet->Initialize(bossObj_->worldTransform.translation_);

			bulletTimer = 60.0f;
			bulletCount++;
			//
			newBullet->SetPlayer(player_);

			bullets_.push_back(std::move(newBullet));


		}

	}
	if (bulletCount >= 3)
	{
		isAttack = false;
		isFire = false;
		bulletCount = 0;
	}
	/*else
	{
		playerPos = player_->GetPosition();
	}*/
	if (isFire == false)
	{
		playerPos = player_->GetObject3d()->GetWorldTransform().translation_;
	}
}

void Boss::Tackle()
{

	/*if (tackleDis.x == 0 || tackleDis.z == 0)
	{
		isTackle == false;
	}*/

	float speed = 0.1f;


	Vector3 bossPos = bossObj_->GetPosition();
	Vector3 bossPosO_ = bossFbxO_.get()->GetWorldTransform().translation_;

	tackleDis = playerPos - bossPos;

	if (isTackle == false)
	{

		moveTimer--;
	}
	if (moveTimer < 0)
	{

		if (isTackle == false)
		{
			isTackle = true;
			moveTimer = 60.0f;

		}
	}
	if (isTackle == true)
	{
		stopTimer--;
	}
	if (stopTimer < 0)
	{
		isTackle = false;

		stopTimer = 60.0f;
		tackleCount++;
	}
	if (tackleCount < 3)
	{
		if (isTackle == false)
		{

			/*if (playerPos.x == 0)
			{
				playerPos.z *= 1.5f;
			}*/

			/*wallPos = player_->GetPosition();

			if (playerPos.x == 0)
			{
				wallPos.x *= 1.5f;
			}
			else if (playerPos.x == 0)
			{
				wallPos.z *= 1.5f;
			}
			else
			{
				wallPos.x *= 1.5f;
				wallPos.z *= 2.0f;
			}*/

			playerPos = player_->GetObject3d()->GetWorldTransform().translation_;
			//向きをキャラのいる方向を常に正面にする
			angle = (atan2(tackleDis.x, tackleDis.z) + 3.141592f / 2);

			bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
		}
		if (isTackle == true)
		{



			tackleDis *= speed;

			bossObj_->worldTransform.translation_.x += tackleDis.x;
			bossObj_->worldTransform.translation_.y += tackleDis.y;
			bossObj_->worldTransform.translation_.z += tackleDis.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

			tackleDis.x = std::abs(tackleDis.x);
			tackleDis.z = std::abs(tackleDis.z);

			/*normTdis = tackleDis.nomalize();*/

		}

	}
	else
	{
		isAttack = false;
		tackleCount = 0;
	}



}

void Boss::Reset()
{

	bossObj_->worldTransform.rotation_ = { 0,0,0 };
	bossObj_->worldTransform.translation_ = { 0,0,30 };

	moveTimer = 60.0f;
	stopTimer = 60.0f;

	bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

}





Vector3 Boss::GetPosition()
{
	return bossObj_->worldTransform.translation_;
}

Boss* Boss::GetInstance()
{
	static Boss instance;
	return &instance;
}

FBXObject3d* Boss::GetObject3d()
{
	return bossFbxO_.get();
}
