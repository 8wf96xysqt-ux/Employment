#pragma once
#include "CameraStrategy.h"

// 前方宣言（StageObjectの実際の定義はcpp側でインクルードします）
class StageObject;

// 特定のターゲット（敵など）を常に画面内に捉えるカメラ挙動
class LockOnStrategy : public CameraStrategy
{
public:
	// コンストラクタでロックオン対象のオブジェクトを受け取る
	LockOnStrategy(StageObject* target);
	virtual ~LockOnStrategy() = default;

	// カメラ挙動が切り替わった瞬間に1度だけ呼ばれる初期化関数
	void Start(Camera* camera) override;

	// 毎フレームのカメラ座標や注視点の計算処理
	void Update(Camera* camera) override;

private:
	// ロックオン対象のオブジェクトへのポインタ
	StageObject* m_LockOnTarget = nullptr;
};