#include "DxLib.h"
#include "GameSetting/GameSetting.h"
#include "Scene/SceneManager.h"
#include "Input/Input.h"
#include "FPS/FPS.h"
#include <windows.h>




int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	int width = GetPrivateProfileIntA("Display", "Width", SCREEN_WIDTH, ".\\Data\\Launcher\\config.ini");
	int height = GetPrivateProfileIntA("Display", "Height", SCREEN_HEIGHT, ".\\Data\\Launcher\\config.ini");
	int fullscreen = GetPrivateProfileIntA("Display", "Fullscreen", 0, ".\\Data\\Launcher\\config.ini");


	ChangeWindowMode(fullscreen == 0);
	SetMainWindowText("Project Bot");
	SetGraphMode(width, height, 32);



	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	// シーンマネージャー生成
	SceneManager::CreateInstance();
	// シーン初期化
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Init();

	// 入力初期化
	Input::Init();

	// FPS初期化
	FPSSystem::Init();

	// ゲームのメインループ

	

	while (ProcessMessage() >= 0)
	{
		Sleep(1);
		ClearDrawScreen();

		// 入力更新
		Input::Update();

		// FPS更新
		FPSSystem::Update();
		
		// シーンを更新
		sceneManager->Update();

		// FPS描画
		FPSSystem::Draw();

		// 入力描画
		Input::Draw();

		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// 1フレームが速すぎた場合の待機
		FPSSystem::WaitFPS();

		ScreenFlip();
	}

	// シーンマネージャー削除
	SceneManager::DeleteInstance();

	// 入力終了
	Input::Fin();


	DxLib_End();

	return 0;
}