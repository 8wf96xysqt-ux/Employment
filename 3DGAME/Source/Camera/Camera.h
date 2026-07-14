#pragma once
#include "DxLib.h"
#include <vector>
#include <memory> 

class Player;
class CameraStrategy; 

// カメラクラス
class Camera
{
public:
	Camera();
	~Camera();
public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();
	float GetYaw() const { return m_Rot.y; }

public: 
	// カメラの挙動（戦略）を動的に切り替える
	void SetStrategy(std::unique_ptr<CameraStrategy> strategy);

	// 戦略クラスからCameraの内部変数を操作・参照するためのゲッター・セッター
	VECTOR GetPosition() const { return m_Pos; }
	void SetPosition(const VECTOR& pos) { m_Pos = pos; }

	VECTOR GetRotation() const { return m_Rot; }
	void SetRotation(const VECTOR& rot) { m_Rot = rot; }

	VECTOR GetTarget() const { return m_Target; }
	void SetTarget(const VECTOR& target) { m_Target = target; }

	VECTOR GetUpVec() const { return m_UpVec; }
	void SetUpVec(const VECTOR& upVec) { m_UpVec = upVec; }

	VECTOR GetIdealPos() const { return m_IdealPos; }
	void SetIdealPos(const VECTOR& idealPos) { m_IdealPos = idealPos; }

	Player* GetTargetPlayer() const { return m_TargetPlayer; }
	float GetTargetYaw() const { return m_TargetYaw; }
	void SetTargetYaw(float yaw) { m_TargetYaw = yaw; }

private:
	float UpdateOcclusion();
	VECTOR CalcPulledCameraPos(const VECTOR& target, const VECTOR& idealPos, float nearestT) const;

private:
	std::unique_ptr<CameraStrategy> m_Strategy; // 現在のカメラ戦略を保持

	VECTOR m_Pos;
	VECTOR m_Rot;
	VECTOR m_Target;
	VECTOR m_UpVec;
	VECTOR m_IdealPos;
	Player* m_TargetPlayer;
	int m_DebugHitCount = 0;
	float m_TargetYaw;
	bool m_AutoRotate = false;
};