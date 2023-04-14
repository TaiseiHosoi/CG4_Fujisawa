#include "TestLand.h"
#include "MeshCollider.h"
#include "CollisionManager.h"

TestLand::TestLand()
{

}

TestLand::~TestLand()
{

}

void TestLand::Initialize(Mesh* model)
{

	gameObject_ = Object3d::Create();
	gameObject_->SetModel(model);

	MeshCollider* collider = new MeshCollider;
	collider->ConstructTriangles(model);

	collider_ = collider;
	CollisionManager::GetInstance()->AddCollider(collider_);
	collider_->SetObject3d(gameObject_.get());
	collider_->Update();
	collider_->SetAttribute(COLLISION_ATTR_LANDSPHERE);

}

void TestLand::Update()
{
	gameObject_->Update();
}

void TestLand::Draw()
{
	gameObject_->Draw();
}
