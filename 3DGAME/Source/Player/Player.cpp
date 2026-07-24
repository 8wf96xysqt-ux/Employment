#include "Player.h"
#include "../Input//Input.h"
#include "../MyMath/MyMath.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionSphere.h"
#include "../Collision/CollisionOBB.h"
#include <math.h>
#include "../Camera/CameraManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/StageObject.h"
#include "../State/PlayerStateIdle.h"

#define ROTATION_SPEED  0.1f
#define PLAYER_ROTATION_SPEED 0.25f
#define MOVE_SPEED      0.08f
#define GRAVITY         0.01f
#define JUMP_POW        0.12f
#define ROLL_SPEED 0.07f
#define ROLL_TIME 0.7f
#define MAX_FALL_SPEED 0.4
// アニメーション種類ごとの再生速度
#define ANIM_SPEED_LANDING     1.0f
#define ANIM_SPEED_DIE         1.0f
#define ANIM_SPEED_FALLING     1.0f
#define ANIM_SPEED_AIRCOMBO1   1.5f
#define ANIM_SPEED_AIRCOMBO2   1.5f
#define ANIM_SPEED_DASHCOMBO1  1.5f
#define ANIM_SPEED_DASHCOMBO2  1.5f
#define ANIM_SPEED_DASHJUMP    1.0f
#define ANIM_SPEED_GDCOMBO1    1.2f
#define ANIM_SPEED_GDCOMBO2    1.2f
#define ANIM_SPEED_IDLE        1.0f
#define ANIM_SPEED_JUMP        1.0f
#define ANIM_SPEED_RUN         0.6f
#define ANIM_SPEED_WALK        0.6f
#define ANIM_SPEED_ROLLING     1.5f
//アタック
#define AIR_ATTACK_FREEZE_TIME 0.05f  // 空中攻撃開始時に静止する時間(秒)


// コンストラクタ
Player::Player()
{
    m_Handle = 0;
    m_AnimationAttachIndex = 0;
    m_AnimationTotalTime = 0.0f;
    m_AnimationNowTime = 0.0f;
    m_IsLoopAnimation = false;
    m_AnimationSpeed = 0.0f;
    m_NowAnimation = PLAYER_ANIMATION_IDLE;

    m_Pos = VGet(0.0f, 0.0f, 0.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(0.0f, 0.0f, 0.0f);
    m_Move = VGet(0.0, 0.0f, 0.0f);
    m_PrevPos = VGet(0.0, 0.0f, 0.0f);

    m_AABB = nullptr;
    m_AttackOBB = nullptr;
    m_SphereCollision = nullptr;

    m_VelY = 0.0f;
    m_IsGround = true;
    m_IsJumping = false;
    m_IsFalling = false;
    //コヨーテタイム & 入力バッファ
    m_CoyoteTime = 0.0f;
    m_CoyoteTimeMax = 0.0f;
    m_JumpBufferTime = 0.0f;
    m_JumpBufferTimeMax = 0.0f;

    m_JumpHold = false;
    m_JumpHoldTime = 0.0f;
    m_JumpHoldMax = 0.0f;   // 長押しで 0.2秒まで上昇を追加

    m_IsAttack = false;
    m_IsAttackHit = false;
    //コンボ
    m_Combo = 0;
    m_AttackBuffer = false;
    m_ComboType = ComboType::COMBO_NONE;
    //アタック
    m_IsAirAttackFreeze = false;
    m_AirAttackFreezeTime = 0.0f;
    m_AirAttackFreezeTimeMax = AIR_ATTACK_FREEZE_TIME;
    m_HasAirAttacked = false;

    // ローリング
    m_IsRolling = false;
    m_RollTime = 0.0f;
    m_RollTimeMax = 0.0f;
    m_RollDir = VGet(0, 0, 0);

    m_PiScale = 0;
    m_TargetYawValue = 0;

    m_FallAnimDelay = 0.0f;
    m_FallAnimDelayMax = 0.0f;
    m_IsInvincible = false;
    m_InvincibleTime = 0.0f;

    m_pState = nullptr;


}

// デストラクタ
Player::~Player()
{
    Fin();
}

// 初期化
void Player::Init()
{
    m_AABB = CollisionManager::GetInstance()->CreateAABB();
    m_AABB->SetTargetPos(&m_Pos);
    m_AABB->SetLocalPos(VGet(0, 0.6f, 0));
    m_AABB->SetSize(VGet(0.7f, 1.3f, 0.7f));

    m_AttackOBB = CollisionManager::GetInstance()->CreateOBB();

    m_AttackOBB->SetTargetPos(&m_Pos);
    m_AttackOBB->SetLocalPos(VGet(0.5f, 1.2f, 0));
    m_AttackOBB->SetSize(VGet(0.8f, 0.8f, 0.8f));
    m_AttackOBB->SetRotation(m_Rot.y);

    //初期化
    m_CoyoteTime = 0.0f;
    m_CoyoteTimeMax = 0.1f;
    m_JumpBufferTime = 0.0f;
    m_JumpBufferTimeMax = 0.1f;

    m_RollTimeMax = ROLL_TIME;

    m_FallAnimDelay = 0.0f;
    m_FallAnimDelayMax = 0.6f;   
    //無敵時間
    m_InvincibleTime = 0.0f;


}

// ロード
void Player::Load()
{
    m_Handle = MV1LoadModel("Data/Player/Bot.x");
}

// 開始
void Player::Start()
{
    m_Pos = VGet(0.0f, 2.5f, 0.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(1.0f, 1.0f, 1.0f);

    m_Move = VGet(0.0, 0.0f, 0.0f);

    m_NowAnimation = (PlayerAnimationType)-1;
    PlayAnimation(PLAYER_ANIMATION_IDLE, true);
}

void Player::Step()
{
    m_PrevPos = m_Pos;

    UpdateGravity();

    UpdateJump();

    UpdateRolling();

    UpdateAttack();

    UpdateMove();

    UpdateInvincible();

}
void Player::Update()
{
    /*
    // 次のStateを取得
    PlayerStateBase* nextState = m_pState->GetNextState();

    // 次のStateがある場合は切り替え
    if (m_pState != nextState)
    {
        // 現在のStateを終了
        m_pState->Exit();
        // 次のStateに切り替え
        delete m_pState;
        m_pState = nextState;
        // 新しいStateの初期化
        m_pState->Enter(this);
    }

    // 現在のStateの更新処理を実行
    m_pState->Update();
    */

    MV1SetPosition(m_Handle, m_Pos);
    MV1SetRotationXYZ(m_Handle, m_Rot);
    MV1SetScale(m_Handle, m_Scale);
    UpdateAttackOBB();
    UpdateAnimation();
}

// 描画
void Player::Draw()
{
    MV1DrawModel(m_Handle);
    DrawFormatString(0, 0, GetColor(255, 255, 255), "座標[%f, %f, %f]", m_Pos.x, m_Pos.y, m_Pos.z);
    DrawFormatString(0, 20, GetColor(255, 255, 255), "回転[%f, %f, %f]", m_Rot.x, m_Rot.y, m_Rot.z);

   /* int num = MV1GetAnimNum(m_Handle);

    for (int i = 0; i < num; i++)
    {
        DrawFormatString(
            500,
            i * 50,
            GetColor(255, 255, 255),
            "%d : %s",
            i,
            MV1GetAnimName(m_Handle, i)
        );
    }*/


}

// 終了
void Player::Fin()
{
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

    MV1DeleteModel(m_Handle);
}

void Player::UpdateAttackOBB()
{
    if (m_IsAttack &&
        m_IsAttackHit)
    {
        m_AttackOBB->SetRotation(m_Rot.y);


        VECTOR pos;

        float distance = 0.7f;

        pos.x = sinf(m_Rot.y) * distance;
        pos.y = 0.5f;
        pos.z = cosf(m_Rot.y);


        m_AttackOBB->SetLocalPos(pos);
    }
    else
    {
        m_AttackOBB->SetLocalPos(
            VGet(0, -100, 0)
        );
    }
}

float Player::RotCap(float rot)
{
    if (rot < 0.0f)
    {
        rot += DX_TWO_PI_F;
    }
    else if (rot > DX_TWO_PI_F)
    {
        rot -= DX_TWO_PI_F;
    }

    return rot;
}

// 方向回転
void Player::DirectionRot()
{
    m_TargetYawValue = RotCap(m_TargetYawValue);

    float difRot = 0.0f;

    difRot = m_TargetYawValue - m_Rot.y;

    float targetRotvalueMinRange = m_TargetYawValue - PLAYER_ROTATION_SPEED;
    float targetRotvalueMaxRange = m_TargetYawValue + PLAYER_ROTATION_SPEED;

    if (difRot > DX_PI_F || difRot < -DX_PI_F)
    {
        if (targetRotvalueMinRange > m_Rot.y)
        {
            m_Rot.y -= PLAYER_ROTATION_SPEED;
        }
        else if (targetRotvalueMaxRange < m_Rot.y)
        {
            m_Rot.y += PLAYER_ROTATION_SPEED;
        }
    }

    else if (targetRotvalueMinRange > m_Rot.y)
    {
        m_Rot.y += PLAYER_ROTATION_SPEED;
    }
    else if (targetRotvalueMaxRange < m_Rot.y)
    {
        m_Rot.y -= PLAYER_ROTATION_SPEED;
    }
    else
    {
        m_Rot.y = m_TargetYawValue;
    }

    m_Rot.y = RotCap(m_Rot.y);
}

void Player::UpdateMove()
{
    if (m_IsRolling)
    {
        return;
    }
    // カメラの向き取得
    float camYaw =
        CameraManager::GetInstance()
        ->GetCamera()
        ->GetYaw();


    // カメラ基準の前方向
    VECTOR camForward =
        MyMath::VecForwardZX(camYaw);


    // カメラ基準の右方向
    VECTOR camRight;

    camRight.x = camForward.z;
    camRight.y = 0.0f;
    camRight.z = -camForward.x;


    VECTOR move = VGet(0, 0, 0);


    // 入力取得
    float lx = Input::GetStickLX();
    float ly = -Input::GetStickLY();


    // キーボード入力
    if (Input::IsInputKey(Input::KEY_A))
        lx -= 1.0f;

    if (Input::IsInputKey(Input::KEY_D))
        lx += 1.0f;

    if (Input::IsInputKey(Input::KEY_W))
        ly += 1.0f;

    if (Input::IsInputKey(Input::KEY_S))
        ly -= 1.0f;


    // カメラ方向へ変換
    move = MyMath::VecAdd(
        move,
        MyMath::VecScale(camRight, lx)
    );

    move = MyMath::VecAdd(
        move,
        MyMath::VecScale(camForward, ly)
    );


    float length = MyMath::VecLong(move);


    // 攻撃キャンセル判定
    const float ATTACK_CANCEL_THRESHOLD = 0.3f;

    if (m_IsAttack &&
        m_IsGround &&
        m_ComboType == ComboType::COMBO_GROUND &&
        length > ATTACK_CANCEL_THRESHOLD)
    {
        m_IsAttack = false;
        m_AttackBuffer = false;
        m_Combo = 0;
        m_ComboType = ComboType::COMBO_NONE;
    }



    // 空中攻撃中は移動禁止
    bool isAirAttacking =
        m_IsAttack &&
        (m_NowAnimation == PLAYER_ANIMATION_AIRCOMBO1 ||
            m_NowAnimation == PLAYER_ANIMATION_AIRCOMBO2);


    if (isAirAttacking)
    {
        m_Move.x = 0.0f;
        m_Move.z = 0.0f;
        return;
    }


    // 移動入力あり
    if (length > 0.02f)
    {
        move = MyMath::VecNormalize(move);


        float power = sqrtf(lx * lx + ly * ly);

        if (power > 1.0f)
        {
            power = 1.0f;
        }


        m_Move.x = move.x * MOVE_SPEED * power;
        m_Move.z = move.z * MOVE_SPEED * power;


        // 向き変更
        m_TargetYawValue = atan2f(move.x, move.z);

        DirectionRot();



        // 地上アニメーション
        if (m_IsGround && !m_IsAttack)
        {
            if (power < 0.6f)
            {
                PlayAnimation(
                    PLAYER_ANIMATION_WALK,
                    true
                );
            }
            else
            {
                PlayAnimation(
                    PLAYER_ANIMATION_RUN,
                    true
                );
            }
        }
    }
    else
    {
        // 入力なし
        if (!m_IsAttack)
        {
            m_Move.x = 0.0f;
            m_Move.z = 0.0f;


            if (m_IsGround)
            {
                PlayAnimation(
                    PLAYER_ANIMATION_IDLE,
                    true
                );
            }
        }
    }
}

void Player::UpdateJump()
{

    // ローリング中はジャンプ不可
    if (m_IsRolling)
    {
        return;
    }

    if (Input::IsTriggerJump())
    {
        m_JumpBufferTime = m_JumpBufferTimeMax;
    }


    if (m_JumpBufferTime > 0)
    {
        m_JumpBufferTime -= 1.0f / 60.0f;
    }


    if (m_CoyoteTime > 0 &&
        m_JumpBufferTime > 0)
    {
        m_Move.y = JUMP_POW;

        m_IsGround = false;

        m_JumpBufferTime = 0;

        m_JumpHold = true;
        m_JumpHoldTime = 0;

        m_IsAttack = false;

        PlayAnimation(
            PLAYER_ANIMATION_DASHJUMP,
            true
        );
    }


    // 可変ジャンプ
    if (!m_IsGround)
    {
        if (Input::IsJump())
        {
            m_JumpHoldMax = 0.1;

            if (m_JumpHold &&
                m_JumpHoldTime < m_JumpHoldMax)
            {
                m_Move.y += 0.006f;

                m_JumpHoldTime += 1.0f / 60.0f;

                m_IsJumping = true;
            }
        }
        else
        {
            if (m_Move.y > 0)
            {
                m_Move.y *= 0.5f;
            }

            m_JumpHold = false;
        }
    }
}
void Player::UpdateGravity()
{
    bool isAirAttack =
        m_IsAttack &&
        (m_NowAnimation == PLAYER_ANIMATION_AIRCOMBO1 ||
            m_NowAnimation == PLAYER_ANIMATION_AIRCOMBO2);

    if (isAirAttack)
    {
        m_Move.y = 0.0f;
    }
    else
    {
        m_Move.y -= GRAVITY;

        // 落下速度制限
        if (m_Move.y < -MAX_FALL_SPEED)
        {
            m_Move.y = -MAX_FALL_SPEED;
        }
    }
}
void Player::UpdateAttack()
{
    // 攻撃入力
    if (Input::IsAttack())
    {
        // 攻撃していない時だけ開始
        if (!m_IsAttack)
        {
            m_IsAttack = true;

            m_Combo = 1;


            // 空中攻撃
            if (!m_IsGround && !m_HasAirAttacked)
            {
                m_ComboType = ComboType::COMBO_AIR;

                PlayAnimation(
                    PLAYER_ANIMATION_AIRCOMBO1,
                    false
                );


                // 空中攻撃開始時の停止
                m_IsAirAttackFreeze = true;

                m_AirAttackFreezeTime =
                    m_AirAttackFreezeTimeMax;


                m_Move = VGet(
                    0.0f,
                    0.0f,
                    0.0f
                );


                m_HasAirAttacked = true;
            }


            // ダッシュ攻撃
            else if (m_Move.x != 0.0f ||
                m_Move.z != 0.0f)
            {
                m_ComboType = ComboType::COMBO_DASH;

                PlayAnimation(
                    PLAYER_ANIMATION_DASHCOMBO1,
                    false
                );
            }


            // 通常攻撃
            else
            {
                m_ComboType = ComboType::COMBO_GROUND;

                PlayAnimation(
                    PLAYER_ANIMATION_GDCOMBO1,
                    false
                );
            }
        }
        else
        {
            // 攻撃中ならコンボ受付
            m_AttackBuffer = true;
        }
    }
}

void Player::UpdateRolling()
{


    // ローリング中
    if (m_IsRolling)
    {
        m_RollTime -= 1.0f / 60.0f;


        m_Move.x = m_RollDir.x * ROLL_SPEED;
        m_Move.z = m_RollDir.z * ROLL_SPEED;


        if (m_RollTime <= 0)
        {
            m_IsRolling = false;

            m_Move.x = 0;
            m_Move.z = 0;

            m_IsInvincible = false;
            m_InvincibleTime = 0.0f;


            PlayAnimation(
                PLAYER_ANIMATION_IDLE,
                true
            );
        }

        return;
    }


    // ローリング開始
    if (Input::IsTriggerKey(Input::KEY_PAD_B))
    {
        // 空中では不可
        if (!m_IsGround)
        {
            return;
        }


        // 攻撃キャンセル
        m_IsAttack = false;
        m_AttackBuffer = false;
        m_Combo = 0;
        m_ComboType = ComboType::COMBO_NONE;


        m_IsRolling = true;

        m_RollTime = m_RollTimeMax;

        // ローリング無敵開始
        m_IsInvincible = true;
        m_InvincibleTime = m_RollTimeMax;

        // 向いている方向へ回避
        m_RollDir.x = sinf(m_Rot.y);
        m_RollDir.y = 0;
        m_RollDir.z = cosf(m_Rot.y);


        PlayAnimation(
            PLAYER_ANIMATION_ROLLING,
            false
        );
    }
}

void Player::UpdateInvincible()
{
    if (!m_IsInvincible)
        return;

    m_InvincibleTime -= 1.0f / 60.0f; //60FPS想定

    if (m_InvincibleTime <= 0.0f)
    {
        m_InvincibleTime = 0.0f;
        m_IsInvincible = false;
    }
}




// アニメーションの種類ごとの再生速度を返す
float Player::GetAnimationSpeed(PlayerAnimationType anim) const
{
    switch (anim)
    {
    case PLAYER_ANIMATION_LANDING:
        return ANIM_SPEED_LANDING;

    case PLAYER_ANIMATION_DIE:
        return ANIM_SPEED_DIE;

    case PLAYER_ANIMATION_FALLING:
        return ANIM_SPEED_FALLING;

    case PLAYER_ANIMATION_AIRCOMBO1:
        return ANIM_SPEED_AIRCOMBO1;

    case PLAYER_ANIMATION_AIRCOMBO2:
        return ANIM_SPEED_AIRCOMBO2;

    case PLAYER_ANIMATION_DASHCOMBO1:
        return ANIM_SPEED_DASHCOMBO1;

    case PLAYER_ANIMATION_DASHCOMBO2:
        return ANIM_SPEED_DASHCOMBO2;

    case PLAYER_ANIMATION_DASHJUMP:
        return ANIM_SPEED_DASHJUMP;

    case PLAYER_ANIMATION_GDCOMBO1:
        return ANIM_SPEED_GDCOMBO1;

    case PLAYER_ANIMATION_GDCOMBO2:
        return ANIM_SPEED_GDCOMBO2;

    case PLAYER_ANIMATION_IDLE:
        return ANIM_SPEED_IDLE;

    case PLAYER_ANIMATION_JUMP:
        return ANIM_SPEED_JUMP;

    case PLAYER_ANIMATION_RUN:
        return ANIM_SPEED_RUN;

    case PLAYER_ANIMATION_WALK:
        return ANIM_SPEED_WALK;


    case PLAYER_ANIMATION_ROLLING:
        return ANIM_SPEED_ROLLING;

    default:
        return 1.0f;
    }
}

// アニメーション再生
void Player::PlayAnimation(PlayerAnimationType anim, bool isLoop)
{
    if (anim == m_NowAnimation) return;

    MV1DetachAnim(m_Handle, m_AnimationAttachIndex);
    m_AnimationAttachIndex = MV1AttachAnim(m_Handle, anim);

    m_AnimationTotalTime = MV1GetAttachAnimTotalTime(m_Handle, m_AnimationAttachIndex);
    m_AnimationNowTime = 0;

    m_IsLoopAnimation = isLoop;
    m_NowAnimation = anim;

    // 再生するアニメーションの種類に応じた速度を設定する
    m_AnimationSpeed = GetAnimationSpeed(anim);
}

// アニメーション更新
void Player::UpdateAnimation()
{
    MV1SetAttachAnimTime(m_Handle, m_AnimationAttachIndex, m_AnimationNowTime);
    m_AnimationNowTime += m_AnimationSpeed;



    // コンボ受付
    float rate = m_AnimationNowTime / m_AnimationTotalTime;


    if (m_IsAttack)
    {
        // 攻撃が当たる時間
        if (rate > 0.3f &&
            rate < 0.6f)
        {
            m_IsAttackHit = true;
        }
        else
        {
            m_IsAttackHit = false;
        }
    }

    if (!m_IsGround)
    {
        m_FallAnimDelay -= 1.0f / 60.0f;

        if (m_FallAnimDelay <= 0.0f)
        {
            // 空中攻撃中・ローリング中以外
            if (!m_IsAttack &&
                !m_IsRolling &&
                m_NowAnimation != PLAYER_ANIMATION_FALLING)
            {
                PlayAnimation(
                    PLAYER_ANIMATION_FALLING,
                    true
                );
            }
        }
    }
    else
    {
        m_FallAnimDelay = m_FallAnimDelayMax;
    }

    // 1段目→2段目
    if (m_Combo == 1 && rate >= 0.6f && m_AttackBuffer)
    {
        m_AttackBuffer = false;
        m_Combo = 2;

        if (m_ComboType == ComboType::COMBO_GROUND)
            PlayAnimation(PLAYER_ANIMATION_GDCOMBO2, false);
        else if (m_ComboType == ComboType::COMBO_DASH)
            PlayAnimation(PLAYER_ANIMATION_DASHCOMBO2, false);
        else if (m_ComboType == ComboType::COMBO_AIR)
            PlayAnimation(PLAYER_ANIMATION_AIRCOMBO2, false);

        return;
    }

    // 2段目→3段目
    if (m_Combo == 2 && rate >= 0.6f && m_AttackBuffer)
    {
        m_AttackBuffer = false;
        m_Combo = 3;

        if (m_ComboType == ComboType::COMBO_GROUND)
            PlayAnimation(PLAYER_ANIMATION_GDCOMBO1, false);
        else if (m_ComboType == ComboType::COMBO_DASH)
            PlayAnimation(PLAYER_ANIMATION_DASHCOMBO1, false);
        else if (m_ComboType == ComboType::COMBO_AIR)
            PlayAnimation(PLAYER_ANIMATION_AIRCOMBO1, false);

        return;
    }

    if (m_AnimationNowTime > m_AnimationTotalTime)
    {
        m_AnimationNowTime = m_IsLoopAnimation ? 0.0f : m_AnimationTotalTime;
    }

    if (!m_IsLoopAnimation &&m_AnimationNowTime >= m_AnimationTotalTime)
    {
        bool wasAirAttack = (m_NowAnimation == PLAYER_ANIMATION_AIRCOMBO1 || m_NowAnimation == PLAYER_ANIMATION_AIRCOMBO2);

        m_IsAttack = false;
        m_AttackBuffer = false;
        m_Combo = 0;
        m_ComboType = ComboType::COMBO_NONE;

        // 空中攻撃が終わってもまだ着地していなければ落下アニメーションへ
        if (wasAirAttack && !m_IsGround)
        {
            PlayAnimation(PLAYER_ANIMATION_DASHJUMP, true);
        }
    }


}

//ステージ当たり判定
void Player::CheckHitStageObjects(const std::vector<StageObject*> objects)
{
    // X
    m_Pos.x += m_Move.x;
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
    // Y
    m_Pos.y += m_Move.y;

    for (auto obj : objects)
    {
        const CollisionAABB* objAABB = obj->GetAABB();
        if (!objAABB) continue;

        if (m_AABB->CheckAABB(objAABB))
        {
            m_Pos.y = m_PrevPos.y;
            m_Move.y = 0.0f;

            hitGround = true;
           
        }
    }

    if (hitGround)
    {
        m_IsGround = true;
        m_IsJumping = false;
        m_CoyoteTime = m_CoyoteTimeMax;
        m_HasAirAttacked = false;

        // 落下タイマーリセット
        m_FallAnimDelay = m_FallAnimDelayMax;
    }
    else
    {
        if (m_IsGround)
        {
            m_IsGround = false;
        }

        if (m_CoyoteTime > 0.0f)
        {
            m_CoyoteTime -= 1.0f / 60.0f;
        }
    }
    // Z
    m_Pos.z += m_Move.z;
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