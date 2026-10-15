#pragma once
#include "../StageObject.h"
class CollisionAABB;
class Block : public StageObject
{
public:
	Block() = default;
	virtual ~Block() = default;
	void Start() override;
	StageObject* Clone() override;
	bool IsOcclusionTarget() override { return true; }
	bool IsCameraCollisionTarget() const override{return true;}
	bool IsPathfindingTarget() const override{ return true; }
};