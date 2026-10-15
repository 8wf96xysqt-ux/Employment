#include "DxLib.h"
#include "CollisionSphere.h"
#include "CollisionOBB.h"
#include "../MyMath/MyMath.h"
#include <corecrt_math.h>


// コンストラクタ
CollisionSphere::CollisionSphere()
	: m_TargetPos(nullptr)
	, m_LocalPos(VGet(0.0f, 0.0f, 0.0f))
	, m_Radius(0.0f)
{
}
// デストラクタ
CollisionSphere::~CollisionSphere()
{

}

void CollisionSphere::Draw()
{
#if 1
	// デバッグ用の当たり判定の可視化
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	DrawSphere3D(centerPos, m_Radius, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
#endif
}

bool CollisionSphere::CheckSphere(CollisionSphere* other)
{
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	VECTOR otherCenterPos = MyMath::VecAdd(other->GetTargetPos(), other->GetLocalPos());
	float otherRadius = other->GetRadius();

	// ２つの球の中心間の距離を計算する
	VECTOR vec = MyMath::VecCreate(centerPos, otherCenterPos);
	float distance = MyMath::VecLong(vec);

	// 計算した距離が２つの球の半径の合計以下であれば当たっている
	if (distance <= m_Radius + otherRadius)
	{
		return true;
	}

	return false;
}
bool CollisionSphere::CheckOBB(const CollisionOBB* obb) const
{
	// Sphereの中心座標
	VECTOR sphereCenter =
		MyMath::VecAdd(*m_TargetPos, m_LocalPos);

	// OBBの中心座標
	VECTOR obbCenter = MyMath::VecAdd(obb->GetTargetPos(),obb->GetLocalPos());

	// Sphere中心をOBBのローカル座標へ変換する
	VECTOR relative = MyMath::VecSub(sphereCenter, obbCenter);

	float rotation = obb->GetRotation();

	float s = sinf(rotation);
	float c = cosf(rotation);

	// OBBのローカル座標へ変換
	float localX = relative.x * c + relative.z * s;

	float localZ = -relative.x * s + relative.z * c;

	float localY = relative.y;

	// OBBの半サイズ
	VECTOR size = obb->GetSize();

	float halfX = size.x * 0.5f;
	float halfY = size.y * 0.5f;
	float halfZ = size.z * 0.5f;

	// Sphere中心からOBBまでの最近接点
	float closestX = localX;
	float closestY = localY;
	float closestZ = localZ;

	if (closestX < -halfX)
		closestX = -halfX;
	else if (closestX > halfX)
		closestX = halfX;

	if (closestY < -halfY)
		closestY = -halfY;
	else if (closestY > halfY)
		closestY = halfY;

	if (closestZ < -halfZ)
		closestZ = -halfZ;
	else if (closestZ > halfZ)
		closestZ = halfZ;

	// Sphere中心から最近接点までの距離
	float dx = localX - closestX;
	float dy = localY - closestY;
	float dz = localZ - closestZ;

	float distanceSq =
		dx * dx +
		dy * dy +
		dz * dz;

	// 半径以内なら衝突
	return distanceSq <= m_Radius * m_Radius;
}


