#include "Prop.h"
#include "../../Collision/CollisionManager.h"
#include "../../Collision/CollisionAABB.h"
#include "PropParameter.h"

Prop::Prop()
	: m_PropID(-1)
{
}

void Prop::Start()
{
	VECTOR collisionSize = VGet(2.0f, 1.0f, 0.5f);

	if (m_PropID == FENCE_MODEL_01)
	{
		collisionSize = VGet(0.5f, 1.0f, 2.0f);
	}

	m_AABB = CollisionManager::GetInstance()->CreateAABB();
	m_AABB->SetTargetPos(&m_Pos);
	m_AABB->SetSize(collisionSize);
	m_AABB->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
}

StageObject* Prop::Clone()
{
	Prop* clone = new Prop;

	*clone = *this;
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return clone;
}

void Prop::SetPropID(int id)
{
	m_PropID = id;
}