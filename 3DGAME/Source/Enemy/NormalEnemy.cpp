#include "NormalEnemy.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"


NormalEnemy::NormalEnemy()
{
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Init()
{
    m_Handle = MV1LoadModel("Data/Enemy/NormalEnemy.x");

    m_AABB = CollisionManager::GetInstance()->CreateAABB();
    m_AABB->SetTargetPos(&m_Pos);
    m_AABB->SetSize(VGet(1.0f, 1.0f, 1.0f));
    m_AABB->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
}

void NormalEnemy::Update()
{
    UpdateKnockback();

    if (IsKnockback())
    {
        return;
    }

    Player* player = PlayerManager::GetInstance()->GetPlayer();

    if (!player)
    {
        return;
    }

    VECTOR direction = VSub(player->GetPos(), m_Pos);
    direction.y = 0.0f;

    if (VSize(direction) > 0.1f)
    {
        direction = VNorm(direction);

        constexpr float MOVE_SPEED = 0.03f;

        m_Pos.x += direction.x * MOVE_SPEED * TARGET_FPS * FPSSystem::GetDeltaTime();
        m_Pos.z += direction.z * MOVE_SPEED * TARGET_FPS * FPSSystem::GetDeltaTime();
    }
}

void NormalEnemy::Draw()
{
    MV1SetPosition(m_Handle, m_Pos);
    MV1SetRotationXYZ(m_Handle, m_Rot);
    MV1DrawModel(m_Handle);
}