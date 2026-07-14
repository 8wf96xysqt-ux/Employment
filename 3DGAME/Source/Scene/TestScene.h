#pragma once
#include "SceneBase.h"

class TestScene : public SceneBase
{
public:
	TestScene();
	~TestScene();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;
};


