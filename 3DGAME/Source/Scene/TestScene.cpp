#include "DxLib.h"
#include "TestScene.h"
#include "../Scene/SceneManager.h"
#include "../Input/Input.h"
#include "../Player/PlayerManager.h"

TestScene::TestScene() : SceneBase()
{
}

TestScene::~TestScene()
{
	Fin();
}

void TestScene::Init()
{
	// 適当にカメラを設定
	SetCameraNearFar(0.1f, 1000.0f);
	SetCameraPositionAndTargetAndUpVec(VGet(10.0f, 10.0f, -30.0f), VGet(0.0f, 0.0f, 0.0f), VGet(0.0f, 1.0f, 0.0f));

	// プレイヤー
	PlayerManager::CreateInstance();
	PlayerManager::GetInstance()->CreatePlayer();
	PlayerManager::GetInstance()->Init();
}

void TestScene::Load()
{
	PlayerManager::GetInstance()->Load();
}

void TestScene::Start()
{
	PlayerManager::GetInstance()->Start();
}

void TestScene::Step()
{
	PlayerManager::GetInstance()->Step();

	// 1キーでタイトルシーンへ
	if (Input::IsTriggerKey(Input::KEY_1))
	{
		SceneManager::GetInstance()->ChangeScene(TITLE);
	}
}

void TestScene::Update()
{
	PlayerManager::GetInstance()->Update();
}

void TestScene::Draw()
{
	PlayerManager::GetInstance()->Draw();
}

void TestScene::Fin()
{
	// Zバッファーは非常に重いので、
	// 必要な時だけONにしておきたい

	// Zバッファーを無効にする
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);

	// プレイヤー終了
	PlayerManager::GetInstance()->Fin();
}
