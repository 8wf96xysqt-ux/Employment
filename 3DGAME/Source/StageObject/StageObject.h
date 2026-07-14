#pragma once
#include "DxLib.h"
class CollisionAABB;
class StageObject
{
public:
	StageObject();
	virtual ~StageObject();
	virtual void Start();
	void Load(const char* fileName);
	void Update();
	void Draw();
	void Fin();
	virtual StageObject* Clone() = 0;
	CollisionAABB* GetAABB() const { return m_AABB; }
	void SetTransform(VECTOR pos, VECTOR rot, VECTOR scale) { m_Pos = pos; m_Rot = rot; m_Scale = scale; }

	virtual bool IsOcclusionTarget() { return false; }
	void SetTransparency(float alpha) { m_TargetAlpha = alpha; }
	float GetTransparency() const { return m_Alpha; }

protected:
	int m_Handle;
	VECTOR m_Pos;
	VECTOR m_Rot;
	VECTOR m_Scale;
	CollisionAABB* m_AABB;	// “–‚½‚è”»’è
	

	float m_Alpha = 255.0f;
	float m_TargetAlpha = 255.0f;
};