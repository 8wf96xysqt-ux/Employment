#include "DxLib.h"
#include "PlayScene.h"
#include "../Scene/SceneManager.h"
#include "../Input/Input.h"
#include "../Collision/CollisionManager.h"
#include "../Player/PlayerManager.h"
#include "../Camera/CameraManager.h"
#include "../Stage/StageManager.h"
#include "../StageObject/StageObjectManager.h"



PlayScene::PlayScene() : SceneBase()
{
	m_Floor = nullptr;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	// コリジョンマネージャー生成
	CollisionManager::CreateInstance();

	// プレイヤー生成
	PlayerManager::CreateInstance();
	PlayerManager::GetInstance()->CreatePlayer();
	PlayerManager::GetInstance()->Init();
	CameraManager::CreateInstance();
	CameraManager::GetInstance()->CreateCamera();
	CameraManager::GetInstance()->Init();

	// ステージオブジェクト生成/初期化
	StageObjectManager::CreateInstance();
	StageObjectManager::GetInstance()->Init();

	// ステージマネージャー生成
	StageManager::CreateInstance();

	
}

void PlayScene::Load()
{
	// プレイヤーをロード
	PlayerManager::GetInstance()->Load();

	// カメラロード
	CameraManager::GetInstance()->Load();

	// stageオブジェクトをロード
	StageObjectManager::GetInstance()->Load();

	// ステージをロード
	StageManager::GetInstance()->Load("Data/Stage/SampleScene.json");
}


void PlayScene::Start()
{

	// ステージ開始
	StageManager::GetInstance()->Start();

	// ステージオブジェクト開始
	StageObjectManager::GetInstance()->Start();
	// プレイヤー開始
	PlayerManager::GetInstance()->Start();

	// カメラ開始
	CameraManager::GetInstance()->Start();



	
	
}

void PlayScene::Step()
{
	// プレイヤー入力
	PlayerManager::GetInstance()->Step();

	// 当たり判定
	CollisionManager::GetInstance()->CheckCollision();



}

void PlayScene::Update()
{
	//カメラ更新
	CameraManager::GetInstance()->Update();

	// ステージオブジェクト更新
	StageObjectManager::GetInstance()->Update();

	// プレイヤー更新
	PlayerManager::GetInstance()->Update();

	
	

}

void PlayScene::Draw()
{
	// プレイヤー影描画
	PlayerManager::GetInstance()->Draw();

	// ステージオブジェクト描画
	StageObjectManager::GetInstance()->Draw();

	// プレイヤー影描画
	PlayerManager::GetInstance()->Draw();

	
	//カメラ座標描画
	CameraManager::GetInstance()->Draw();


	// 当たり判定描画
	CollisionManager::GetInstance()->Draw();


}

void PlayScene::Fin()
{

	// ステージオブジェクト削除
	StageObjectManager::DeleteInstance();

	// ステージ削除
	StageManager::DeleteInstance();

	if (PlayerManager::GetInstance())
	{
		PlayerManager::GetInstance()->Fin();
		PlayerManager::DeleteInstance();
	}

	if (CameraManager::GetInstance())
	{
		CameraManager::GetInstance()->Fin();
		CameraManager::DeleteInstance();
	}

	CollisionManager::DeleteInstance();


}
