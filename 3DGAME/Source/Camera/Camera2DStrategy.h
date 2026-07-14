#pragma once
#include "CameraStrategy.h"

// 2D視点のカメラ挙動（2Dステージで使用する）
class Camera2DStrategy : public CameraStrategy
{
public:
	Camera2DStrategy() = default;
	virtual ~Camera2DStrategy() = default;

	// カメラ挙動が切り替わった瞬間に1度だけ呼ばれる初期化関数
	void Start(Camera* camera) override;

	// 毎フレームのカメラ座標や注視点の計算処理
	void Update(Camera* camera) override;
};