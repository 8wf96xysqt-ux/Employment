#pragma once

#include "DxLib.h"

class CollisionOBB;

class CollisionSphere
{
public:
	CollisionSphere();
	~CollisionSphere();

public:
	void Draw();			// 描画

public:
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetRadius(float radius) { m_Radius = radius; }

	VECTOR GetTargetPos() const { return *m_TargetPos; }
	VECTOR GetLocalPos() const { return m_LocalPos; }
	float GetRadius() const { return m_Radius; }


public:
	bool CheckSphere(CollisionSphere* other);
	// Sphere × OBB
	bool CheckOBB(const CollisionOBB* obb) const;

private:
	// 対象の座標
	VECTOR* m_TargetPos;
	// 対象の座標を原点としたローカル座標
	VECTOR m_LocalPos;
	// 球の半径
	float m_Radius;
};