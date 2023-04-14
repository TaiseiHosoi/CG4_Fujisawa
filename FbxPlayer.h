#pragma once
#include "Object3d.h"
#include "FBXObject3d.h"
#include "FBXModel.h"
#include "input.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class FbxPlayer
{
public:
	//コンストラクタ
	FbxPlayer();

	//デストラクタ
	~FbxPlayer();

	//初期化
	void Initialize(FBXModel* model);

	//更新
	void Update();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	FBXObject3d* GetObject3d();

private:
	//移動
	void Move();

private:

	//入力
	Input* input_ = nullptr;

	//オブジェクト
	std::unique_ptr<FBXObject3d> gameObject_;
	int SPHERE_COLISSION_NUM;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	Vector3 spherePos = {};
	


	//移動速度
	const float kMoveSpeed_ = 0.5;
	//旋回速度
	const float kTurnSpeed_ = MathFunc::Dig2Rad(10);
	//移動ベクトル
	Vector3 velocity_;
	//自機の向き
	Vector3 faceAngle_ = {0 , 0 , 0};
	//カメラの向き
	Vector3 cameraAngle_ = {0 , 0 , 0};
	//動いているかフラグ
	bool isRun = false;
	bool oldIsRun = false;
	//ブレーキアニメーション用フラグ
	bool isbrake = false;
	int brakeFlameCount = 0;
	//アニメーション何番目か
	int animCT = 2;
	int oldAnimCT = 0;
	//攻撃フラグ
	bool isAtk = false;

	//攻撃用フレ―m
	int atkCurrent = 0;

	//防御時行動
	//反撃フラグオン→反撃遷移モーション→反撃→反撃フラグオフ
	bool isCounter = false;	//反撃フラグ
	int atkMovePhase = 0;	//攻撃用フェーズ
	int counterFrameCount = 0;	//全体フレーム
	int nowCTFlame = 0;	//アニメーション用フレームカウント
	float kAccumulateRotVel = 0;


};