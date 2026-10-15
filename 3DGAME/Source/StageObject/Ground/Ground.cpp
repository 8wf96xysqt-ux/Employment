#include "Ground.h"
#include "../../Collision/CollisionManager.h"
#include "../../Collision/CollisionAABB.h"

void Ground::Start()
{
	m_AABB = CollisionManager::GetInstance()->CreateAABB();
	m_AABB->SetTargetPos(&m_Pos);
	m_AABB->SetSize(VGet(2.0f, 1.0f, 2.0f));
	m_AABB->SetLocalPos(VGet(0.0f, 0.0f, 0.0f));
}

StageObject* Ground::Clone()
{
	Ground* clone = new Ground;

	*clone = *this;
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return clone;
}

