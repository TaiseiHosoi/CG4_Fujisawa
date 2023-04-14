#pragma once
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "Object3d.h"

class TestLand
{
public:
	TestLand();

	~TestLand();

	void Initialize(Mesh* model);
	
	void Update();

	void Draw();

	void OnCollision(const CollisionInfo& info)
	{

	}

	BaseCollider* collider_ = nullptr;

	std::unique_ptr<Object3d> gameObject_;

};

