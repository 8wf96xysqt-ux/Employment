#include "DxLib.h"
#include "CollisionAABB.h"
#include "CollisionSphere.h"
#include "CollisionOBB.h"
#include "../MyMath/MyMath.h"
#include <type_traits>
#include <corecrt_math.h>
// コンストラクタ
CollisionAABB::CollisionAABB()
	:m_TargetPos(nullptr)
	,m_LocalPos(VGet(0.0f, 0.0f, 0.0f))
	,m_Size(VGet(0.0f, 0.0f, 0.0f))
{
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
bool CollisionAABB::CheckSphere(const CollisionSphere* sphere) const
{
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);

	VECTOR sphereCenter =
		MyMath::VecAdd(sphere->GetTargetPos(), sphere->GetLocalPos());

	float radius = sphere->GetRadius();

	// AABBの最小・最大座標
	float minX = centerPos.x - m_Size.x * 0.5f;
	float maxX = centerPos.x + m_Size.x * 0.5f;

	float minY = centerPos.y - m_Size.y * 0.5f;
	float maxY = centerPos.y + m_Size.y * 0.5f;

	float minZ = centerPos.z - m_Size.z * 0.5f;
	float maxZ = centerPos.z + m_Size.z * 0.5f;

	// Sphereの中心からAABB上で一番近い点を求める
	float closestX = sphereCenter.x;
	float closestY = sphereCenter.y;
	float closestZ = sphereCenter.z;

	if (closestX < minX)
		closestX = minX;
	else if (closestX > maxX)
		closestX = maxX;

	if (closestY < minY)
		closestY = minY;
	else if (closestY > maxY)
		closestY = maxY;

	if (closestZ < minZ)
		closestZ = minZ;
	else if (closestZ > maxZ)
		closestZ = maxZ;

	// Sphere中心から最近接点までの距離の2乗
	float dx = sphereCenter.x - closestX;
	float dy = sphereCenter.y - closestY;
	float dz = sphereCenter.z - closestZ;

	float distanceSq = dx * dx + dy * dy + dz * dz;

	// 半径以内なら衝突
	return distanceSq <= radius * radius;
}

bool CollisionAABB::CheckOBB(const CollisionOBB* obb) const
{
	// AABBの中心座標
	VECTOR posA =
		MyMath::VecAdd(*m_TargetPos, m_LocalPos);

	// OBBの中心座標
	VECTOR posB =MyMath::VecAdd(obb->GetTargetPos(),obb->GetLocalPos());

	// AABBの軸
	VECTOR axisA[2];

	// X軸
	axisA[0] = VGet(1.0f, 0.0f, 0.0f);

	// Z軸
	axisA[1] = VGet(0.0f, 0.0f, 1.0f);


	// OBBの軸
	VECTOR axisB[2];

	float s = sinf(obb->GetRotation());
	float c = cosf(obb->GetRotation());

	// OBBのX軸
	axisB[0] = VGet(c, 0.0f, s);

	// OBBのZ軸
	axisB[1] = VGet(-s, 0.0f, c);


	// 中心間の距離
	VECTOR distance =
		MyMath::VecSub(posB, posA);


	// 分離軸
	// AABB X
	// AABB Z
	// OBB X
	// OBB Z
	VECTOR axes[4];

	axes[0] = axisA[0];
	axes[1] = axisA[1];
	axes[2] = axisB[0];
	axes[3] = axisB[1];


	for (int i = 0; i < 4; i++)
	{
		VECTOR axis = axes[i];

		// 中心間距離を軸に投影
		float distanceProjection =fabsf(distance.x * axis.x +distance.z * axis.z);


		// AABBの半サイズ
		float halfAX = m_Size.x * 0.5f;
		float halfAZ = m_Size.z * 0.5f;


		// AABBを軸に投影した幅
		float a = halfAX * fabsf(axis.x) + halfAZ * fabsf(axis.z);


		// OBBの半サイズ
		VECTOR obbSize = obb->GetSize();

		float halfBX = obbSize.x * 0.5f;
		float halfBZ = obbSize.z * 0.5f;


		// OBBを軸に投影した幅
		float b = 
			halfBX * fabsf(axisB[0].x * axis.x +axisB[0].z * axis.z)
			+
			halfBZ *fabsf(axisB[1].x * axis.x +axisB[1].z * axis.z);


		// この軸上で離れていたら衝突していない
		if (distanceProjection > a + b)
		{
			return false;
		}
	}


	// Y方向の判定
	float topA = posA.y + m_Size.y * 0.5f;

	float bottomA = posA.y - m_Size.y * 0.5f;


	float topB = posB.y + obb->GetSize().y * 0.5f;

	float bottomB = posB.y - obb->GetSize().y * 0.5f;


	// Y方向で離れている
	if (bottomA > topB || topA < bottomB)
	{
		return false;
	}


	// すべての軸で分離していない
	return true;
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