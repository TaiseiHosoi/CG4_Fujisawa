#include"WinApp.h"
#include"DirectXCommon.h"
#include"Error.h"
#include"Mesh.h"
#include"input.h"
#include"Object3d.h"
#include"Sprite.h"
#include"Audio.h"
#include"GameScene.h"
#include"ImGuiManager.h"
#include"fbxsdk.h"



const float PI = 3.14f;

//using namespace Microsoft::WRL;
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//@brief コンソール画面にフォーマット付き文字列の表示
//@param format フォーマット(%dとか%fとかの)
//@param 可変長引数
//@remarks この関数はデバック用です。デバッグ時にしか動作しません
//void DebugOutputFormatString(const char* format, ...) {
//#ifdef _DEBUG
//	va_list valist;
//	va_start(valist, format);
//	vprintf(format, valist);
//	va_end(valist);
//#endif
//}



//Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//コンソールへの文字出力
{


	//OutputDebugStringA("Hello,DrectX!!/n");

	//ポインタ宣言
	WinApp* winApp_ = nullptr;
	winApp_ = new WinApp;


	DirectXCommon* dxCommon_ = nullptr;
	dxCommon_ = new DirectXCommon;

	//winApp初期化
	winApp_->Initialize();

	MSG msg = {};
	//DirectX初期化処理　ここから
	dxCommon_->Initialize(winApp_);

	Input* input_ = Input::GetInstance();
	input_->Initialize(winApp_);

	// FBX関連静的初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);


	GameScene* gameScene_ = nullptr;
	gameScene_ = new GameScene();
	gameScene_->Initialize(dxCommon_);

	ImGuiManager* imGuiManager_ = nullptr;
	imGuiManager_ = new ImGuiManager;
	imGuiManager_->Initialize(winApp_,dxCommon_);

	


	while (true) {
		if (winApp_->ProcessMessage()) {
			break;
		}

		//更新
		input_->Update();
		imGuiManager_->Begin();
		gameScene_->Update();


		imGuiManager_->End();

		//描画
		dxCommon_->PreDraw();


		gameScene_->Draw();

		imGuiManager_->Draw();

		dxCommon_->PostDraw();

	}

	winApp_->Finalize();
	imGuiManager_->Finalize();

	ID3D12DebugDevice* debugInterface;

	if (SUCCEEDED(dxCommon_->GetDevice()->QueryInterface(&debugInterface))) {
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}
	//元データ解放
	//delete[] imageData;
	//入力開放

	delete gameScene_;
	delete imGuiManager_;
	delete dxCommon_;
	delete winApp_;
	

	return 0;
}