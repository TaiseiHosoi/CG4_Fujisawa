#include "Player.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"


Player::Player()
{

}

Player::~Player()
{
	delete collider_;
}

void Player::Initialize(Mesh* model)
{

	gameObject_ = Object3d::Create();
	gameObject_->SetModel(model);

	gameObject_->SetPosition({0 , 5 , 0});
	gameObject_->Update();

	collider_ = new SphereCollider;
	CollisionManager::GetInstance()->AddCollider(collider_);
	collider_->SetObject3d(gameObject_.get());
	collider_->Update();
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);
}

void Player::Update()
{
	cameraAngle_.y =
		atan2(gameObject_->camera_->GetTarget().x - gameObject_->camera_->GetEye().x ,
			  gameObject_->camera_->GetTarget().z - gameObject_->camera_->GetEye().z);
	Move();

	if (!onGround)
	{
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;

		fallVec.y = max(fallVec.y + fallAcc , fallVYMin);

		gameObject_->worldTransform.translation_ += fallVec;
	}

	else if (input_->TriggerKey(DIK_SPACE))
	{
		onGround = false;
		const float jumpVYFirst = 0.2f;
		fallVec.y = jumpVYFirst;
	}

	gameObject_->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider_);
	assert(sphereCollider);

	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.y += sphereCollider->GetRadius();
	ray.dir = {0 , -1 , 0};
	RaycastHit raycastHit;

	if (onGround)
	{
		const float adsDistance = 0.2f;

		if (CollisionManager::GetInstance()->Raycast(ray , COLLISION_ATTR_LANDSPHERE ,
			&raycastHit , sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround = true;
			gameObject_->worldTransform.translation_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);

			gameObject_->Update();

		}
		else
		{

			onGround - false;
			fallVec = {};

		}
	}
	else if (fallVec.y <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray , COLLISION_ATTR_LANDSPHERE ,
			&raycastHit , sphereCollider->GetRadius() * 2.0f))
		{
			onGround = true;
			gameObject_->worldTransform.translation_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);

			gameObject_->Update();

		}
	}

	if (collider_->GetIsHit())
	{
		OnCollision(collider_->GetCollisionInfo());
	}
	collider_->Update();

}

void Player::Draw()
{
	gameObject_->Draw();
}

void Player::OnCollision(const CollisionInfo& info)
{
	ImGui::Begin("Collision");
	ImGui::SetWindowSize({500 , 100});

	ImGui::Text("hit");

	ImGui::End();
}


Object3d* Player::GetObject3d()
{
	return gameObject_.get();
}

void Player::Move()
{
	//���x��0�ɂ���
	velocity_ = {0 , 0 , 0};

	//�L�[���͂���������
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{

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
	gameObject_->worldTransform.rotation_ = faceAngle_;

	gameObject_->worldTransform.UpdateMatWorld();

	velocity_ = MathFunc::MatVector(velocity_ , gameObject_->worldTransform.matWorld_);

	gameObject_->worldTransform.translation_ += velocity_;
}