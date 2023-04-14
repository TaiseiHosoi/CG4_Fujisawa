#include "SceneIntegrate.h"


GamePart1::GamePart1(SceneManager* controller) {
	_controller = controller;
}

GamePart1::~GamePart1() {

}

void GamePart1::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {


}

void GamePart1::Update(Input* input, GameCamera* camera) {

	if (input->TriggerKey(DIK_SPACE)) {
		_controller->ChangeScene(new GamePart2(_controller));
	}

}

void GamePart1::Draw(DirectXCommon* dxCommon) {

}