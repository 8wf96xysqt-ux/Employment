#pragma once
#include "DxLib.h"
#include <vector>
class CollisionAABB;
class CollisionSphere;
class CollisionOBB;
class StageObject;
enum PlayerAnimationType
{
	PLAYER_ANIMATION_DIE,
	PLAYER_ANIMATION_FALLING,
	PLAYER_ANIMATION_AIRCOMBO1,
	PLAYER_ANIMATION_AIRCOMBO2,
	PLAYER_ANIMATION_DASHCOMBO1,
	PLAYER_ANIMATION_DASHCOMBO2,
	PLAYER_ANIMATION_DASHJUMP,
	PLAYER_ANIMATION_GDCOMBO1,
	PLAYER_ANIMATION_GDCOMBO2,
	PLAYER_ANIMATION_IDLE,
	PLAYER_ANIMATION_JUMP,
	PLAYER_ANIMATION_RUN,
	PLAYER_ANIMATION_WALK,
};

enum class ComboType
{
	COMBO_NONE,
	COMBO_GROUND,
	COMBO_AIR,
	COMBO_DASH,
	
};

class Player
{
public:
	Player();
	~Player();
public:
	void Init();	// 初期化
	void Load();	// ロード
	void Start();	// 開始
	void Step();	// ステップ
	void Update();	// 更新
	void Draw();	// 描画
	void Fin();		// 終了
private:
	void UpdateAttackOBB();
public:
	VECTOR GetPos() { return m_Pos; }
	VECTOR GetRot() const { return m_Rot; }
	CollisionAABB* GetAABB() { return m_AABB; }
	CollisionSphere* GetSphereCollision() { return m_SphereCollision; }
	CollisionOBB* GetAttackOBB() { return m_AttackOBB; }
private:
	void PlayAnimation(PlayerAnimationType anim, bool isLoop);
	void UpdateAnimation();
	// アニメーションの種類ごとの再生速度を返す
	float GetAnimationSpeed(PlayerAnimationType anim) const;
public:
	void CheckHitStageObjects(const std::vector<StageObject*>objects);
private:
	int m_Handle;	// 画像ハンドル
	int m_AnimationAttachIndex;	// アニメーションのアタッチインデックス
	float m_AnimationTotalTime;	// 再生中のアニメーションの総時間
	float m_AnimationNowTime;	// 再生中のアニメーションの再生時間
	bool m_IsLoopAnimation;		// アニメーションをループさせるか
	float m_AnimationSpeed;     // 現在再生中のアニメーションの再生速度
	PlayerAnimationType m_NowAnimation;	// 再生中のアニメーション
	VECTOR m_Pos;	// 座標
	VECTOR m_Rot;	// 回転
	VECTOR m_Scale;	// スケール
	VECTOR m_Move;	// 移動量
	VECTOR m_PrevPos; // 前回の座標
	CollisionAABB* m_AABB;	// AABBの当たり判定
	CollisionOBB* m_AttackOBB; // OBBの攻撃当たり判定
	CollisionSphere* m_SphereCollision;
	float m_VelY;       // Y方向速度
	//判定
	bool  m_IsGround;   // 地面にいるか
	bool m_IsJumping; //ジャンプ中
	bool m_IsFalling; //落下中
	bool m_IsAttack; //攻撃中

	float m_CoyoteTime;          // コヨーテタイム残り時間
	float m_CoyoteTimeMax;       // 最大コヨーテタイム
	float m_JumpBufferTime;      // 入力バッファ残り時間
	float m_JumpBufferTimeMax;   // 最大入力バッファ時間
	// 可変ジャンプ用
	bool  m_JumpHold;        // ジャンプボタンを押しているか
	float m_JumpHoldTime;    // 押し続けている時間
	float m_JumpHoldMax;     // 最大ホールド時間
	//コンボ用
	int m_Combo;
	bool m_AttackBuffer;
	ComboType m_ComboType;
	//攻撃
	bool  m_IsAirAttackFreeze;      // 空中攻撃開始時の静止中フラグ
	float m_AirAttackFreezeTime;    // 静止残り時間
	float m_AirAttackFreezeTimeMax; // 静止する時間の最大値
	bool m_HasAirAttacked; // このジャンプ中にすでに空中攻撃したか

};