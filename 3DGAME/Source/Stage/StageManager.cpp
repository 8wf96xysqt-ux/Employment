
#include "StageManager.h"
#include "StageParameter.h"
#include "../StageObject/StageObjectManager.h"
#include "../Enemy/EnemyManager.h"
#include <fstream>
#include "../StageObject/Floor/FloorParameter.h"
#include "../StageObject/Block/BlockParameter.h"
#include "../StageObject/Ground/GroundParameter.h"
#include "../StageObject/Prop/PropParameter.h"

// 静的メンバ変数の初期化
StageManager* StageManager::m_Instance = nullptr;

using json = nlohmann::json;
constexpr const char* KEY_ITEMS = "items";

// コンストラクタ
StageManager::StageManager()
{
}

// デストラクタ
StageManager::~StageManager()
{
    Fin();
}

// 地形JSONとエネミーJSONを読み込む
void StageManager::Load(const char* fileName)
{
    // 前回の配置データをリセット
    m_Objects.clear();
    m_Enemies.clear();

    // 地形データの読み込み
    std::ifstream stageFile(fileName);
    if (stageFile.is_open())
    {
        json stageJson;
        stageFile >> stageJson;

        m_Objects = stageJson[KEY_ITEMS].get<std::vector<GameObject>>();

        stageFile.close();
    }

    // エネミー用JSONのパスを生成
    std::string stagePath = fileName;
    std::string enemyPath;

    size_t lastSlash = stagePath.find_last_of("/\\");
    if (lastSlash != std::string::npos)
    {
        enemyPath = stagePath.substr(0, lastSlash + 1)
            + "enemy_" + stagePath.substr(lastSlash + 1);
    }
    else
    {
        enemyPath = "enemy_" + stagePath;
    }

    // エネミーデータの読み込み
    std::ifstream enemyFile(enemyPath);
    if (enemyFile.is_open())
    {
        json enemyJson;
        enemyFile >> enemyJson;

        m_Enemies = enemyJson[KEY_ITEMS].get<std::vector<EnemyObject>>();

        enemyFile.close();
    }
}

// 地形と敵を生成する
void StageManager::Start()
{
    // 地形オブジェクトの生成
    for (const GameObject& obj : m_Objects)
    {
        if (obj.id == FLOOR_00)
        {
            StageObjectManager::GetInstance()->CreateFloor(
                FLOOR_MODEL_00, obj.pos, obj.rot, obj.scale);
        }
        else if (obj.id == BLOCK_00)
        {
            StageObjectManager::GetInstance()->CreateBlock(
                BLOCK_MODEL_00, obj.pos, obj.rot, obj.scale);
        }
        else if (obj.id == DIRT_FLOOR_01)
        {
            StageObjectManager::GetInstance()->CreateGround(
                DIRT_GROUND_MODEL_01, obj.pos, obj.rot, obj.scale);
        }
        else if (obj.id == GRASS_FLOOR_01)
        {
            StageObjectManager::GetInstance()->CreateGround(
                GRASS_GROUND_MODEL_01, obj.pos, obj.rot, obj.scale);
        }
        else if (obj.id == FENCE_00)
        {
            StageObjectManager::GetInstance()->CreateProp(
                FENCE_MODEL_00, obj.pos, obj.rot, obj.scale);
        }
        else if (obj.id == FENCE_01)
        {
            StageObjectManager::GetInstance()->CreateProp(
                FENCE_MODEL_01, obj.pos, obj.rot, obj.scale);
        }
    }

    // 敵を初期配置から生成
    ResetEnemies();
}

// 敵を初期配置に戻す
void StageManager::ResetEnemies()
{
    // 現在の敵をすべて削除
    EnemyManager::GetInstance()->ClearEnemies();

    // 保存している初期配置データから敵を再生成
    for (const EnemyObject& enemy : m_Enemies)
    {
        EnemyManager::GetInstance()->CreateEnemy(
            enemy.id,
            enemy.name,
            enemy.pos,
            enemy.rot,
            enemy.scale,
            enemy.movementType,
            enemy.circleRadius,
            enemy.clockwise,
            enemy.patrolPoints
        );
    }
}

// 描画処理
void StageManager::Draw()
{
}

// メモリ解放処理
void StageManager::Fin()
{
    m_Objects.clear();
    m_Enemies.clear();
}
