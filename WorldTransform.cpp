#include "WorldTransform.h"

WorldTransform::WorldTransform(Vector3 scale, Vector3 rotation,Vector3 translation)
{
	this->scale_ = scale;
	this->rotation_ = rotation;
	this->translation_ = translation;
	
}

void WorldTransform::initialize() {

	scale_ = {1 , 1 , 1};
	rotation_ = {0 , 0 , 0};
	translation_ = {0 , 0 , 0};
	matWorld_.identity();

	UpdateMatWorld();

}

//ワールド変換行列の更新
void WorldTransform::UpdateMatWorld() {

	//ワールド変換行列を用意
	Matrix4 affineMat, matScale, matRot, matTrans;

	affineMat.identity();
	//matScale.identity();
	matRot.identity();
	//matTrans.identity();

	//ワールド変換行列にスケーリング,回転,平行移動の行列を合成
	matScale = MathFunc::Scale(scale_);
	matRot.rotateZ(rotation_.z);
	matRot.rotateX(rotation_.x);
	matRot.rotateY(rotation_.y);
	matTrans = MathFunc::Move(translation_);
	//ワールド行列に単位行列を代入
	matWorld_.identity();

	//行列の計算
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;


	//もし親がある場合
	if (parent_) {
		//親のワールド行列との計算を行う
		matWorld_ *= parent_->matWorld_;
	}

}