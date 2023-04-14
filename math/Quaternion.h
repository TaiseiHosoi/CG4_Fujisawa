#pragma once
#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	// 単位クォータニオン生成
	Quaternion()
		: x(0), y(0), z(0), w(1) {}

	// 指定した値で生成
	Quaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}

public:
	// Quaternionの積
	Quaternion Multiply(const Quaternion& r);

	// 単位Quaternionを返す
	Quaternion IdentityQuaternion();

	// 共役Quaternionを返す
	static Quaternion Conjugate(const Quaternion& quaternion);

	// Quaternionのnormを返す
	static float Norm(const Quaternion& quaternion);

	// 正規化したQuaternionを返す
	Quaternion Normalize(const Quaternion& quaternion);

	// 逆Quaternionを返す
	static Quaternion Inverse(const Quaternion& quaternion);

	// Quaternion -> Mat4に変換するもの
	Matrix4 Quaternion2Matrix() const;

	// 任意軸回転を表すQuaternionの生成
	Quaternion MakeAxisAngle(const Vector3& axis, float angle);

	// ベクトルをQuaternionで回転させた結果のベクトルを求める
	Vector3 RotateVector(const Vector3& vector);

	// Quaternionから回転行列を求める
	Matrix4 MakeRotateMatrix(const Quaternion& quaternion);

	// 球面線形補間
	Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	// 内積
	float Dot(const Quaternion& q0, const Quaternion& q1);

	// uからvへの回転を生成
	Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);


public:
	Quaternion operator/(const float& s) const;
	Quaternion operator*(const float& s) const;
	Quaternion& operator/=(const float& s);
	Quaternion& operator*=(const float& s);
};
