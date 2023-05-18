#include "SceneIntegrate.h"



TitleScene::TitleScene(SceneManager* controller) {
	_controller = controller;
}
TitleScene::~TitleScene() {
	
}

void TitleScene::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {
	sprite1 = std::make_unique<Sprite>();
	sprite1->InitializeTex(_controller->spriteCommon_.get(), 1);
	sprite2 = std::make_unique<Sprite>();
	sprite2->InitializeTex(_controller->spriteCommon_.get(), 2);


	sprite1->SetSize({ 1280,720 });
	sprite2->SetSize({ 1280,720 });
	sprite1->SetPozition({ 0,0 });
	sprite2->SetPozition({ 0,0 });
}

void TitleScene::Update(Input* input, GameCamera* camera) {
	
	if (input->TriggerKey(DIK_SPACE)) {
		_controller->ChangeScene(new GamePart1(_controller));
	}
}

void TitleScene::Draw(DirectXCommon* dxCommon) {

	_controller->spriteCommon_->SpritePreDraw();

	sprite1->Draw();
	sprite2->Draw();

	_controller->spriteCommon_->SpritePostDraw();

	Object3d::PreDraw(dxCommon->GetCommandList());

	Object3d::PostDraw();
}
