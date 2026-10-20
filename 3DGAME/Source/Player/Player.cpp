#include "Player.h"
#include "../FPS/FPS.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../GameSetting/Color.h"
#include "../Camera/CameraManager.h"
#include "../ParameterData/PlayerDataManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../StageObject/StageObject.h"
#include "../GameSetting/GameSetting.h"
#include "../State/PlayerStateIdle.h"
#include "../State/PlayerStateDead.h"

#include <math.h>

// コンストラクタ
Player::Player()

    : m_PlayerData(nullptr)

    , m_HasHovered(false)

    // コヨーテタイム & 入力バッファ
    , m_CoyoteTime(0.0f)
    , m_CoyoteTimeMax(0.0f)
    , m_JumpBufferTime(0.0f)
    , m_JumpBufferTimeMax(0.0f)

    , m_TargetYawValue(0)

    , m_pState(nullptr)

    , m_RespawnPos(VGet(0.0f, 1.0f, 0.0f))
    ,m_RespawnRot(VGet(0.0f, 0.0f, 0.0f))

    , m_LastGroundY(0.0f)
{
}

// デストラクタ
Player::~Player()
{
    Fin();
}

// 初期化
void Player::Init()
{
    PlayerDataManager* manager = PlayerDataManager::GetInstance();

    if (manager == nullptr)
    {
        return;
    }

    m_PlayerData = manager->GetParameter();

    if (m_PlayerData == nullptr)
    {
        return;
    }

    m_AABB = CollisionManager::GetInstance()->CreateAABB();
    m_AABB->SetTargetPos(&m_Pos);

    m_AABB->SetLocalPos(VGet(m_PlayerData->aabbOffsetX,m_PlayerData->aabbOffsetY,m_PlayerData->aabbOffsetZ));

    m_AABB->SetSize(VGet( m_PlayerData->aabbSizeX, m_PlayerData->aabbSizeY, m_PlayerData->aabbSizeZ));

    m_AttackComponent.Init(&m_Pos);

    m_CoyoteTimeMax = m_PlayerData->coyoteTime;

    m_JumpBufferTime = 0.0f;
    m_JumpBufferTimeMax = m_PlayerData->jumpBufferTime;
}

// ロード
void Player::Load()
{
    m_Handle = MV1LoadModel("Data/Player/MMP.x");
    if (m_Handle == -1)
    {

        printf("Error: Failed to load model Data/Player/MMP.x\n");
        m_Handle = -1;
    }
}

// 開始
void Player::Start()
{

    // HP初期化
    m_MaxHP = m_PlayerData->maxHP;
    m_HP = m_MaxHP;
    m_IsDead = false;

    m_Pos = VGet(0.0f, 1.0f, 0.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);

    // 初期リスポーン地点を登録
    SetRespawnPoint(m_Pos, m_Rot);

    m_Scale = VGet(m_PlayerData->scale, m_PlayerData->scale,m_PlayerData->scale);

    m_Move = VGet(0.0f, 0.0f, 0.0f);

    m_Animation.Init(m_Handle);
    m_Animation.Play(PLAYER_ANIMATION_IDLE, true,m_PlayerData->animSpeedIdle);

    m_pState = std::make_unique<PlayerStateIdle>();
    m_pState->Enter(this);
}

void Player::Step()
{

    if (IsDead())
    {
        return;
    }
     // Y座標が-5以下になったら即死させる
    if (m_Pos.y <= -5.0f)
    {
        Kill();
        return;
    }

    // 前フレームの位置を保存
    m_PrevPos = m_Pos;

    // ジャンプ入力をバッファに保存
    if (Input::IsTriggerJump())
    {
        SetJumpBuffer();
    }

    UpdateJumpBuffer();

    // 重力・無敵時間を更新
    m_Gravity.Update(m_Move.y);
    UpdateInvincible();
}


void Player::Update()
{
    if (!m_pState)
    {
        return;
    }

    // 次のステートがあれば切り替える
    std::unique_ptr<PlayerStateBase> nextState =
        m_pState->TakeNextState();

    if (nextState)
    {
        m_pState->Exit();

        m_pState = std::move(nextState);

        m_pState->Enter(this);
    }

    // Stateの更新
    m_pState->Update();

    // モデル・アニメーションの更新
    MV1SetPosition(m_Handle, m_Pos);
    MV1SetRotationXYZ(m_Handle, m_Rot);
    MV1SetScale(m_Handle, m_Scale);
    m_Animation.Update();

    // 攻撃判定の更新
    m_AttackComponent.Update(m_Animation.GetNowTime(),m_Rot.y);
}


const char* Player::GetStateTypeName() const
{
    if (m_pState == nullptr)
    {
        return "None";
    }

    return GetPlayerStateTypeName(m_pState->GetStateType());
}

// 描画
void Player::Draw()
{
    // モデル描画
    MV1DrawModel(m_Handle);
}

void Player::DrawDebug()
{
#ifdef _DEBUG

    // デバッグ文字はZバッファの影響を受けないようにする
    SetUseZBuffer3D(FALSE);

    DrawFormatString(0, 0, Color::White(),
        "座標[%.2f, %.2f, %.2f]",
        m_Pos.x, m_Pos.y, m_Pos.z);

    DrawFormatString(0, 20, Color::White(),
        "回転[%.2f, %.2f, %.2f]",
        m_Rot.x, m_Rot.y, m_Rot.z);

    DrawFormatString(0, 40, Color::Red(),
        "State : %s",
        GetStateTypeName());

    DrawFormatString(0, 60, Color::White(),
        "IsGround : %s",
        IsGround() ? "TRUE" : "FALSE");

    DrawFormatString(0, 80, Color::White(),
        "VelocityY : %.3f",
        m_Move.y);

    DrawFormatString(0, 100, Color::White(),
        "HasHovered : %s",
        HasHovered() ? "TRUE" : "FALSE");

    DrawFormatString(0, 120, Color::White(),
        "HasAirAttacked : %s",
        m_AttackComponent.HasAirAttacked() ? "TRUE" : "FALSE");

    DrawFormatString(0, 140, Color::White(),
        "CoyoteTime : %.3f",
        GetCoyoteTime());
    DrawFormatString(0, 260, Color::White(),
        "CoyoteTimeMax : %.3f",
        m_CoyoteTimeMax);

    DrawFormatString(0, 220, Color::White(),
        "InvincibleTime : %.3f",
        m_InvincibleTime);

    DrawFormatString(0, 240, Color::White(),
        "DeltaTime : %.4f",
        FPSSystem::GetDeltaTime());
    DrawFormatString(0, 280, Color::White(),
        "HP : %d / %d",
        GetHP(), GetMaxHP());

    // Zバッファを元に戻す
    SetUseZBuffer3D(TRUE);

#endif
}

// 終了
void Player::Fin()
{
    if (m_pState)
    {
        m_pState->Exit();
        m_pState.reset();
    }

    if (m_AABB)
    {
        CollisionManager::GetInstance()->DeleteAABB(m_AABB);
        m_AABB = nullptr;
    }

    m_AttackComponent.Fin();

    if (m_Handle != -1)
    {
        MV1DeleteModel(m_Handle);
        m_Handle = -1;
    }
}

// 方向回転
void Player::DirectionRot()
{
    RotateToward(m_TargetYawValue,m_PlayerData->rotationSpeed);
}

// カメラ基準の移動方向を取得する
VECTOR Player::GetCameraMoveInput()
{
    float camYaw = CameraManager::GetInstance()->GetCamera()->GetYaw();

    VECTOR camForward = MyMath::VecForwardZX(camYaw);

    VECTOR camRight;

    camRight.x = camForward.z;
    camRight.y = 0.0f;
    camRight.z = -camForward.x;

    VECTOR move = VGet(0, 0, 0);

    VECTOR input = Input::GetMove();

    move = MyMath::VecAdd(move, MyMath::VecScale(camRight, input.x));

    move = MyMath::VecAdd(move, MyMath::VecScale(camForward, input.z));

    return move;
}


// プレイヤーのダメージ処理
void Player::TakeDamage(int damage)
{
    // 死亡中・無敵中・不正なダメージは無視
    if (IsDead() || IsInvincible() || damage <= 0)
    {
        return;
    }

    // HP減少と死亡判定
    CharacterBase::TakeDamage(damage);

    // HPが0になったら死亡ステートへ遷移
    if (IsDead() && m_pState != nullptr)
    {
        m_pState->ChangeState(std::make_unique<PlayerStateDead>());
    }
}

void Player::Kill()
{
    // すでに死亡している場合は何もしない
    if (IsDead())
    {
        return;
    }

    // HPを0にして死亡状態にする
    CharacterBase::TakeDamage(m_HP);

    // 死亡ステートへ遷移
    if (m_pState != nullptr)
    {
        m_pState->ChangeState(std::make_unique<PlayerStateDead>());
    }
}

// リスポーン地点を設定
void Player::SetRespawnPoint(VECTOR pos, VECTOR rot)
{
    m_RespawnPos = pos;
    m_RespawnRot = rot;
}

// リスポーン地点を取得
VECTOR Player::GetRespawnPoint() const
{
    return m_RespawnPos;
}
void Player::Respawn()
{
    // 登録済みのリスポーン地点へ移動
    m_Pos = m_RespawnPos;
    m_Rot = m_RespawnRot;
    m_PrevPos = m_Pos;

    // 移動を停止
    m_Move = VGet(0.0f, 0.0f, 0.0f);

    // HPを全回復
    m_HP = m_MaxHP;
    m_IsDead = false;

    // 無敵状態を解除
    EndInvincible();

    // 重力を再開
    ResetGravity();

    // 地上・ジャンプ関連の状態を初期化
    m_IsGround = false;
    m_HasHovered = false;
    m_CoyoteTime = 0.0f;
    m_JumpBufferTime = 0.0f;

    // 復活したことを通知
    m_HasRespawned = true;

    // 待機ステートへ遷移
    if (m_pState != nullptr)
    {
        m_pState->ChangeState(std::make_unique<PlayerStateIdle>());
    }
}

//分離候補
// ステージ当たり判定
void Player::CheckHitStageObjects(const std::vector<StageObject*>& objects)
{
    bool wasGround = m_IsGround;

    // 共通処理で衝突解決（m_Pos, m_Move, m_IsGround を更新）
    ResolveStageCollisions(objects);

    if (m_IsGround)
    {
        // 着地したのでコヨーテタイム終了
        m_CoyoteTime = 0.0f;

        m_AttackComponent.SetHasAirAttacked(false);

        m_LastGroundY = m_Pos.y;
    }
    else
    {
        // 地面から離れた瞬間だけコヨーテタイム開始
        if (wasGround)
        {
            m_CoyoteTime = m_CoyoteTimeMax;
        }
        else if (m_CoyoteTime > 0.0f)
        {
            m_CoyoteTime -= FPSSystem::GetDeltaTime();

            if (m_CoyoteTime < 0.0f)
            {
                m_CoyoteTime = 0.0f;
            }
        }
    }
}

bool Player::CheckAttackHit(CollisionAABB* target)
{
    if (!target)
    {
        return false;
    }

    // パンチ
    if (m_AttackComponent.CheckHit(target))
    {
        return true;
    }

    // ホバー
    if (m_pState)
    {
        return m_pState->CheckAttackHit(target);
    }

    return false;
}

// GetStateType(): m_pStateのnullガード
PlayerStateType Player::GetStateType() const
{
    if (m_pState == nullptr)
    {
        return PlayerStateType::IDLE; 
    }
    return m_pState->GetStateType();
}

