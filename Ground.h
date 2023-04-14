#pragma once
#include "DirectXCommon.h"

#include "Vector3.h"
#include "input.h"
#include "Object3d.h"
#include"Mesh.h"
class Field
{
	//コンストラクタ
public:
	Field();
	~Field();

public:
	void Initialize();

	void Update();

	void Draw();


	//メンバ変数
private:
	std::unique_ptr<Object3d> gameObject_;	//地面
	std::unique_ptr<Mesh> fieldM;
	
	const int fenceLen_ = 4;
	Object3d* fence_[4] = {};	//フェンス
	Mesh* fenceM = nullptr;


};
