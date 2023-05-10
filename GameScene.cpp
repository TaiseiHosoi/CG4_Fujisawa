#include "GameScene.h"

#include<sstream>
#include<iomanip>
#include"imgui.h"




GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);
	dxCommon_ = dxcomon;

	//かめら初期化
	gameCamera_ = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height, input_);
	assert(gameCamera_);

	//カメラのポインタをセット
	//カメラ位置セット
	//gameCamera_->SetTarget(hitokunFbxO_.get()->wtf.translation_);
	gameCamera_->SetEye({ 0 , 5 , -20 });
	gameCamera_->SetTarget({ 0 , 0 , 0 });
	Object3d::SetCamera(gameCamera_.get());
	FBXObject3d::SetCamera(gameCamera_.get());
	ParticleManager::SetCamera(gameCamera_.get());

	collisionManager = CollisionManager::GetInstance();

	//スプライト初期化
	spriteCommon_->LoadTexture(1 , "gamen.png");
	spriteCommon_->LoadTexture(2 , "title.png");
	Sprite::SetCamera(gameCamera_.get());

	sprite1 = std::make_unique<Sprite>();
	sprite1->InitializeTex(spriteCommon_ , 1);
	sprite2 = std::make_unique<Sprite>();
	sprite2->InitializeTex(spriteCommon_ , 2);


	sprite1->SetSize({1280 , 720});
	sprite2->SetSize({1280 , 720});
	sprite1->SetPozition({0 , 0});
	sprite2->SetPozition({0 , 0});
	audio = std::make_unique<Audio>();
	audio->Initialize();


	//gamescene

	//モデル
	model = Mesh::LoadFormOBJ("lowpoliInukun" , true);
	bulletModel_ = Mesh::LoadFormOBJ("bume" , true);
	model_ = Mesh::LoadFormOBJ("cube" , true);
	ico_ = Mesh::LoadFormOBJ("ico" , true);
	land_ = Mesh::LoadFormOBJ("testLand" , true);

	//FBX	(テスト)
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();
	hitokunFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliInukun"));
	hitokunFbxO_ = std::make_unique<FBXObject3d>();
	hitokunFbxO_->Initialize();
	hitokunFbxO_->SetModel(hitokunFbxM_.get());
	hitokunFbxO_->PlayAnimation(0);
	hitokunFbxO_->SetPosition({ 0,0,0 });

	//パーティクルのセット
	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_.get()->Initialize();

	


	////object3d
	//object3d = Object3d::Create();
	////object3d->SetRimEmission();
	//object3d->worldTransform.translation_.z = 0.0f;
	//object3d->SetRimEmission(0.7f);
	//object3d->SetModel(model.get());
	//object3d->Update();
	//
	//gameCamera_->SetTargetPos(&object3d.get()->worldTransform);

	//パーティクル用素材
	particleManager_->LoadTexture("effect.png");
	particleManager_->Update();

	//Player
	fbxPlayer_ = std::make_unique<FbxPlayer>();
	fbxPlayer_.get()->Initialize(hitokunFbxM_.get());
	//gameCamera_.get()->SetEyePos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());
	gameCamera_->SetFollowerPos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());

	//boss
	boss_ = std::make_unique<Boss>();
	boss_.get()->Initialize(dxCommon_);
	boss_.get()->SetPlayer(fbxPlayer_.get());
	gameCamera_.get()->SetTargetPos(boss_.get()->GetObject3d()->GetWorldTransformPtr());
	
	//field
	field_ = std::make_unique<Field>();
	field_.get()->Initialize();
	field_.get()->Update();

	sceneManager_ = std::make_unique<SceneManager>(dxCommon_,gameCamera_.get());
	sceneManager_->ObjectInitialize();
	sceneManager_->SceneInitialize();

}

void GameScene::Update()
{
	gameCamera_->Update();

	collisionManager->CheckAllCollisions();

	sceneManager_->SceneUpdate(input_);

	particleManager_.get()->Update();
  
	//object3d.get()->Update();

	fbxPlayer_.get()->Update();

	boss_.get()->Update();
	////fbxtest
	//if (input_->TriggerKey(DIK_V)) {
	//	hitokunFbxO_.get()->PlayAnimation();
	//}
	//hitokunFbxO_->Update();

	float kCameraVel = 0.02f;
	Vector3 nowEye = gameCamera_->GetEye();

	//フィールドの更新
	field_.get()->Update();

	ImGui::Begin("Camera");

	ImGui::SliderFloat("eye:x" , &nowEye.x , -400.0f , 400.0f);
	ImGui::SliderFloat("eye:xz" , &nowEye.z , -400.0f , 400.0f);

	ImGui::End();

	gameCamera_->SetEye(nowEye);

	//カメラのアップデート
	gameCamera_->Update();

}

void GameScene::Draw()
{
	sceneManager_->SceneDraw();

	spriteCommon_->SpritePreDraw();

	//sprite1->Draw();
	//sprite2->Draw();

	spriteCommon_->SpritePostDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	//object3d->Draw();

		//object3d->Draw();
		fbxPlayer_.get()->Draw(dxCommon_->GetCommandList());
		boss_.get()->Draw();
		field_.get()->Draw();

	Object3d::PostDraw();


}

void GameScene::SetSpriteCommon(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
}

