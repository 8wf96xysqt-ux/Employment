#pragma once

#include "DxLib.h"

class CollisionAABB
{
public:
	CollisionAABB();
	~CollisionAABB();

public:
	void Draw();			// 描画

public:
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetSize(VECTOR size) { m_Size = size; }

	VECTOR GetTargetPos() const { return *m_TargetPos; }
	VECTOR GetLocalPos() const { return m_LocalPos; }
	VECTOR GetSize() const { return m_Size; }

public:
	bool CheckAABB(const CollisionAABB* other) const;
	bool CheckRayHit(VECTOR rayStart, VECTOR rayEnd) const;
	bool CheckRayHit(VECTOR rayStart, VECTOR rayEnd, float* outT) const;
private:
	// 対象の座標
	VECTOR* m_TargetPos;
	// 対象の座標を原点としたローカル座標
	VECTOR m_LocalPos;
	// 縦横奥行き幅
	VECTOR m_Size;
};