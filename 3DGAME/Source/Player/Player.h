#pragma once
#include "DxLib.h"
#include <vector>
#include "../GameSetting/GameSetting.h"
#include "../Animation/AnimationController.h"
#include "../FPS/FPS.h"
#include "../Character/CharacterBase.h"
#include "../State/PlayerStateBase.h"
#include "../Attack/AttackComponent.h"
#include "../Animation/PlayerAnimParam.h"

class CollisionAABB;
class StageObject;
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
    
    bool UpdateMove(float speed);
    void DirectionRot();
    float GetGroundY() const { return m_LastGroundY; }

    // State用
    void SetTargetYaw(float yaw) { m_TargetYawValue = yaw; }
    bool HasHovered() const { return m_HasHovered; }
    void SetHasHovered(bool value) { m_HasHovered = value; }
    PlayerStateType GetStateType() const;

    // 攻撃Component取得
    AttackComponent* GetAttackComponent()
    {
        return &m_AttackComponent;
    }

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
    float GetAnimationNowTime() const 
    { 
        return m_Animation.GetNowTime(); 
    }
    float GetAnimationTotalTime() const 
    { 
        return m_Animation.GetTotalTime(); 
    }
    void PlayAnimation(PlayerAnimationType anim, bool isLoop, float speed)
    {
        m_Animation.Play(anim, isLoop, speed);
    }

    // カメラ基準移動取得
    VECTOR GetCameraMoveInput();

    // ダメージ処理
    void TakeDamage(int damage) override;
    // 即死処理
    void Kill();

    // リスポーン地点
    void SetRespawnPoint(VECTOR pos, VECTOR rot);
    VECTOR GetRespawnPoint() const;
    void Respawn();

    void CheckHitStageObjects(const std::vector<StageObject*>& objects);
    bool CheckAttackHit(CollisionAABB* target);
private:
    // プレイヤーの状態を管理
    std::unique_ptr<PlayerStateBase> m_pState;
    // プレイヤーのパラメータ
    const PlayerData* m_PlayerData;
    float m_TargetYawValue;
    // 攻撃
    AttackComponent m_AttackComponent;
    // 判定
    bool m_HasHovered;
    // コヨーテタイム
    float m_CoyoteTime;
    float m_CoyoteTimeMax;
    // ジャンプバッファ
    float m_JumpBufferTime;
    float m_JumpBufferTimeMax;
    // リスポーン地点
    VECTOR m_RespawnPos;
    VECTOR m_RespawnRot;

    float m_LastGroundY;
};