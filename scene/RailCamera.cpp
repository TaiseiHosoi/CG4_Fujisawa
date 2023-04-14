#include "RailCamera.h"
#include "math.h"
#include <cassert>
#define PI 3.141592653589

void RailCamera::Initialize()
{
	//引数から座標と回転角を受け取る
	/*worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;*/
	/*worldTransform_.translation_.x = worldTransform.translation_.x;
	worldTransform_.translation_.y = worldTransform.translation_.y;
	worldTransform_.translation_.z = worldTransform.translation_.z;
	worldTransform_.rotation_ = worldTransform.rotation_;*/
	//ビュープロジェクションの初期化
	
	viewProjection_.eye = { 0,7,-50 };
	viewProjection_.target = { 0,0,-32 };
	viewProjection_.Initialize();

}

void RailCamera::Update(WorldTransform worldTransform)
{
	//////引数から座標と回転角を受け取る
	////worldTransform_.scale_ = {1,1,1};
	//////ワールドトランスフォームの更新
	////worldTransformUpdate(&worldTransform_);

	////オブジェクトに追従する座標の処理
	////ワールド座標を視点座標に代入
	///*viewProjection_.eye = worldTransform_.translation_;*/
	////ワールド前方ベクトル
	//Vector3 forward(0, 0, 1);
	////レールカメラの反転を影響
	///*forward = vector3Normalize(worldTransform_.translation_);*/
	////視点から前方に適当な距離進んだ位置が注視点
	//viewProjection_.target = { viewProjection_.eye.x + forward.x ,viewProjection_.eye.y + forward.y ,viewProjection_.eye.z + forward.z };
	////ワールド上方ベクトル
	//Vector3 up(0, 1, 0);
	////レールカメラの回転を反映
	///*up = vector3Normalize(worldTransform_.rotation_);*/
	////ビュープロジェクションを更新
	//viewProjection_.Initialize();

	/*float x = worldTransform.translation_.x - (sin(worldTransform.rotation_.y) * length);
	float y = worldTransform.translation_.y - (worldTransform.rotation_.x * length);
	float z = worldTransform.translation_.z - (cos(worldTransform.rotation_.y) * length);*/
	/*float x = worldTransform.translation_.x - (sin(worldTransform.rotation_.y) * length);
	float y = worldTransform.translation_.y - (worldTransform.rotation_.x * length);
	float z = worldTransform.translation_.z + (cos(worldTransform.rotation_.x) * length);*/

	/*float x = worldTransform.translation_.x - (sin(worldTransform.rotation_.y) * length * 2);
	float y = worldTransform.translation_.y + (sin(worldTransform.rotation_.x) * length * 2);
	float z = worldTransform.translation_.z - (cos(worldTransform.rotation_.y) * length) - (cos(worldTransform.rotation_.x) * length );*/

	/*float x, y, z;

	x = worldTransform.translation_.x + (sin(-worldTransform.rotation_.x + (PI / 2)) * cos(-worldTransform.rotation_.y - (PI / 2)) * length);
	y = worldTransform.translation_.y + (cos(-worldTransform.rotation_.x + (PI / 2)) * length);
	z = worldTransform.translation_.z + (sin(-worldTransform.rotation_.x + (PI / 2)) * sin(-worldTransform.rotation_.y - (PI / 2)) * length);

	if (sin(-worldTransform.rotation_.x + (PI / 2)) <= 0)
	{
		viewProjection_.up = { 0,-1,0 };
	}
	else
	{
		viewProjection_.up = { 0,1,0 };
	}
	viewProjection_.eye = { x,y,z };
	viewProjection_.target = { worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z };
	viewProjection_.Initialize();*/
}
