#pragma once
#include "CameraStrategy.h"

// 新しく作成されたFollowカメラの挙動
class FollowCameraStrategy : public CameraStrategy
{
public:
	FollowCameraStrategy() = default;
	virtual ~FollowCameraStrategy() = default;

	// カメラ挙動が切り替わった瞬間に1度だけ呼ばれる初期化関数
	void Start(Camera* camera) override;

	// 毎フレームのカメラ座標や注視点の計算処理
	void Update(Camera* camera) override;
private:
	// リセット回転用
	float m_TargetYaw = 0.0f;

	// 自動回転中か
	bool m_AutoRotate = false;
};