#pragma once
#include "DxLib.h"

// 坂道の「高さが変化していく方向」を表す軸
enum class SlopeTiltAxis
{
	X, // X軸方向に沿って高さが変化する坂
	Z, // Z軸方向に沿って高さが変化する坂
};

// 斜面の当たり判定
// AABBと違い、面が傾いた地形(坂道)を表現するためのクラス
// 「地面の高さ取得」と「歩行可否の判定」に使うことを想定している
class CollisionSlope
{
public:
	CollisionSlope();
	~CollisionSlope();
public:
	void Draw();			// 描画(デバッグ用)
public:
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetSize(VECTOR size) { m_Size = size; }
	void SetTiltAxis(SlopeTiltAxis axis) { m_TiltAxis = axis; }
	// true: 軸の+方向に向かって高くなる坂 / false: 軸の-方向に向かって高くなる坂
	void SetAscending(bool isAscending) { m_IsAscending = isAscending; }
	VECTOR GetTargetPos() const { return *m_TargetPos; }
	VECTOR GetLocalPos() const { return m_LocalPos; }
	VECTOR GetSize() const { return m_Size; }
public:
	// 指定したワールドXZ座標における、斜面表面のY座標を返す
	// 範囲外の場合は端の高さにクランプして返す
	float GetHeightAt(float worldX, float worldZ) const;
	// 斜面がワールドXZ座標を底面として含んでいるか(高さは無視した平面上の判定)
	bool IsInsideXZ(float worldX, float worldZ) const;
	// 斜面の法線(上向き、正規化済み)を返す
	VECTOR GetNormal() const;
	// 斜面の傾斜角(度)を返す。0度=水平、90度=垂直な壁
	float GetSlopeAngleDeg() const;
	// 歩いて登れる坂かどうか(傾斜角が最大角度以下か)を返す
	bool IsWalkable() const { return GetSlopeAngleDeg() <= m_MaxWalkableAngleDeg; }
	// 歩行可能とみなす最大傾斜角(度)を設定する。これを超えると壁/滑り扱いになる
	void SetMaxWalkableAngleDeg(float angleDeg) { m_MaxWalkableAngleDeg = angleDeg; }
	float GetMaxWalkableAngleDeg() const { return m_MaxWalkableAngleDeg; }
	// 斜面の「下り方向」を表す水平な単位ベクトル(Y成分は0)を返す
	// 急斜面を滑り落ちる処理を作るときの移動方向として使う想定
	VECTOR GetDownhillDirectionXZ() const;
private:
	VECTOR* m_TargetPos;		// 対象の座標(基準点)
	VECTOR m_LocalPos;			// 対象からの相対位置(ボックス中心)
	VECTOR m_Size;				// ボックスのサイズ(幅・高さ範囲・奥行き)
	SlopeTiltAxis m_TiltAxis;	// どの軸方向に高さが変化するか
	bool m_IsAscending;			// 高くなっていく方向が軸の+か-か
	float m_MaxWalkableAngleDeg;	// 歩行可能とみなす最大傾斜角(度)。デフォルト45度
};