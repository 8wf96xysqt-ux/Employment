#include "Slope.h"
#include "../../Collision/CollisionManager.h"
#include "../../Collision/CollisionSlope.h"
void Slope::Start()
{
	m_Slope = CollisionManager::GetInstance()->CreateSlope();
	m_Slope->SetTargetPos(&m_Pos);
	m_Slope->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
	m_Slope->SetSize(VGet(1.0f, 1.0f, 1.0f));
	m_Slope->SetTiltAxis(SlopeTiltAxis::X);
	m_Slope->SetAscending(true);
	m_Slope->SetMaxWalkableAngleDeg(45.0f);
	
}
StageObject* Slope::Clone()
{
	Slope* clone = new Slope;
	*clone = *this;
	clone->m_Handle = MV1DuplicateModel(m_Handle);
	return clone;
}