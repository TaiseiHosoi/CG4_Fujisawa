#include "Ground.h"

Field::Field()
{
}

Field::~Field()
{
	
}

void Field::Initialize()
{
	fieldM = std::make_unique<Mesh>();
	fieldM = Mesh::LoadFormOBJ("fieldkari",false);

	gameObject_ = std::make_unique<Object3d>();
	gameObject_.get()->Initialize(true);
	gameObject_.get()->SetModel(fieldM.get());
	gameObject_->worldTransform.scale_ = { 20,20,20 };
	gameObject_->worldTransform.translation_ = { 0,-2,0 };
	gameObject_->Update();




}

void Field::Update()
{
	gameObject_->Update();
}

void Field::Draw()
{

	gameObject_->Draw();
	
}
