#pragma once

#include "DirectXCommon.h"

#include "Vector3.h"
#include "input.h"
#include "Object3d.h"

#include "BossBullet.h"

#include <list>
#include <memory>

#include"FBXObject3d.h"
#include"fbx/FBXLoader.h"
#include"FBXModel.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

//前方宣言
class FbxPlayer;

enum BattleMode
{
	Run,
	Ground,
};

enum AttackValue
{
	nasi,
	FIRE,
	TACKLE,
};

class Boss
{
public:
	std::unique_ptr<Object3d> bossObj_;
#pragma region 攻撃関連


	Vector3 distance = { 0,0,0 };

	Vector3 normDis = { 0,0,0 };


	int isTackle = false;

	int tackleCount = 0;

	Vector3 playerPos;

	Vector3 tackleDis = { 0,0,0 };

	Vector3 normTdis = { 0,0,0 };

	Vector3 bulletDistance = { 0,0,0 };

	//タックル用の壁
	Vector3 wallPos;

	float moveTimer = 60.0f;

	float stopTimer = 60.0f;

#pragma endregion



	float bulletTimer = 60.0f;

	int bulletNum = 0;

	int isFire = false;

	int bulletCount = 0;

#pragma region

	float attackTimer = 180.0f;

	float coolTimer = 90.0f;

	int attackValue = AttackValue::nasi;

	bool isAttack = false;

#pragma endregion

public:

	void Initialize(DirectXCommon* dxcomon);

	void Update();

	void Draw();

	void RunAway();

	void Move();

	void Shot();

	void Tackle();

	void Reset();

	Vector3 GetPosition();

	static Boss* GetInstance();

	void SetPlayer(FbxPlayer* player) { player_ = player; }

	FBXObject3d* GetObject3d();
private:

	Input* input_ = nullptr;

	std::unique_ptr<Mesh> bossModel_;

	FbxPlayer* player_ = nullptr;

	float angle = 0.0f;

	float radian = 0.0f;

	int mode = BattleMode::Run;


private:

	DirectXCommon* dxcomon_ = nullptr;

	std::unique_ptr<FBXModel> bossFbxM_;
	std::unique_ptr<FBXObject3d> bossFbxO_;

	std::list<std::unique_ptr<BossBullet>> bullets_;
	
	//コライダー
	int SPHERE_COLISSION_NUM;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos;

	//testobj
	std::unique_ptr <Mesh> ico_;
	std::vector<std::unique_ptr<Object3d>> testObj_;


};