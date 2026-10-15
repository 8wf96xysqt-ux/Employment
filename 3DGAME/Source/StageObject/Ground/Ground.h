#pragma once
#include "../StageObject.h"

class CollisionAABB;

// è∞ÉNÉâÉX
class Ground : public StageObject
{
public:
	Ground() = default;
	virtual ~Ground() = default;

	void Start() override;
	StageObject* Clone() override;

	bool IsCameraCollisionTarget() const override { return true; }
};
