#include "DxLib.h"
#include "CollisionSlope.h"
#include "../MyMath/MyMath.h"
#include <corecrt_math.h>

// コンストラクタ
CollisionSlope::CollisionSlope()
{
	m_TargetPos = nullptr;
	m_LocalPos = VGet(0.0f, 0.0f, 0.0f);
	m_Size = VGet(0.0f, 0.0f, 0.0f);
	m_TiltAxis = SlopeTiltAxis::Z;
	m_IsAscending = true;
	m_MaxWalkableAngleDeg = 45.0f; // デフォルトは45度まで歩行可能
}
// デストラクタ
CollisionSlope::~CollisionSlope()
{
}
void CollisionSlope::Draw()
{
#ifdef _DEBUG
	VECTOR center = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	float minX = center.x - m_Size.x * 0.5f;
	float maxX = center.x + m_Size.x * 0.5f;
	float minZ = center.z - m_Size.z * 0.5f;
	float maxZ = center.z + m_Size.z * 0.5f;

	// 斜面の4隅の高さを求めて、線で結んで可視化する
	VECTOR p00 = VGet(minX, GetHeightAt(minX, minZ), minZ);
	VECTOR p10 = VGet(maxX, GetHeightAt(maxX, minZ), minZ);
	VECTOR p01 = VGet(minX, GetHeightAt(minX, maxZ), maxZ);
	VECTOR p11 = VGet(maxX, GetHeightAt(maxX, maxZ), maxZ);

	unsigned int color = GetColor(0, 255, 255);
	DrawLine3D(p00, p10, color);
	DrawLine3D(p10, p11, color);
	DrawLine3D(p11, p01, color);
	DrawLine3D(p01, p00, color);
	DrawLine3D(p00, p11, color); // 対角線を引いて面であることを分かりやすくする
#endif
}
float CollisionSlope::GetHeightAt(float worldX, float worldZ) const
{
	VECTOR center = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	float minY = center.y - m_Size.y * 0.5f;
	float maxY = center.y + m_Size.y * 0.5f;

	// 0.0=低い端 〜 1.0=高い端 になる補間係数tを求める
	float t = 0.0f;
	if (m_TiltAxis == SlopeTiltAxis::X)
	{
		float minX = center.x - m_Size.x * 0.5f;
		float maxX = center.x + m_Size.x * 0.5f;
		t = (maxX > minX) ? (worldX - minX) / (maxX - minX) : 0.0f;
	}
	else
	{
		float minZ = center.z - m_Size.z * 0.5f;
		float maxZ = center.z + m_Size.z * 0.5f;
		t = (maxZ > minZ) ? (worldZ - minZ) / (maxZ - minZ) : 0.0f;
	}
	// 範囲外は端の高さにクランプする
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;
	// 上り方向が軸の-方向なら、tを反転する
	if (!m_IsAscending) t = 1.0f - t;

	return minY + (maxY - minY) * t;
}
bool CollisionSlope::IsInsideXZ(float worldX, float worldZ) const
{
	VECTOR center = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	float minX = center.x - m_Size.x * 0.5f;
	float maxX = center.x + m_Size.x * 0.5f;
	float minZ = center.z - m_Size.z * 0.5f;
	float maxZ = center.z + m_Size.z * 0.5f;
	return (worldX >= minX && worldX <= maxX && worldZ >= minZ && worldZ <= maxZ);
}
VECTOR CollisionSlope::GetNormal() const
{
	VECTOR center = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	float minX = center.x - m_Size.x * 0.5f;
	float maxX = center.x + m_Size.x * 0.5f;
	float minZ = center.z - m_Size.z * 0.5f;
	float maxZ = center.z + m_Size.z * 0.5f;

	// 斜面上の3点から法線を求める
	// (軸やAscendingの正負ごとに式を変える必要がなく、間違いにくい)
	VECTOR p0 = VGet(minX, GetHeightAt(minX, minZ), minZ);
	VECTOR p1 = VGet(maxX, GetHeightAt(maxX, minZ), minZ);
	VECTOR p2 = VGet(minX, GetHeightAt(minX, maxZ), maxZ);

	VECTOR edge1 = MyMath::VecSub(p1, p0);
	VECTOR edge2 = MyMath::VecSub(p2, p0);
	VECTOR normal = VCross(edge1, edge2);
	normal = VNorm(normal);

	// 上向き(Y成分が正)になるよう、必要なら反転する
	if (normal.y < 0.0f)
	{
		normal = VScale(normal, -1.0f);
	}
	return normal;
}
float CollisionSlope::GetSlopeAngleDeg() const
{
	VECTOR normal = GetNormal();
	// 法線と真上ベクトルの内積から、傾斜角を求める(normalは正規化済み)
	float dot = normal.x * 0.0f + normal.y * 1.0f + normal.z * 0.0f;
	// 浮動小数の誤差でacosfの定義域(-1〜1)を超えないようクランプする
	if (dot > 1.0f) dot = 1.0f;
	if (dot < -1.0f) dot = -1.0f;
	float radian = acosf(dot);
	return radian * (180.0f / 3.14159265f);
}
VECTOR CollisionSlope::GetDownhillDirectionXZ() const
{
	// 法線の水平成分(XZ)が、坂の下り方向そのものになる
	// (法線は坂の表面から垂直に立っているので、その水平投影は登り方向の逆=下り方向を指す)
	VECTOR normal = GetNormal();
	VECTOR downhill = VGet(normal.x, 0.0f, normal.z);

	float lengthSq = downhill.x * downhill.x + downhill.z * downhill.z;
	// 水平な床(垂直な法線)の場合、下り方向は定義できないのでゼロベクトルを返す
	if (lengthSq < 0.0001f)
	{
		return VGet(0.0f, 0.0f, 0.0f);
	}
	return VNorm(downhill);
}