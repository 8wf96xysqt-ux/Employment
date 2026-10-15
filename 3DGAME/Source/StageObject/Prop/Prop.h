#pragma once
#include "../StageObject.h"

class CollisionAABB;

// PropƒNƒ‰ƒX
class Prop : public StageObject
{
public:
	Prop();
	~Prop() {};

	void Start() override;
	StageObject* Clone() override;

	void SetPropID(int id);

private:
	int m_PropID;
};