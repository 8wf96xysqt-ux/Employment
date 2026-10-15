#pragma once
#include "CameraStrategy.h"
class FollowCameraStrategy : public CameraStrategy
{
public:
	// カメラをこのストラテジーで使い始めるときの初期化
	void Start(Camera* camera) override;

	// 毎フレームのカメラ更新
	void Update(Camera* camera) override;

private:
	// Rボタンなどでプレイヤーの向きへカメラを自動回転させているか
	bool m_AutoRotate = false;
	// プレイヤーからカメラまでの距離
	float m_CameraDistance = -3.0f;
	// カメラが追従するY座標
	// ジャンプ中はこの値を固定する
	float m_CameraY = 0.0f;
	// Rボタンを押したときのカメラの目標Yaw
	float m_TargetYaw = 0.0f;
};