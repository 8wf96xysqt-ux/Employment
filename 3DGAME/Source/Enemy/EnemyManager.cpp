#include "EnemyManager.h"
#include "EnemyBase.h"
#include "NormalEnemy.h"
#include "../GameSetting/Color.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Collision/CollisionAABB.h"
#include <algorithm>
#include "../StageObject/StageObjectManager.h"

EnemyManager* EnemyManager::m_Instance = nullptr;

EnemyManager::EnemyManager() {}
EnemyManager::~EnemyManager() { Fin(); }

void EnemyManager::Init() {}

void EnemyManager::Load()
{
    for (auto& enemy : m_Enemies) { if (enemy) enemy->Load(); }
}

void EnemyManager::Start()
{
    for (auto& enemy : m_Enemies) { if (enemy) enemy->Start(); }
}

void EnemyManager::Step()
{
    for (auto& enemy : m_Enemies) { if (enemy) enemy->Step(); }
    const auto& objects = StageObjectManager::GetInstance()->GetStageObjects();
    for (auto& enemy : m_Enemies) { if (enemy) enemy->CheckHitStageObjects(objects); }
    CheckEnemyCollision();
}

void EnemyManager::Update()
{
    for (auto& enemy : m_Enemies) { if (enemy) enemy->Update(); }
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (player) CheckPlayerAttack(player);
}

void EnemyManager::Draw()
{
    for (auto& enemy : m_Enemies) { if (enemy) enemy->Draw(); }
}

void EnemyManager::DrawDebug()
{
#ifdef _DEBUG
    DrawFormatString(10, 400, Color::White(), "Enemy Count : %d", static_cast<int>(m_Enemies.size()));
#endif
}

void EnemyManager::Fin() { m_Enemies.clear(); }

void EnemyManager::CreateEnemy(int id, const std::string& name, VECTOR pos, VECTOR rot, VECTOR scale,
    int moveType, float radius, bool clockwise, const std::vector<VECTOR>& points)
{
    std::unique_ptr<NormalEnemy> enemy = std::make_unique<NormalEnemy>();

    enemy->SetPos(pos);
    enemy->SetRot(rot);
    //enemy->SetScale(scale);
    enemy->SetMovementParameter(moveType, radius, clockwise, points);

    enemy->Init();
    enemy->Load();
    enemy->Start();

    AddEnemy(std::move(enemy));
}

void EnemyManager::AddEnemy(std::unique_ptr<EnemyBase> enemy) { m_Enemies.push_back(std::move(enemy)); }


void EnemyManager::ClearEnemies()
{
    m_Enemies.clear();
}


void EnemyManager::CheckPlayerAttack(Player* player)
{
    if (!player) return;
    for (auto& enemy : m_Enemies)
    {
        if (!enemy || enemy->IsDead()) continue;
        CollisionAABB* enemyAABB = enemy->GetAABB();
        if (!enemyAABB) continue;
        if (player->CheckAttackHit(enemyAABB)) { enemy->TakeDamage(1); }
    }
    m_Enemies.erase(
        std::remove_if(
            m_Enemies.begin(),
            m_Enemies.end(),
            [](const std::unique_ptr<EnemyBase>& enemy)
            {
                if (!enemy) return true;

                // 死亡していてもノックバック中は削除しない
                if (!enemy->IsDead()) return false;
                if (enemy->IsKnockback()) return false;

                return enemy->IsDeathAnimationFinished();
            }),
        m_Enemies.end());
}

void EnemyManager::CheckEnemyCollision()
{
    for (int i = 0; i < static_cast<int>(m_Enemies.size()); i++)
    {
        for (int j = i + 1; j < static_cast<int>(m_Enemies.size()); j++)
        {
            EnemyBase* enemyA = m_Enemies[i].get();
            EnemyBase* enemyB = m_Enemies[j].get();
            if (!enemyA || !enemyB || enemyA->IsDead() || enemyB->IsDead()) continue;

            CollisionAABB* aabbA = enemyA->GetAABB();
            CollisionAABB* aabbB = enemyB->GetAABB();
            if (!aabbA || !aabbB || !aabbA->CheckAABB(aabbB)) continue;

            VECTOR posA = enemyA->GetPos(); VECTOR posB = enemyB->GetPos();
            VECTOR centerA = VAdd(posA, aabbA->GetLocalPos());
            VECTOR centerB = VAdd(posB, aabbB->GetLocalPos());
            VECTOR sizeA = aabbA->GetSize(); VECTOR sizeB = aabbB->GetSize();

            float overlapX = (sizeA.x + sizeB.x) * 0.5f - fabsf(centerA.x - centerB.x);
            float overlapZ = (sizeA.z + sizeB.z) * 0.5f - fabsf(centerA.z - centerB.z);

            if (overlapX < overlapZ)
            {
                float push = overlapX * 0.5f;
                if (centerA.x < centerB.x) { posA.x -= push; posB.x += push; }
                else { posA.x += push; posB.x -= push; }
            }
            else
            {
                float push = overlapZ * 0.5f;
                if (centerA.z < centerB.z) { posA.z -= push; posB.z += push; }
                else { posA.z += push; posB.z -= push; }
            }
            enemyA->SetPos(posA); enemyB->SetPos(posB);
        }
    }
}
