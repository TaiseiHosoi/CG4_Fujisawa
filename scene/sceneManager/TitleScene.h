#pragma once
#include "IScene.h"

class TitleScene :public IScene
{
protected:
	SceneManager* _controller;

public:
	
	TitleScene(SceneManager* controller);
	~TitleScene() override;

	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	void Update(Input* input, GameCamera* camera) override;
	void Draw(DirectXCommon* dxCommon) override;
public:

private:
	
	Input* input_ = Input::GetInstance();

	std::unique_ptr <SceneManager> sceneManager_;

private://リソース
	std::unique_ptr <Sprite> sprite1;
	std::unique_ptr <Sprite> sprite2;
	std::unique_ptr <Sprite> sprite3;

	std::unique_ptr <Mesh> bulletModel_;
private:


	//3Dモデル
	std::unique_ptr <Mesh> model_;
	std::unique_ptr <Mesh> ico_;

	////FBXモデル
	//std::unique_ptr<FBXModel> hitokunFbxM_;
	//std::unique_ptr<FBXObject3d>hitokunFbxO_;


	//レーンデバッグ用のテクスチャ
	//uint32_t laneTexture_[3];

	//テスト3-2
	//当たり判定　球
	Sphere sphere;
	//当たり判定　平面
	Plane plane;

	int collideCount;



	//待機中フラグ
	bool isStand_ = false;
	//待機タイマー
	int standTime_ = 0;

	int gameTimer_ = 0;

};