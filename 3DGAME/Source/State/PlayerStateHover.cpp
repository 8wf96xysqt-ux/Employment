#include "PlayerStateHover.h"
#include "PlayerStateFall.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAirAttack.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../FPS/FPS.h"
#include "../MyEffekseer/Effekseer.h"
#include "../MyEffekseer/EffekseerManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionOBB.h"
#include "../Collision/CollisionAABB.h"
#include "../ParameterData/PlayerDataManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/StageObject.h"

PlayerStateHover::PlayerStateHover()
    : m_StartY(0.0f)
    , m_HoverTime(0.0f)
    , m_HoverTimeMax(0.0f)
    , m_IsRising(false)
    , m_LeftBeamEffect(nullptr)
    , m_RightBeamEffect(nullptr)
    , m_LeftBeamHitEffect(nullptr)
    , m_RightBeamHitEffect(nullptr)
    , m_LeftBeamOBB(nullptr)
    , m_RightBeamOBB(nullptr)
    , m_PlayerData(nullptr)
{
}

void PlayerStateHover::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = this;

    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();
    m_pPlayer->SetParameter(m_PlayerData);

    m_StartY = m_pPlayer->GetPos().y;

    m_IsRising = true;

    m_HoverTime = 0.0f;
    m_HoverTimeMax = m_PlayerData->hoverTime;

    VECTOR move = m_pPlayer->GetMove();

    move.y = m_PlayerData->hoverRiseSpeed;

    m_pPlayer->SetMove(move);

    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_HOVER,true,m_PlayerData->animSpeedHover);

    m_LeftBeamEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM,m_pPlayer->GetPos());

    m_RightBeamEffect =EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM,m_pPlayer->GetPos());

    m_LeftBeamHitEffect =EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM_HIT, m_pPlayer->GetPos());

    m_RightBeamHitEffect =EffekseerManager::GetInstance()->PlayEffect( EFFEKSEER_BEAM_HIT, m_pPlayer->GetPos());

    m_LeftBeamOBB = CollisionManager::GetInstance()->CreateOBB();
    m_RightBeamOBB = CollisionManager::GetInstance()->CreateOBB();

    m_LeftBeamOBB->SetTargetPos(m_pPlayer->GetPosPtr());
    m_RightBeamOBB->SetTargetPos(m_pPlayer->GetPosPtr());

    m_LeftBeamOBB->SetSize(VGet(0.25f, 10.0f, 0.25f));
    m_RightBeamOBB->SetSize(VGet(0.25f, 10.0f, 0.25f));

    m_LeftBeamOBB->SetActive(true);
    m_RightBeamOBB->SetActive(true);


}


void PlayerStateHover::Update()
{
    Player* player = m_pPlayer;

    VECTOR move = player->GetMove();

    float currentY = player->GetPos().y;

    // 空中攻撃
    if (Input::IsAttack() && !m_pPlayer->HasAirAttacked())
    {
        ChangeState(new PlayerStateAirAttack());
        return;
    }


    //ホバー開始時の上昇

    if (m_IsRising)
    {
        move.y = m_PlayerData->hoverRiseSpeed;

        // 1体分くらい上昇
        if (currentY >= m_StartY + m_PlayerData->hoverRiseHeight)
        {
            m_IsRising = false;

            // ここから高さを維持
            move.y = 0.0f;
        }
    }

    //ホバー中

    else
    {
        // Y方向を完全に止める
        move.y = 0.0f;

        // ホバー時間
        m_HoverTime += FPSSystem::GetDeltaTime();;

        // 数秒経過したら落下
        if (m_HoverTime >= m_HoverTimeMax)
        {
            ChangeState(new PlayerStateFall());
            return;
        }
    }
    // 空中移動

    VECTOR airMove = player->GetCameraMoveInput();

    float length = MyMath::VecLong(airMove);

    if (length > m_PlayerData->moveInputThreshold)
    {
        airMove = MyMath::VecNormalize(airMove);

        move.x = airMove.x * m_PlayerData->hoverMoveSpeed;
        move.z = airMove.z * m_PlayerData->hoverMoveSpeed;

        player->SetTargetYaw(atan2f(airMove.x, airMove.z));

        player->DirectionRot();
    }
    else
    {
        move.x = 0.0f;
        move.z = 0.0f;
    }

    player->SetMove(move);
    
    // エフェクトを両手に追従させる


    float yaw = player->GetRot().y;

    float cosY = cosf(yaw);
    float sinY = sinf(yaw);

    // レーザー攻撃OBB
    VECTOR leftOBBPos;
    leftOBBPos.x = cosY * -0.25f;
    leftOBBPos.y = -4.5;
    leftOBBPos.z = -sinY * -0.25f;

    VECTOR rightOBBPos;
    rightOBBPos.x = cosY * 0.25f;
    rightOBBPos.y = -4.5;
    rightOBBPos.z = -sinY * 0.25f;

    m_LeftBeamOBB->SetLocalPos(leftOBBPos);
    m_RightBeamOBB->SetLocalPos(rightOBBPos);

    m_LeftBeamOBB->SetRotation(yaw);
    m_RightBeamOBB->SetRotation(yaw);

    VECTOR playerPos = player->GetPos();

    // プレイヤーの右方向
    VECTOR right = VGet(cosY,0.0f, -sinY );

    // 左手
    VECTOR leftPos = VAdd( playerPos,VScale(right, -0.25f) );

    leftPos.y += 0.28f;

    // 右手
    VECTOR rightPos = VAdd( playerPos,VScale(right, +0.25f));

    rightPos.y += 0.28f;

    // 地面判定
    const auto& objects = StageObjectManager::GetInstance()->GetStageObjects();

    VECTOR beamPositions[2] =
    {
        leftPos,
        rightPos
    };

    EffekseerEffect* hitEffects[2] =
    {
        m_LeftBeamHitEffect,
        m_RightBeamHitEffect
    };

    for (int i = 0; i < 2; i++)
    {
        VECTOR rayStart = beamPositions[i];

        // 真下にRayを飛ばす
        VECTOR rayEnd = VAdd(rayStart, VGet(0.0f, -20.0f, 0.0f));

        float nearestT = 1.0f;
        bool hitGround = false;

        for (auto obj : objects)
        {
            const CollisionAABB* aabb = obj->GetAABB();

            if (!aabb)
                continue;

            float t = 0.0f;

            if (aabb->CheckRayHit(rayStart, rayEnd, &t))
            {
                if (t < nearestT)
                {
                    nearestT = t;
                    hitGround = true;
                }
            }
        }

        if (hitGround)
        {
            // Rayが地面に当たった位置を求める
            VECTOR hitPos = VAdd( rayStart,VScale(VSub(rayEnd, rayStart),nearestT));

            // 着弾エフェクトが無ければ再生
            if (!hitEffects[i] || !hitEffects[i]->IsActive())
            {
                hitEffects[i] =EffekseerManager::GetInstance()->PlayEffect( EFFEKSEER_BEAM_HIT,hitPos);
            }
            else
            {
                // 地面に追従
                hitEffects[i]->SetPos(hitPos);
            }
        }
        else
        {
            // 地面が無ければ消す
            if (hitEffects[i])
            {
                hitEffects[i]->Stop();
                hitEffects[i] = nullptr;
            }
        }
    }

    m_LeftBeamHitEffect = hitEffects[0];
    m_RightBeamHitEffect = hitEffects[1];

    if (m_LeftBeamEffect && m_LeftBeamEffect->IsActive())
    {
        m_LeftBeamEffect->SetPos(leftPos);
    }

    if (m_RightBeamEffect && m_RightBeamEffect->IsActive())
    {
        m_RightBeamEffect->SetPos(rightPos);
    }
    // 着地

    if (player->IsGround())
    {
        m_pPlayer->SetHasHovered(false);
        ChangeState(new PlayerStateIdle());
        return;
    }


    // ジャンプボタンを離したら落下

    if (!Input::IsJump())
    {
        ChangeState(new PlayerStateFall());
        return;
    }
}


void PlayerStateHover::Exit()
{
    if (m_LeftBeamEffect)
    {
        m_LeftBeamEffect->Stop();
        m_LeftBeamEffect = nullptr;
    }

    if (m_RightBeamEffect)
    {
        m_RightBeamEffect->Stop();
        m_RightBeamEffect = nullptr;
    }

    if (m_LeftBeamHitEffect)
    {
        m_LeftBeamHitEffect->Stop();
        m_LeftBeamHitEffect = nullptr;
    }

    if (m_RightBeamHitEffect)
    {
        m_RightBeamHitEffect->Stop();
        m_RightBeamHitEffect = nullptr;
    }

    if (m_LeftBeamOBB)
    {
        m_LeftBeamOBB->SetActive(false);
    }

    if (m_RightBeamOBB)
    {
        m_RightBeamOBB->SetActive(false);
    }
}