#pragma once

#include "DxLib.h"

class CollisionOBB
{
public:
    CollisionOBB();
    ~CollisionOBB();

public:
    void Draw();

public:
    void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
    void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
    void SetSize(VECTOR size) { m_Size = size; }
    void SetRotation(float rotY) { m_RotY = rotY; }


    VECTOR GetTargetPos() const { return *m_TargetPos; }
    VECTOR GetLocalPos() const { return m_LocalPos; }
    VECTOR GetSize() const { return m_Size; }

    float GetRotation() const { return m_RotY; }


public:
    bool CheckOBB(const CollisionOBB* other) const;

private:

    // 対象の座標
    VECTOR* m_TargetPos;
    // 対象の座標を原点としたローカル座標
    VECTOR m_LocalPos;
    // 縦横奥行き幅
    VECTOR m_Size;
    // Y軸回転
    float m_RotY;
};