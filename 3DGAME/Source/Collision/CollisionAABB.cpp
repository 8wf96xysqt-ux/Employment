#include "DxLib.h"
#include "CollisionAABB.h"
#include "../MyMath/MyMath.h"
#include <type_traits>
#include <corecrt_math.h>
// コンストラクタ
CollisionAABB::CollisionAABB()
{
	m_TargetPos = nullptr;
	m_LocalPos = VGet(0.0f, 0.0f, 0.0f);
	m_Size = VGet(0.0f, 0.0f, 0.0f);
}
// デストラクタ
CollisionAABB::~CollisionAABB()
{
}
void CollisionAABB::Draw()
{
#ifdef _DEBUG
	// デバッグ用の当たり判定の可視化
	VECTOR posA, posB;
	// 対象座標 + ローカル座標でボックスの中心座標を求める
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	// ボックスの最小座標(左下手前)を求める
	posA.x = centerPos.x - m_Size.x * 0.5f;
	posA.y = centerPos.y - m_Size.y * 0.5f;
	posA.z = centerPos.z - m_Size.z * 0.5f;
	// ボックスの最大座標(右上奥)を求める
	posB.x = centerPos.x + m_Size.x * 0.5f;
	posB.y = centerPos.y + m_Size.y * 0.5f;
	posB.z = centerPos.z + m_Size.z * 0.5f;
	// ワイヤーフレームでボックスを描画する
	DrawCube3D(posA, posB, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
#endif
}
bool CollisionAABB::CheckAABB(const CollisionAABB* other) const
{
	// 自身のボックスの中心座標(対象座標+ローカル座標)を求める
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	// 対象のボックスの中心座標を求める
	VECTOR otherCenterPos = MyMath::VecAdd(other->GetTargetPos(), other->GetLocalPos());
	// 対象のボックスのサイズを取得する
	VECTOR otherSize = other->GetSize();
	// 自身のボックスの各面の値
	float left = centerPos.x - m_Size.x * 0.5f;	// 左
	float right = centerPos.x + m_Size.x * 0.5f;	// 右
	float top = centerPos.y + m_Size.y * 0.5f;	// 上
	float bottom = centerPos.y - m_Size.y * 0.5f;	// 下
	float front = centerPos.z - m_Size.z * 0.5f;	// 手前
	float back = centerPos.z + m_Size.z * 0.5f;	// 奥
	// 対象のボックスの各面の値
	float otherLeft = otherCenterPos.x - otherSize.x * 0.5f;	// 左
	float otherRight = otherCenterPos.x + otherSize.x * 0.5f;	// 右
	float otherTop = otherCenterPos.y + otherSize.y * 0.5f;	// 上
	float otherBottom = otherCenterPos.y - otherSize.y * 0.5f;	// 下
	float otherFront = otherCenterPos.z - otherSize.z * 0.5f;	// 手前
	float otherBack = otherCenterPos.z + otherSize.z * 0.5f;	// 奥
	// 各面が交差していれば当たっている
	if (left < otherRight && right > otherLeft &&
		bottom < otherTop && top > otherBottom &&
		front < otherBack && back > otherFront)
	{
		// 全軸で重なっていたので当たっている
		return true;
	}
	// いずれかの軸で重なっていないので当たっていない
	return false;
}
// 当たったかどうかだけを返す
bool CollisionAABB::CheckRayHit(VECTOR rayStart, VECTOR rayEnd) const
{
	// t値版を呼び出し、t値は使わずに結果のbool値だけ返す
	float dummyT = 0.0f;
	return CheckRayHit(rayStart, rayEnd, &dummyT);
}
// 当たったかどうかと、当たった位置をt値(0.0=rayStart 〜 1.0=rayEnd)で返す
bool CollisionAABB::CheckRayHit(VECTOR rayStart, VECTOR rayEnd, float* outT) const
{
	// ボックスの中心座標を求める
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	// ボックスの各面の座標を求める
	float left = centerPos.x - m_Size.x * 0.5f;
	float right = centerPos.x + m_Size.x * 0.5f;
	float bottom = centerPos.y - m_Size.y * 0.5f;
	float top = centerPos.y + m_Size.y * 0.5f;
	float front = centerPos.z - m_Size.z * 0.5f;
	float back = centerPos.z + m_Size.z * 0.5f;
	// レイの方向(終点-始点)を求める
	VECTOR dir = MyMath::VecSub(rayEnd, rayStart);
	// レイ全体のうち、ボックスと重なっている区間の下限・上限(0.0〜1.0)
	float tMin = 0.0f;
	float tMax = 1.0f;
	// X軸
	if (fabs(dir.x) < 0.0001f)
	{
		// レイがX方向に進んでいない場合、開始点がX範囲内かどうかだけ調べる
		if (rayStart.x < left || rayStart.x > right) return false;
	}
	else
	{
		// レイが左面・右面と交差するt値を求める
		float t1 = (left - rayStart.x) / dir.x;
		float t2 = (right - rayStart.x) / dir.x;
		// t1を小さい方、t2を大きい方に並び替える
		if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
		// 区間を絞り込む
		tMin = max(tMin, t1);
		tMax = min(tMax, t2);
		// 区間が無くなったら当たっていない
		if (tMin > tMax) return false;
	}
	// Y軸
	if (fabs(dir.y) < 0.0001f)
	{
		// レイがY方向に進んでいない場合、開始点がY範囲内かどうかだけ調べる
		if (rayStart.y < bottom || rayStart.y > top) return false;
	}
	else
	{
		// レイが下面・上面と交差するt値を求める
		float t1 = (bottom - rayStart.y) / dir.y;
		float t2 = (top - rayStart.y) / dir.y;
		// t1を小さい方、t2を大きい方に並び替える
		if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
		// 区間を絞り込む
		tMin = max(tMin, t1);
		tMax = min(tMax, t2);
		// 区間が無くなったら当たっていない
		if (tMin > tMax) return false;
	}
	// Z軸
	if (fabs(dir.z) < 0.0001f)
	{
		// レイがZ方向に進んでいない場合、開始点がZ範囲内かどうかだけ調べる
		if (rayStart.z < front || rayStart.z > back) return false;
	}
	else
	{
		// レイが手前面・奥面と交差するt値を求める
		float t1 = (front - rayStart.z) / dir.z;
		float t2 = (back - rayStart.z) / dir.z;
		// t1を小さい方、t2を大きい方に並び替える
		if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
		// 区間を絞り込む
		tMin = max(tMin, t1);
		tMax = min(tMax, t2);
		// 区間が無くなったら当たっていない
		if (tMin > tMax) return false;
	}
	// ヒットした位置のt値をoutTに書き込む
	if (outT != nullptr)
	{
		*outT = tMin;
	}
	return true;
}