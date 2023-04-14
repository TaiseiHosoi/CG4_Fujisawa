#pragma once
#include "WorldTransform.h"
#include "Matrix.h"
#include "ViewProjection.h"

class RailCamera
{
public:
	void Initialize();
	void Update(WorldTransform worldTransform);
	//ゲッター
	ViewProjection GetViewProjection() { return viewProjection_; }
private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//自機とカメラの距離
	float length = 20.0f;
};

