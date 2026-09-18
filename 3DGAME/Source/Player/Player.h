#pragma once
#include "DxLib.h"
#include <vector>
#include "../GameSetting/GameSetting.h"
#include "../Animation/AnimationController.h"
#include "../FPS/FPS.h"
#include "../Physics/Gravity.h"

class CollisionAABB;
class CollisionOBB;
class StageObject;
class PlayerStateBase;
struct AttackData;
struct PlayerData;

class Player
{
public:
    Player();
    ~Player();

    void Init();
    void Load();
    void Start();
    void Step();
    void Update();
    void Draw();
    void DrawDebug();
    void Fin();

    const char* GetStateTypeName() const;

    float RotCap(float rot);
    bool UpdateMove(float speed);
    void DirectionRot();
    float GetGroundY() const { return m_LastGroundY; }

    // 重力
    void StopGravity();
    void ResetGravity();

    VECTOR GetPos() const { return m_Pos; }
    VECTOR* GetPosPtr() { return &m_Pos; }
    VECTOR GetRot() const { return m_Rot; }
    CollisionAABB* GetAABB() { return m_AABB; }
    CollisionOBB* GetAttackOBB() { return m_AttackOBB; }

    // State用
    VECTOR GetMove() const { return m_Move; }
    void SetMove(VECTOR move) { m_Move = move; }
    float GetVelocityY() const { return m_Move.y; }
    void SetTargetYaw(float yaw) { m_TargetYawValue = yaw; }
    bool IsGround() const { return m_IsGround; }
    void SetGround(bool value) { m_IsGround = value; }
    bool IsAttack() const { return m_IsAttack; }
    bool HasHovered() const { return m_HasHovered; }
    void SetHasHovered(bool value) { m_HasHovered = value; }
    void SetParameter(const PlayerData* data){m_PlayerData = data;}

    // 無敵処理
    void StartInvincible(float time) { m_IsInvincible = true; m_InvincibleTime = time; }
    void EndInvincible() { m_IsInvincible = false; m_InvincibleTime = 0.0f; }
    bool IsInvincible() const { return m_IsInvincible; }

    // 攻撃State用
    void StartAttack() { m_IsAttack = true; }
    void EndAttack() { m_IsAttack = false; }
    void SetAttackData(const AttackData* data) { m_AttackData = data; }
    bool HasAirAttacked() const { return m_HasAirAttacked; }
    void SetHasAirAttacked(bool value) { m_HasAirAttacked = value; }

    // コヨーテタイム
    float GetCoyoteTime() const { return m_CoyoteTime; }
    void ResetCoyoteTime() { m_CoyoteTime = 0.0f; }

    // ジャンプバッファ
    void SetJumpBuffer() { m_JumpBufferTime = m_JumpBufferTimeMax; }
    bool IsJumpBuffered() const { return m_JumpBufferTime > 0.0f; }
    void ResetJumpBuffer() { m_JumpBufferTime = 0.0f; }
    void UpdateJumpBuffer() {if (m_JumpBufferTime > 0.0f){m_JumpBufferTime -= FPSSystem::GetDeltaTime();}}

    // アニメーション
    float GetAnimationNowTime() const { return m_Animation.GetNowTime(); }
    float GetAnimationTotalTime() const { return m_Animation.GetTotalTime(); }

    bool IsAirAttack() const { return m_IsAttack &&(m_Animation.GetCurrentAnimation() == PLAYER_ANIMATION_AIRCOMBO1 || m_Animation.GetCurrentAnimation() == PLAYER_ANIMATION_AIRCOMBO2); }

    void PlayAnimation(PlayerAnimationType anim, bool isLoop) { m_Animation.Play(anim, isLoop);}

    void PlayAnimation(PlayerAnimationType anim, bool isLoop, float speed){m_Animation.Play(anim, isLoop, speed);}

    // カメラ基準移動取得
    VECTOR GetCameraMoveInput();

    void CheckHitStageObjects(const std::vector<StageObject*>& objects);

private:
    void UpdateAttackOBB();
    void UpdateGravity();
    void UpdateInvincible();

    PlayerStateBase* m_pState;
    const PlayerData* m_PlayerData;
    int m_Handle;


    VECTOR m_Pos;
    VECTOR m_Rot;
    VECTOR m_Scale;
    VECTOR m_Move;
    VECTOR m_PrevPos;

    float m_TargetYawValue;

    CollisionAABB* m_AABB;
    CollisionOBB* m_AttackOBB;

    // 判定
    bool m_IsGround;
    bool m_IsAttack;
    bool m_IsAttackHit;
    bool m_HasHovered;

    float m_CoyoteTime;
    float m_CoyoteTimeMax;
    float m_JumpBufferTime;
    float m_JumpBufferTimeMax;

    // 攻撃
    bool m_HasAirAttacked;
    const AttackData* m_AttackData;

    // 無敵時間
    bool m_IsInvincible;
    float m_InvincibleTime;

    float m_LastGroundY;

    // 重力
    Gravity m_Gravity;

    // アニメーション
    AnimationController m_Animation;
};