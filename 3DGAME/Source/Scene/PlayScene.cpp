#include "DxLib.h"
#include "PlayScene.h"
#include "../Scene/SceneManager.h"
#include "../Input/Input.h"
#include "../Collision/CollisionManager.h"
#include "../Player/PlayerManager.h"
#include "../Camera/CameraManager.h"
#include "../Stage/StageManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../MyEffekseer/EffekseerManager.h"
#include "../ParameterData/AttackDataManager.h"
#include "../ShadowMap/ShadowMap.h"
#include "../ParameterData/PlayerDataManager.h"



PlayScene::PlayScene()
	: SceneBase()
{
}

PlayScene::~PlayScene()
{
	Fin();
}

void PlayScene::Init()
{
	// Zバッファ
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// コリジョンマネージャー生成
	CollisionManager::CreateInstance();

	// プレイヤーデータ生成・ロード
	PlayerDataManager::CreateInstance();
	PlayerDataManager::GetInstance()->Load();

	//アタックマネージャー生成
	AttackDataManager::CreateInstance();
	
	// Effekseer初期化
	EffekseerManager::CreateInstence();
	EffekseerManager::GetInstance()->Setup();
	EffekseerManager::GetInstance()->Init();
	
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

	// シャドウマップ生成
	ShadowMap::CreateInstance();
	ShadowMap::GetInstance()->Init();

	
}

void PlayScene::Load()
{
	// 攻撃データをロード
	AttackDataManager::GetInstance()->Load();

	// プレイヤーをロード
	PlayerManager::GetInstance()->Load();

	// カメラロード
	CameraManager::GetInstance()->Load();

	// stageオブジェクトをロード
	StageObjectManager::GetInstance()->Load();

	// ステージをロード
	StageManager::GetInstance()->Load("Data/Stage/SampleScene.json");

	// Effekseerロード
	EffekseerManager::GetInstance()->Load();
}


void PlayScene::Start()
{
	// Effekseer開始
	EffekseerManager::GetInstance()->Start();

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

	// Effekseer更新
	EffekseerManager::GetInstance()->Step();

}

void PlayScene::Update()
{
	//カメラ更新
	CameraManager::GetInstance()->Update();

	// ステージオブジェクト更新
	StageObjectManager::GetInstance()->Update();

	// プレイヤー更新
	PlayerManager::GetInstance()->Update();
	
	// Effekseer更新
	EffekseerManager::GetInstance()->Update();

	
	

}

void PlayScene::Draw()
{
	// シャドウマップ作成
	ShadowMap::GetInstance()->StartDrawShadowMap();

	PlayerManager::GetInstance()->Draw();
	StageObjectManager::GetInstance()->Draw();
	StageManager::GetInstance()->Draw();

	ShadowMap::GetInstance()->EndDrawShadowMap();


	// 通常描画
	ShadowMap::GetInstance()->StartAppearsShadowMap();

	PlayerManager::GetInstance()->Draw();
	StageObjectManager::GetInstance()->Draw();
	StageManager::GetInstance()->Draw();

	ShadowMap::GetInstance()->EndAppearsShadowMap();

	CameraManager::GetInstance()->Draw();

	EffekseerManager::GetInstance()->Draw();

	CollisionManager::GetInstance()->Draw();

	PlayerManager::GetInstance()->DrawDebug();
}


void PlayScene::Fin()
{

	// Zバッファ無効
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);

	//AttackData削除
	AttackDataManager::DeleteInstance();

	// ステージオブジェクト削除
	StageObjectManager::DeleteInstance();

	// ステージ削除

	StageManager::DeleteInstance();

	// プレイヤー終了
	if (PlayerManager::GetInstance())
	{
		PlayerManager::GetInstance()->Fin();
		PlayerManager::DeleteInstance();
	}

	//カメラ終了
	if (CameraManager::GetInstance())
	{
		CameraManager::GetInstance()->Fin();
		CameraManager::DeleteInstance();
	}

	// コリジョンマネージャー削除
	CollisionManager::DeleteInstance();

	// シャドウマップ削除
	ShadowMap::DeleteInstance();

	// Effekseer終了
	EffekseerManager::DeleteInstance();


}
