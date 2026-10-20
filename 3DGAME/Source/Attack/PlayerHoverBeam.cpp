#include "PlayerHoverBeam.h"
#include "../Player/Player.h"
#include "../MyEffekseer/Effekseer.h"
#include "../MyEffekseer/EffekseerManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionOBB.h"
#include "../Collision/CollisionAABB.h"
#include "../ParameterData/AttackDataManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/StageObject.h"

#include <cmath>

namespace
{
    constexpr float BEAM_HAND_OFFSET_X = 0.25f;
    constexpr float BEAM_HAND_OFFSET_Y = 0.25f;
}

PlayerHoverBeam::PlayerHoverBeam()
    : m_pPlayer(nullptr)
    , m_AttackData(nullptr)
    , m_LeftBeamEffect(nullptr)
    , m_RightBeamEffect(nullptr)
    , m_LeftBeamHitEffect(nullptr)
    , m_RightBeamHitEffect(nullptr)
    , m_LeftBeamOBB(nullptr)
    , m_RightBeamOBB(nullptr)
{
}

PlayerHoverBeam::~PlayerHoverBeam()
{
    Fin();
}

void PlayerHoverBeam::Start(Player* player, const AttackData* attackData)
{
    // 既存のリソースがあれば解放
    Fin();

    if (!player || !attackData)
    {
        return;
    }

    m_pPlayer = player;
    m_AttackData = attackData;

    // ビームエフェクトを生成
    m_LeftBeamEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM, m_pPlayer->GetPos());

    m_RightBeamEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM, m_pPlayer->GetPos());

    // 着弾エフェクトを生成
    m_LeftBeamHitEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM_HIT, m_pPlayer->GetPos());

    m_RightBeamHitEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM_HIT, m_pPlayer->GetPos());

    // ビーム攻撃用のOBBを生成
    m_LeftBeamOBB = CollisionManager::GetInstance()->CreateOBB();
    m_RightBeamOBB = CollisionManager::GetInstance()->CreateOBB();

    VECTOR hitboxSize = VGet(
        m_AttackData->hitboxSizeX,
        m_AttackData->hitboxSizeY,
        m_AttackData->hitboxSizeZ);

    if (m_LeftBeamOBB)
    {
        m_LeftBeamOBB->SetTargetPos(m_pPlayer->GetPosPtr());
        m_LeftBeamOBB->SetSize(hitboxSize);
        m_LeftBeamOBB->SetActive(true);
    }

    if (m_RightBeamOBB)
    {
        m_RightBeamOBB->SetTargetPos(m_pPlayer->GetPosPtr());
        m_RightBeamOBB->SetSize(hitboxSize);
        m_RightBeamOBB->SetActive(true);
    }
}

void PlayerHoverBeam::Update()
{
    if (!m_pPlayer || !m_AttackData)
    {
        return;
    }

    UpdateBeamOBB();
    UpdateBeamEffects();
}

void PlayerHoverBeam::UpdateBeamOBB()
{
    float yaw = m_pPlayer->GetRot().y;

    float cosY = cosf(yaw);
    float sinY = sinf(yaw);

    // 攻撃データからビームの横方向の位置を取得
    float offsetX = m_AttackData->hitboxOffsetX;

    // 左ビームの位置
    VECTOR leftOBBPos;
    leftOBBPos.x = cosY * -offsetX;
    leftOBBPos.y = m_AttackData->hitboxOffsetY;
    leftOBBPos.z = -sinY * -offsetX;

    // 右ビームの位置
    VECTOR rightOBBPos;
    rightOBBPos.x = cosY * offsetX;
    rightOBBPos.y = m_AttackData->hitboxOffsetY;
    rightOBBPos.z = -sinY * offsetX;

    if (m_LeftBeamOBB)
    {
        m_LeftBeamOBB->SetLocalPos(leftOBBPos);
        m_LeftBeamOBB->SetRotation(yaw);
    }

    if (m_RightBeamOBB)
    {
        m_RightBeamOBB->SetLocalPos(rightOBBPos);
        m_RightBeamOBB->SetRotation(yaw);
    }
}

void PlayerHoverBeam::UpdateBeamEffects()
{
    float yaw = m_pPlayer->GetRot().y;

    float cosY = cosf(yaw);
    float sinY = sinf(yaw);

    VECTOR playerPos = m_pPlayer->GetPos();

    // プレイヤーの右方向
    VECTOR right = VGet(cosY, 0.0f, -sinY);

    // 左手の位置
    VECTOR leftPos = VAdd(playerPos, VScale(right, -BEAM_HAND_OFFSET_X));
    leftPos.y += BEAM_HAND_OFFSET_Y;

    // 右手の位置
    VECTOR rightPos = VAdd(playerPos, VScale(right, BEAM_HAND_OFFSET_X));
    rightPos.y += BEAM_HAND_OFFSET_Y;

    // ビームエフェクトを手の位置に追従させる
    if (m_LeftBeamEffect && m_LeftBeamEffect->IsActive())
    {
        m_LeftBeamEffect->SetPos(leftPos);
    }

    if (m_RightBeamEffect && m_RightBeamEffect->IsActive())
    {
        m_RightBeamEffect->SetPos(rightPos);
    }

    // ステージオブジェクトを取得
    const auto& objects = StageObjectManager::GetInstance()->GetStageObjects();

    // 左右のビームを同じ処理で扱う
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
        VECTOR rayEnd = VAdd(rayStart,VGet(0.0f, -m_AttackData->rayLength, 0.0f));

        float nearestT = 1.0f;
        bool hitGround = false;

        // すべてのステージオブジェクトとRayの当たり判定
        for (auto obj : objects)
        {
            const CollisionAABB* aabb = obj->GetAABB();

            if (!aabb)
            {
                continue;
            }

            float t = 0.0f;

            if (aabb->CheckRayHit(rayStart, rayEnd, &t))
            {
                // 最も近い地面の位置を取得
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
            VECTOR hitPos = VAdd(rayStart,VScale(VSub(rayEnd, rayStart), nearestT));

            // 着弾エフェクトがなければ再生
            if (!hitEffects[i] || !hitEffects[i]->IsActive())
            {
                hitEffects[i] = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM_HIT, hitPos);
            }
            else
            {
                // 着弾エフェクトを地面に追従させる
                hitEffects[i]->SetPos(hitPos);
            }
        }
        else
        {
            // 地面がなければ着弾エフェクトを停止
            if (hitEffects[i])
            {
                hitEffects[i]->Stop();
                hitEffects[i] = nullptr;
            }
        }
    }

    // 更新したエフェクトをメンバー変数へ戻す
    m_LeftBeamHitEffect = hitEffects[0];
    m_RightBeamHitEffect = hitEffects[1];
}

bool PlayerHoverBeam::CheckAttackHit(CollisionAABB* target)
{
    if (!target)
    {
        return false;
    }

    // 左ビームの攻撃判定
    if (m_LeftBeamOBB && m_LeftBeamOBB->IsActive())
    {
        if (target->CheckOBB(m_LeftBeamOBB))
        {
            return true;
        }
    }

    // 右ビームの攻撃判定
    if (m_RightBeamOBB && m_RightBeamOBB->IsActive())
    {
        if (target->CheckOBB(m_RightBeamOBB))
        {
            return true;
        }
    }

    return false;
}

void PlayerHoverBeam::Fin()
{
    // ビームエフェクトを停止
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

    // 着弾エフェクトを停止
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

    // ビーム攻撃用OBBを削除
    if (m_LeftBeamOBB)
    {
        CollisionManager::GetInstance()->DeleteOBB(m_LeftBeamOBB);
        m_LeftBeamOBB = nullptr;
    }

    if (m_RightBeamOBB)
    {
        CollisionManager::GetInstance()->DeleteOBB(m_RightBeamOBB);
        m_RightBeamOBB = nullptr;
    }

    m_pPlayer = nullptr;
    m_AttackData = nullptr;
}