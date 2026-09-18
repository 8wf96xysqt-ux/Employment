#include "Player.h"
#include "../StageObject/StageObjectManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionOBB.h"
#include "../StageObject/StageObject.h"
#include "../GameSetting/GameSetting.h"
#include "../State/PlayerStateIdle.h"
#include "../Camera/CameraManager.h"
#include "../GameSetting/Color.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../FPS/FPS.h"
#include <math.h>
#include "../ParameterData/AttackDataManager.h"
#include "../ParameterData/PlayerDataManager.h"

// コンストラクタ
Player::Player()
    : m_Handle(-1)
   
    , m_Pos(VGet(0.0f, 0.0f, 0.0f))
    , m_Rot(VGet(0.0f, 0.0f, 0.0f))
    , m_Scale(VGet(0.0f, 0.0f, 0.0f))
    , m_Move(VGet(0.0f, 0.0f, 0.0f))
    , m_PrevPos(VGet(0.0f, 0.0f, 0.0f))
    , m_PlayerData(nullptr)
    , m_AABB(nullptr)
    , m_AttackOBB(nullptr)

    , m_IsGround(true)
    , m_HasHovered(false)

    // コヨーテタイム & 入力バッファ
    , m_CoyoteTime(0.0f)
    , m_CoyoteTimeMax(0.0f)
    , m_JumpBufferTime(0.0f)
    , m_JumpBufferTimeMax(0.0f)

    , m_IsAttack(false)
    , m_IsAttackHit(false)
    // アタック
    , m_HasAirAttacked(false)
    , m_AttackData(nullptr)

    , m_TargetYawValue(0)

    , m_IsInvincible(false)
    , m_InvincibleTime(0.0f)

    , m_pState(nullptr)

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

    m_AABB->SetSize(VGet(m_PlayerData->aabbSizeX,m_PlayerData->aabbSizeY,m_PlayerData->aabbSizeZ));

    m_AttackOBB = CollisionManager::GetInstance()->CreateOBB();

    m_AttackOBB->SetTargetPos(&m_Pos);
    m_AttackOBB->SetLocalPos(VGet(0, 1.0f, 0));
    m_AttackOBB->SetSize(VGet(0.8f, 0.8f, 0.8f));
    m_AttackOBB->SetRotation(m_Rot.y);
    m_AttackOBB->SetActive(false);

    m_CoyoteTime = 0.0f;
    m_CoyoteTimeMax = m_PlayerData->coyoteTime;

    m_JumpBufferTime = 0.0f;
    m_JumpBufferTimeMax = m_PlayerData->jumpBufferTime;

    m_InvincibleTime = 0.0f;
}

// ロード
void Player::Load()
{
    m_Handle = MV1LoadModel("Data/Player/Test19.x");
}

// 開始
void Player::Start()
{
    m_Pos = VGet(0.0f, 1.0f, 0.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(m_PlayerData->scale,m_PlayerData->scale,m_PlayerData->scale);
    m_Move = VGet(0.0f, 0.0f, 0.0f);
    m_Animation.Init(m_Handle);
    m_Animation.Play(PLAYER_ANIMATION_IDLE,true,m_PlayerData->animSpeedIdle);
    m_pState = new PlayerStateIdle();
    m_pState->Enter(this);
}
void Player::Step()
{
    m_PrevPos = m_Pos;

    if (Input::IsTriggerJump())
    {
        SetJumpBuffer();
    }

    UpdateJumpBuffer();

    UpdateGravity();

    UpdateInvincible();
}
void Player::Update()
{
    if (m_pState == nullptr)
    {
        return;
    }


    PlayerStateBase* nextState = m_pState->GetNextState();


    // 次のStateが存在するときだけ切替
    if (nextState != nullptr && m_pState != nextState)
    {
        m_pState->Exit();

        delete m_pState;

        m_pState = nextState;

        m_pState->Enter(this);
    }


    m_pState->Update();


    MV1SetPosition(m_Handle, m_Pos);
    MV1SetRotationXYZ(m_Handle, m_Rot);
    MV1SetScale(m_Handle, m_Scale);
    m_Animation.Update();
    UpdateAttackOBB();
  
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
        GetVelocityY());

    DrawFormatString(0, 100, Color::White(),
        "HasHovered : %s",
        HasHovered() ? "TRUE" : "FALSE");

    DrawFormatString(0, 120, Color::White(),
        "HasAirAttacked : %s",
        HasAirAttacked() ? "TRUE" : "FALSE");

    DrawFormatString(0, 140, Color::White(),
        "CoyoteTime : %.3f",
        GetCoyoteTime());

    DrawFormatString(0, 220, Color::White(),
        "InvincibleTime : %.3f",
        m_InvincibleTime);

    DrawFormatString(0, 240, Color::White(),
        "DeltaTime : %.4f",
        FPSSystem::GetDeltaTime());

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

        delete m_pState;

        m_pState = nullptr; 
    }

    if (m_AABB)
    {
        CollisionManager::GetInstance()->DeleteAABB(m_AABB);
        m_AABB = nullptr;
    }

    if (m_AttackOBB)
    {
        CollisionManager::GetInstance()->DeleteOBB(m_AttackOBB);
        m_AttackOBB = nullptr;
    }

    if (m_Handle != -1)
    {
        MV1DeleteModel(m_Handle);
        m_Handle = -1;
    }
}

void Player::UpdateAttackOBB()
{
    if (!m_IsAttack)
    {
        m_AttackOBB->SetActive(false);
        return;
    }

    if (!m_AttackData)
    {
        m_AttackOBB->SetActive(false);
        return;
    }

    float nowTime = m_Animation.GetNowTime();

    // CSVの攻撃判定発生時間と持続時間を使用
    if (nowTime < m_AttackData->start || nowTime > m_AttackData->start + m_AttackData->duration)
    {
        m_AttackOBB->SetActive(false);
        return;
    }

    m_AttackOBB->SetActive(true);
    m_AttackOBB->SetRotation(m_Rot.y);

    // CSVの攻撃判定サイズを使用
    m_AttackOBB->SetSize(VGet(m_AttackData->hitboxSizeX, m_AttackData->hitboxSizeY, m_AttackData->hitboxSizeZ));

    // CSVのオフセットをプレイヤーの向きに合わせて回転
    VECTOR pos;

    pos.x = sinf(m_Rot.y) * m_AttackData->hitboxOffsetZ + cosf(m_Rot.y) * m_AttackData->hitboxOffsetX;

    pos.y = m_AttackData->hitboxOffsetY;

    pos.z = cosf(m_Rot.y) * m_AttackData->hitboxOffsetZ - sinf(m_Rot.y) * m_AttackData->hitboxOffsetX;

    m_AttackOBB->SetLocalPos(pos);
}

float Player::RotCap(float rot)
{
    while (rot < 0.0f)
    {
        rot += DX_TWO_PI_F;
    }

    while (rot >= DX_TWO_PI_F)
    {
        rot -= DX_TWO_PI_F;
    }

    return rot;
}


// 方向回転
void Player::DirectionRot()
{

    float rotationSpeed = m_PlayerData->rotationSpeed;

    m_TargetYawValue = RotCap(m_TargetYawValue);

    float difRot = m_TargetYawValue - m_Rot.y;

    float targetRotvalueMinRange = m_TargetYawValue - rotationSpeed;

    float targetRotvalueMaxRange = m_TargetYawValue + rotationSpeed;

    if (difRot > DX_PI_F || difRot < -DX_PI_F)
    {
        if (targetRotvalueMinRange > m_Rot.y)
        {
            m_Rot.y -= rotationSpeed;
        }
        else if (targetRotvalueMaxRange < m_Rot.y)
        {
            m_Rot.y += rotationSpeed;
        }
    }
    else if (targetRotvalueMinRange > m_Rot.y)
    {
        m_Rot.y += rotationSpeed;
    }
    else if (targetRotvalueMaxRange < m_Rot.y)
    {
        m_Rot.y -= rotationSpeed;
    }
    else
    {
        m_Rot.y = m_TargetYawValue;
    }

    m_Rot.y = RotCap(m_Rot.y);
}

void Player::UpdateGravity()
{
    m_Gravity.Update(m_Move.y);
}

void Player::StopGravity()
{
    m_Gravity.Stop();
}

void Player::ResetGravity()
{
    m_Gravity.Reset();
}

void Player::UpdateInvincible()
{
    if (!m_IsInvincible)
        return;

    m_InvincibleTime -= FPSSystem::GetDeltaTime();

    if (m_InvincibleTime <= 0.0f)
    {
        m_InvincibleTime = 0.0f;
        m_IsInvincible = false;
    }
}




// プレイヤーを移動させる
bool Player::UpdateMove(float speed)
{
    VECTOR move = GetCameraMoveInput();

    float inputLength = MyMath::VecLong(move);

    if (inputLength <= m_PlayerData -> moveInputThreshold)
    {
        m_Move.x = 0.0f;
        m_Move.z = 0.0f;
        return false;
    }

    move = MyMath::VecNormalize(move);

    float moveSpeed = speed * inputLength;

    m_Move.x = move.x * moveSpeed;
    m_Move.z = move.z * moveSpeed;

    m_TargetYawValue = atan2f(move.x, move.z);
    DirectionRot();

    return true;
}
// カメラ基準の移動方向を取得する
VECTOR Player::GetCameraMoveInput()
{
    float camYaw =CameraManager::GetInstance()->GetCamera()->GetYaw();

    VECTOR camForward = MyMath::VecForwardZX(camYaw);

    VECTOR camRight;

    camRight.x = camForward.z;
    camRight.y = 0.0f;
    camRight.z = -camForward.x;



    VECTOR move = VGet(0, 0, 0);

    VECTOR input = Input::GetMove();

    move = MyMath::VecAdd(move,MyMath::VecScale(camRight, input.x));

    move = MyMath::VecAdd(move,MyMath::VecScale(camForward, input.z));

    return move;
}

//ステージ当たり判定
void Player::CheckHitStageObjects(const std::vector<StageObject*>& objects)
{
    // X
    m_Pos.x += m_Move.x * TARGET_FPS * FPSSystem::GetDeltaTime();
    for (auto obj : objects)
    {
        const CollisionAABB* objAABB = obj->GetAABB();
        if (!objAABB) continue;

        if (m_AABB->CheckAABB(objAABB))
        {
            m_Pos.x = m_PrevPos.x;
        }
    }

    bool wasGround = m_IsGround;

    bool hitGround = false;

    float moveY = m_Move.y * TARGET_FPS * FPSSystem::GetDeltaTime();

    m_Pos.y += moveY;

    for (auto obj : objects)
    {
        const CollisionAABB* objAABB = obj->GetAABB();
        if (!objAABB) continue;

        if (m_AABB->CheckAABB(objAABB))
        {
            m_Pos.y = m_PrevPos.y;
            m_Move.y = 0.0f;

            // 下方向に移動していた場合だけ着地
            if (moveY < 0.0f)
            {
                hitGround = true;
            }
        }
    }

    if (hitGround)
    {
        m_IsGround = true;

        // 着地したのでコヨーテタイム終了
        m_CoyoteTime = 0.0f;

        m_HasAirAttacked = false;
        m_LastGroundY = m_Pos.y;
    }
    else
    {
        m_IsGround = false;

        // 地面から離れた瞬間だけコヨーテタイム開始
        if (wasGround)
        {
            m_CoyoteTime = m_CoyoteTimeMax;
        }
        else if (m_CoyoteTime > 0.0f)
        {
            m_CoyoteTime -= FPSSystem::GetDeltaTime();
        }
    }
    // Z
    m_Pos.z += m_Move.z * TARGET_FPS * FPSSystem::GetDeltaTime();
    for (auto obj : objects)
    {
        const CollisionAABB* objAABB = obj->GetAABB();
        if (!objAABB) continue;

        if (m_AABB->CheckAABB(objAABB))
        {
            m_Pos.z = m_PrevPos.z;
        }
    }

}