#include "NormalEnemy.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../EnemyAI/BehaviorTree/BehaviorTree.h"
#include "../EnemyAI/Blackboard/BlackBoard.h"
#include "../EnemyAI/BuildTree/NormalEnemyTree.h"
#include "../GameSetting/Color.h"

NormalEnemy::NormalEnemy()
    : m_BasePos(VGet(0.0f, 0.0f, 0.0f))
    , m_BehaviorTree(nullptr)
    , m_PlayDeathAnimation(false)
{
}

NormalEnemy::~NormalEnemy()
{
    Fin();
}

void NormalEnemy::Init()
{
    CollisionManager* collisionManager = CollisionManager::GetInstance();
    if (!collisionManager) return;

    m_AABB = collisionManager->CreateAABB();
    if (!m_AABB) return;

    m_AABB->SetTargetPos(&m_Pos);
    m_AABB->SetSize(VGet(1.0f, 1.0f, 1.0f));
    m_AABB->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
}

void NormalEnemy::Load()
{
    m_Handle = MV1LoadModel("Data/Enemy/Enemy_00.x");
}

void NormalEnemy::Start()
{
    m_Animation.Init(m_Handle);
    m_Animation.Play(NORMAL_ANIMATION_IDLE, true, 0.8f);

    m_BehaviorTree = new BehaviorTree();

    BlackBoard* blackBoard = m_BehaviorTree->GetBlackBoard();

    blackBoard->SetValue("Enemy", static_cast<EnemyBase*>(this));
    blackBoard->SetValue("Player", PlayerManager::GetInstance()->GetPlayer());
    blackBoard->SetValue("BasePos", m_Pos);
    blackBoard->SetValue("ReturnToBase", false);
    blackBoard->SetValue("IsChasing", false);

    m_BehaviorTree->SetRootNode(NormalEnemyTree::Build(blackBoard));
    m_BehaviorTree->Init();
}

void NormalEnemy::Step()
{
    if (IsDead() && !IsKnockback()) return;

    m_PrevPos = m_Pos;

    if (!IsDead())
    {
        UpdateInvincible();
    }

    UpdateKnockback();
}

void NormalEnemy::Die()
{
    if (IsDead()) return;

    Player* player = PlayerManager::GetInstance()->GetPlayer();
    m_PlayDeathAnimation =
        player && player->GetStateType() == PlayerStateType::HOVER;

    CharacterBase::Die();

    if (m_BehaviorTree)
    {
        m_BehaviorTree->Finalize();
        delete m_BehaviorTree;
        m_BehaviorTree = nullptr;
    }

    if (m_PlayDeathAnimation)
    {
        m_Animation.Play(NORMAL_ANIMATION_DIE, false, 1.0f);
    }
}

bool NormalEnemy::IsDeathAnimationFinished() const
{
    if (!IsDead()) return false;
    if (!m_PlayDeathAnimation) return true;

    return m_Animation.GetCurrentAnimation() == NORMAL_ANIMATION_DIE && m_Animation.IsFinished();
}

void NormalEnemy::PlayAnimation(int anim, bool isLoop, float speed)
{
    m_Animation.Play(anim, isLoop, speed);
}
void NormalEnemy::Update()
{
    if (IsDead())
    {
        if (m_PlayDeathAnimation)
        {
            m_Animation.Update();
        }
        return;
    }

    if (IsKnockback()) return;

    if (m_BehaviorTree)
    {
        m_BehaviorTree->Tick();
    }

    m_Animation.Update();
}

void NormalEnemy::Draw()
{
    if (m_Handle == -1) return;

    MV1SetPosition(m_Handle, m_Pos);
    MV1SetRotationXYZ(m_Handle, m_Rot);
    MV1DrawModel(m_Handle);

#ifdef _DEBUG
    if (m_BehaviorTree)
    {
        BlackBoard* blackBoard = m_BehaviorTree->GetBlackBoard();

        if (blackBoard->HasKey("CurrentNode"))
        {
            const char* nodeName =
                blackBoard->GetValue<const char*>("CurrentNode");

            DrawFormatString(0, 300, Color::White(),
                "Current Node : %s", nodeName);
        }
    }
#endif
}

void NormalEnemy::Fin()
{
    if (m_BehaviorTree)
    {
        m_BehaviorTree->Finalize();
        delete m_BehaviorTree;
        m_BehaviorTree = nullptr;
    }

    if (m_AABB)
    {
        CollisionManager::GetInstance()->DeleteAABB(m_AABB);
        m_AABB = nullptr;
    }

    if (m_Handle != -1)
    {
        MV1DeleteModel(m_Handle);
        m_Handle = -1;
    }
}
