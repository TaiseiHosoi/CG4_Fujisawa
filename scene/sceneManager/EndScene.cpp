#include "EndScene.h"
#include "SceneManager.h"

#include "TitleScene.h"

EndScene::EndScene(SceneManager* controller) {
	_controller = controller;
}
EndScene::~EndScene() {

}

void EndScene::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {

}

void EndScene::Update(Input* input, GameCamera* camera) {
	if (input->TriggerKey(DIK_SPACE)) {
		_controller->ChangeScene(new TitleScene(_controller));
	}

}

void EndScene::Draw(DirectXCommon* dxCommon) {


}