#include "Camera2DStrategy.h"
#include "Camera.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"

// カメラが2Dに切り替わった瞬間に1度だけ呼ばれる初期化
void Camera2DStrategy::Start(Camera* camera)
{
	// 2Dカメラ中に右スティックで画面がグルグル回らないように回転角を固定
	// X軸（ピッチ）：少し見下ろす角度
	// Y軸（ヨー）：正面向き
	camera->SetRotation(VGet(-0.3f, 0.0f, 0.0f));
	camera->SetTargetYaw(0.0f);
}

// 毎フレームの座標計算
void Camera2DStrategy::Update(Camera* camera)
{
	// 追従対象のプレイヤーを取得
	auto* player = camera->GetTargetPlayer();
	if (!player) return;

	// プレイヤーの現在座標
	VECTOR playerPos = player->GetPos();
	// 現在のカメラの回転角度
	VECTOR cameraRot = camera->GetRotation();

	//回転行列を作成
	MATRIX matYaw = MyMath::MatRotationYaw(cameraRot.y);
	MATRIX matPitch = MyMath::MatRotationPitch(cameraRot.x);
	MATRIX matRot = MyMath::MatMult(matYaw, matPitch);

	//プレイヤーから見たカメラの配置オフセット
	//横スクロールとして見やすくなるよう、高さと引き距離を設定
	VECTOR offset = VGet(0.0f, 3.0f, -5.0f);

	// 設定したオフセットを回転行列で変換
	offset = MyMath::MatTransform(matRot, offset);

	//理想のカメラ位置（m_IdealPos）を計算
	VECTOR idealPos = MyMath::VecAdd(playerPos, offset);

	//注視点（m_Target）を計算（プレイヤーの足元ではなく、少し上にする）
	VECTOR targetPos = MyMath::VecAdd(playerPos, VGet(0.0f, 1.0f, 0.0f));

	//計算した結果をCameraクラスのメンバ変数に書き戻す
	camera->SetIdealPos(idealPos);
	camera->SetTarget(targetPos);
	camera->SetUpVec(VGet(0.0f, 1.0f, 0.0f)); // 上方向は常にY軸ポジティブ
}