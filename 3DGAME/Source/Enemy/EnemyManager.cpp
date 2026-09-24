#include "EnemyManager.h"
#include "EnemyBase.h"
#include "NormalEnemy.h"

// 静的変数の初期化
EnemyManager* EnemyManager::m_Instance = nullptr;

// コンストラクタ
EnemyManager::EnemyManager()
{
}

// デストラクタ
EnemyManager::~EnemyManager()
{
    Fin();
}

// 初期化
void EnemyManager::Init()
{
}

void EnemyManager::Load()
{

}

void EnemyManager::Start()
{

}

void EnemyManager::Step()
{

}



// 更新
void EnemyManager::Update()
{
    for (auto& enemy : m_Enemies)
    {
        if (enemy)
        {
            enemy->Update();
        }
    }
}

// 描画
void EnemyManager::Draw()
{
    for (auto& enemy : m_Enemies)
    {
        if (enemy)
        {
            enemy->Draw();
        }
    }
}

// デバッグ描画
void EnemyManager::DrawDebug()
{
}

// 終了処理
void EnemyManager::Fin()
{
    m_Enemies.clear();
}

// 敵を生成
void EnemyManager::CreateEnemy(const std::string& name, VECTOR pos, VECTOR rot, VECTOR scale)
{
    if (name == "NormalEnemy")
    {
        std::unique_ptr<NormalEnemy> enemy = std::make_unique<NormalEnemy>();

        enemy->SetPos(pos);
        enemy->SetRot(rot);

        enemy->Init();

        AddEnemy(std::move(enemy));
    }
}

// 敵を追加
void EnemyManager::AddEnemy(std::unique_ptr<EnemyBase> enemy)
{
    m_Enemies.push_back(std::move(enemy));
}