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
#include "../ParameterData/AttackDataManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/StageObject.h"

#include <cmath>
#include <cstdio>

constexpr float BEAM_HAND_OFFSET_X = 0.25f;
constexpr float BEAM_HAND_OFFSET_Y = 0.25f;

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
    , m_AttackData(nullptr)
{
}

void PlayerStateHover::Enter(Player* player)
{
    m_pPlayer = player;

    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();
    m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::HOVER);

    // データが取得できなければホバーを開始しない
    if (!m_PlayerData || !m_AttackData)
    {
        ChangeState(std::make_unique<PlayerStateFall>());
        return;
    }

    // ホバー開始時の状態を初期化
    m_StartY = m_pPlayer->GetPos().y;
    m_IsRising = true;
    m_HoverTime = 0.0f;
    m_HoverTimeMax = m_PlayerData->hoverTime;

    // 上昇を開始
    VECTOR move = m_pPlayer->GetMove();
    move.y = m_PlayerData->hoverRiseSpeed;
    m_pPlayer->SetMove(move);

    // ホバーアニメーションを再生
    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_HOVER,true,m_PlayerData->animSpeedHover);

    // ビームエフェクトを生成
    m_LeftBeamEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM,m_pPlayer->GetPos());

    m_RightBeamEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM,m_pPlayer->GetPos());

    // 着弾エフェクトを生成
    m_LeftBeamHitEffect =
        EffekseerManager::GetInstance()->PlayEffect(
            EFFEKSEER_BEAM_HIT,
            m_pPlayer->GetPos());

    m_RightBeamHitEffect = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM_HIT,m_pPlayer->GetPos());

    // ビーム攻撃用のOBBを生成
    m_LeftBeamOBB = CollisionManager::GetInstance()->CreateOBB();

    m_RightBeamOBB = CollisionManager::GetInstance()->CreateOBB();

    m_LeftBeamOBB->SetTargetPos(m_pPlayer->GetPosPtr());
    m_RightBeamOBB->SetTargetPos(m_pPlayer->GetPosPtr());

    VECTOR hitboxSize = VGet(
        m_AttackData->hitboxSizeX,
        m_AttackData->hitboxSizeY,
        m_AttackData->hitboxSizeZ);

    m_LeftBeamOBB->SetSize(hitboxSize);
    m_RightBeamOBB->SetSize(hitboxSize);

    m_LeftBeamOBB->SetActive(true);
    m_RightBeamOBB->SetActive(true);
}

void PlayerStateHover::Update()
{
    Player* player = m_pPlayer;

    // 空中攻撃
    if (Input::IsAttack() && !player->GetAttackComponent()->HasAirAttacked())
    {
        ChangeState(std::make_unique<PlayerStateAirAttack>());
        return;
    }

    // ホバー中の移動を更新
    UpdateHoverMovement();

    // レーザーの当たり判定を更新
    UpdateBeamOBB();

    // レーザーのエフェクトを更新
    UpdateBeamEffects();

    // 着地
    if (player->IsGround())
    {
        player->SetHasHovered(false);
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }

    // ジャンプボタンを離したら落下
    if (!Input::IsJump())
    {
        ChangeState(std::make_unique<PlayerStateFall>());
        return;
    }
}

void PlayerStateHover::UpdateHoverMovement()
{
    Player* player = m_pPlayer;

    VECTOR move = player->GetMove();
    float currentY = player->GetPos().y;

    // ホバー開始時の上昇
    if (m_IsRising)
    {
        move.y = m_PlayerData->hoverRiseSpeed;

        // 一定の高さまで上昇したら高さを維持
        if (currentY >= m_StartY + m_PlayerData->hoverRiseHeight)
        {
            m_IsRising = false;
            move.y = 0.0f;
        }
    }
    // 上昇後は一定時間ホバーする
    else
    {
        // Y方向を止める
        move.y = 0.0f;

        // ホバー時間を更新
        m_HoverTime += FPSSystem::GetDeltaTime();

        // ホバー時間を超えたら落下
        if (m_HoverTime >= m_HoverTimeMax)
        {
            ChangeState(std::make_unique<PlayerStateFall>());
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

        // 移動方向へプレイヤーを向ける
        player->SetTargetYaw(atan2f(airMove.x, airMove.z));
        player->DirectionRot();
    }
    else
    {
        // 入力がなければ水平方向の移動を止める
        move.x = 0.0f;
        move.z = 0.0f;
    }

    player->SetMove(move);
}

void PlayerStateHover::UpdateBeamOBB()
{
    Player* player = m_pPlayer;

    float yaw = player->GetRot().y;

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

    m_LeftBeamOBB->SetLocalPos(leftOBBPos);
    m_RightBeamOBB->SetLocalPos(rightOBBPos);

    // プレイヤーの向きに合わせてOBBを回転
    m_LeftBeamOBB->SetRotation(yaw);
    m_RightBeamOBB->SetRotation(yaw);
}

void PlayerStateHover::UpdateBeamEffects()
{
    Player* player = m_pPlayer;

    float yaw = player->GetRot().y;

    float cosY = cosf(yaw);
    float sinY = sinf(yaw);

    VECTOR playerPos = player->GetPos();

    // プレイヤーの右方向
    VECTOR right = VGet(cosY,0.0f,-sinY);

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
    const auto& objects =
        StageObjectManager::GetInstance()->GetStageObjects();

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
                continue;

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
            VECTOR hitPos =VAdd(rayStart,VScale(VSub(rayEnd, rayStart),nearestT));

            // 着弾エフェクトがなければ再生
            if (!hitEffects[i] || !hitEffects[i]->IsActive())
            {
                hitEffects[i] = EffekseerManager::GetInstance()->PlayEffect(EFFEKSEER_BEAM_HIT,hitPos);
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

void PlayerStateHover::Exit()
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

    // ビーム攻撃用OBBを無効化
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
}

bool PlayerStateHover::CheckAttackHit(CollisionAABB* target)
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