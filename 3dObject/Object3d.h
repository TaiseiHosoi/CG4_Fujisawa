#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Mesh.h"
#include"Vector3.h"
#include"Matrix4.h"
#include"Camera.h"
#include"WorldTransform.h"
#include "imgui.h"

//class BaseCollider;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
public:
	Object3d() = default;
	virtual ~Object3d();

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		Matrix4 mat;	// ３Ｄ変換行列
	};


	//マテリアル
	struct Material
	{
		std::string name;	//マテリアル名
		XMFLOAT3 ambient;	//アンビエント影響度
		XMFLOAT3 diffuse;	//ディフューズ影響度
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material()
		{
			ambient = {0.3f , 0.3f , 0.3f};
			diffuse = {0.0f , 0.0f , 0.0f};
			specular = {0.0f , 0.0f , 0.0f};
			alpha = 1.0f;
		}
	};

	struct Rim
	{
		XMFLOAT4 color;
		float pow;
		float Emission;

		Rim()
		{
			pow = 3;
			color = {1.0f , 1.0f , 1.0f , 1.0f};
			Emission = 1.0f;
		}
	};


private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = planeCount * 3;		// 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device , int window_width , int window_height);

	/// <summary>
	/// 通常描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Object3d> Create();






private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
#pragma region 通常モデル（ライト無し）
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

#pragma endregion
#pragma region リムライト
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rimRootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> rimPipelinestate;
#pragma endregion
	//// ビュー行列
	static Matrix4 matView;
	//// 射影行列
	static Matrix4 matProjection;
	//// 視点座標
	//static XMFLOAT3 eye;
	//// 注視点座標
	//static XMFLOAT3 target;
	//// 上方向ベクトル
	//static XMFLOAT3 up;

private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>


	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width , int window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数
	virtual bool Initialize(bool Isrim);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

public:
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const { return worldTransform.translation_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position) { this->worldTransform.translation_ = position; }

	/// <summary>
	/// 倍率の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetScale() const { return worldTransform.scale_; }

	/// <summary>
	/// 倍率の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetScale(const Vector3& scale) { this->worldTransform.scale_ = scale; }

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4& GetMatWorld() { return this->worldTransform.matWorld_; };

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetRimColor(const XMFLOAT4& rimColor) { this->rim.color = rimColor; }

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera);
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetRimPow(const float& rimPow) { this->rim.pow = rimPow; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetRimEmission(const float& rimEmission) { this->rim.Emission = rimEmission; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	//void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	//virtual void OnCollision(const CollisionInfo& info) {};

	//アクセッサ
	void SetModel(Mesh* model) { this->model = model; }

	inline Mesh* GetModel() { return model; };

#pragma region 共通の定数バッファ
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
#pragma endregion
#pragma region リムライトの定数バッファ
	ComPtr<ID3D12Resource> constBuffCameraPosition; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffRim; // 定数バッファ
#pragma endregion

public: // メンバ変数
	//ワールド行列
	WorldTransform worldTransform;

	Rim rim;
	// 色
	XMFLOAT4 color = {1 , 1 , 1 , 1};
	//// ローカルスケール
	//Vector3 scale = { 1,1,1 };
	//// X,Y,Z軸回りのローカル回転角
	//Vector3 rotation = { 0,0,0 };
	//// ローカル座標
	//Vector3 position = { 0,0,0 };
	//// ローカルワールド変換行列
	//Matrix4 matWorld;
	//// 親オブジェクト
	//Object3d* parent = nullptr;

	//モデル
	Mesh* model = nullptr;

	//クラス名(デバッグ用)
	const char* name = nullptr;
	////コライダー
	//BaseCollider* collider = nullptr;

	bool isRim = false;

	//Camera
	static Camera* camera_;
};