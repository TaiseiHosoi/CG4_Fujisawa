#include "SceneIntegrate.h"


GamePart2::GamePart2(SceneManager* controller) {
	_controller = controller;
}

GamePart2::~GamePart2() {

}

void GamePart2::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {


}

void GamePart2::Update(Input* input, GameCamera* camera) {
	if (input->TriggerKey(DIK_SPACE)) {
		_controller->ChangeScene(new EndScene(_controller));
	}

}

void GamePart2::Draw(DirectXCommon* dxCommon) {


}