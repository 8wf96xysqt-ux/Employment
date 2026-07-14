#include "DxLib.h"
#include "ClearScene.h"
#include "../Scene/SceneManager.h"
#include "../Input/Input.h"

ClearScene::ClearScene() : SceneBase()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init()
{
}

void ClearScene::Load()
{
}

void ClearScene::Start()
{
}

void ClearScene::Step()
{
	// Zキーでタイトルシーンへ
	if (Input::IsTriggerKey(Input::KEY_Z))
	{
		SceneManager::GetInstance()->ChangeScene(TITLE);
	}
}

void ClearScene::Update()
{
}

void ClearScene::Draw()
{
	DrawString(0, 0, "クリアシーン", GetColor(255, 255, 255));
}

void ClearScene::Fin()
{
}
