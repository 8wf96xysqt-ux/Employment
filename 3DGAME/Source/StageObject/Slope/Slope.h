#pragma once
#include "../StageObject.h"

class CollisionSlope;

class Slope : public StageObject
{
public:
	
	Slope() = default;
	virtual ~Slope() = default;
	void Start() override;
	StageObject* Clone() override;
	bool IsOcclusionTarget() override { return true; }

private:
	CollisionSlope* m_Slope;
};