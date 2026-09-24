#pragma once
#include "DxLib.h"
#include <vector>
#include "../GameSetting/GameSetting.h"
#include "../Animation/AnimationController.h"
#include "../FPS/FPS.h"
#include "../Character/CharacterBase.h"

class CollisionAABB;
class CollisionOBB;
class StageObject;
class PlayerStateBase;
struct AttackData;
struct PlayerData;

class Player : public CharacterBase
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

    // State用
    VECTOR GetMove() const { return m_Move; }
    void SetMove(VECTOR move) { m_Move = move; }
    void SetTargetYaw(float yaw) { m_TargetYawValue = yaw; }
    bool IsGround() const { return m_IsGround; }
    void SetGround(bool value) { m_IsGround = value; }
    bool HasHovered() const { return m_HasHovered; }
    void SetHasHovered(bool value) { m_HasHovered = value; }

    // 攻撃State用
    void StartAttack() { m_IsAttack = true; }
    void EndAttack() { m_IsAttack = false; }
    void SetAttackData(const AttackData* data) { m_AttackData = data; }
    bool HasAirAttacked() const { return m_HasAirAttacked; }
    void SetHasAirAttacked(bool value) { m_HasAirAttacked = value; }

    // コヨーテタイム
    float GetCoyoteTime() const { return m_CoyoteTime; }

    // ジャンプバッファ
    void SetJumpBuffer() { m_JumpBufferTime = m_JumpBufferTimeMax; }
    bool IsJumpBuffered() const { return m_JumpBufferTime > 0.0f; }
    void ResetJumpBuffer() { m_JumpBufferTime = 0.0f; }
    void UpdateJumpBuffer()
    {
        if (m_JumpBufferTime > 0.0f)
        {
            m_JumpBufferTime -= FPSSystem::GetDeltaTime();
        }
    }

    // アニメーション
    float GetAnimationNowTime() const { return m_Animation.GetNowTime(); }
    float GetAnimationTotalTime() const { return m_Animation.GetTotalTime(); }

    void PlayAnimation(PlayerAnimationType anim, bool isLoop, float speed)
    {
        m_Animation.Play(anim, isLoop, speed);
    }

    // カメラ基準移動取得
    VECTOR GetCameraMoveInput();

    void CheckHitStageObjects(const std::vector<StageObject*>& objects);

private:
    void UpdateAttackOBB();

    // プレイヤーの状態を管理
    PlayerStateBase* m_pState;

    // プレイヤーのパラメータ
    const PlayerData* m_PlayerData;

    // モデルハンドル
    int m_Handle;

    VECTOR m_Scale;

    // 移動速度
    VECTOR m_Move;

    float m_TargetYawValue;

    // 衝突判定
    CollisionAABB* m_AABB;
    CollisionOBB* m_AttackOBB;

    // 判定
    bool m_IsGround;
    bool m_IsAttack;
    bool m_HasHovered;

    // コヨーテタイム
    float m_CoyoteTime;
    float m_CoyoteTimeMax;

    // ジャンプバッファ
    float m_JumpBufferTime;
    float m_JumpBufferTimeMax;

    // 攻撃
    bool m_HasAirAttacked;
    const AttackData* m_AttackData;

    float m_LastGroundY;

    // アニメーション
    AnimationController m_Animation;
};

