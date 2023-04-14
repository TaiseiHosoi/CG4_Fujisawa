#include "Skydome.h"
#include "Matrix.h"
#include <cassert>

void Skydome::Initialize(Model* model, float r)
{
	assert(model);

	model_ = model;
	r_ = r;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { r_,r_,r_ };
}

void Skydome::Update()
{
	worldTransformUpdate(&worldTransform_);
}

void Skydome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}