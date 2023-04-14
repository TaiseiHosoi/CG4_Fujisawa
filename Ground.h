#pragma once
#include "DirectXCommon.h"

#include "Vector3.h"
#include "input.h"
#include "Object3d.h"
#include"Mesh.h"
class Field
{
	//�R���X�g���N�^
public:
	Field();
	~Field();

public:
	void Initialize();

	void Update();

	void Draw();


	//�����o�ϐ�
private:
	std::unique_ptr<Object3d> gameObject_;	//�n��
	std::unique_ptr<Mesh> fieldM;
	
	const int fenceLen_ = 4;
	Object3d* fence_[4] = {};	//�t�F���X
	Mesh* fenceM = nullptr;


};
