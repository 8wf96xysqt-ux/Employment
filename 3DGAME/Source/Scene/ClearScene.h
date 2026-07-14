#pragma once
#include "SceneBase.h"

class ClearScene : public SceneBase
{
public:
	ClearScene();
	~ClearScene();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;
};


