#include "FollowCameraStrategy.h"
#include "Camera.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"

#define CAMERA_HEIGHT 3.0f
#define PLAYER_DISTANCE -3.0f
#define ROTATION_SPEED 0.05f

void FollowCameraStrategy::Start(Camera* camera)
{
	m_AutoRotate = false;
}

void FollowCameraStrategy::Update(Camera* camera)
{
	// 追従対象のプレイヤーを取得
	auto* targetPlayer = camera->GetTargetPlayer();
	if (!targetPlayer) return;

	VECTOR rot = camera->GetRotation();

	if (Input::IsTriggerKey(Input::KEY_PAD_R_PUSH))
	{
		m_TargetYaw = targetPlayer->GetRot().y;
		m_AutoRotate = true;
	}

	if (m_AutoRotate)
	{
		// 右スティック操作で解除
		if (fabsf(Input::GetStickRX()) > 0.0f ||
			fabsf(Input::GetStickRY()) > 0.0f)
		{
			m_AutoRotate = false;
		}
		else
		{
			float diff = m_TargetYaw - rot.y;


			if (diff > DX_PI_F)
				diff -= DX_PI_F * 2;

			if (diff < -DX_PI_F)
				diff += DX_PI_F * 2;


			rot.y += diff * 0.1f;


			if (fabs(diff) < 0.01f)
			{
				rot.y = m_TargetYaw;
				m_AutoRotate = false;
			}
		}
	}
	else
	{
		// 手動カメラ操作
		rot.y += Input::GetStickRX() * ROTATION_SPEED;
		rot.x += Input::GetStickRY() * ROTATION_SPEED;
	}



	// ピッチ角を上下の範囲内に収める
	rot.x = MyMath::Clamp(rot.x, -DX_PI_F * 0.3f, DX_PI_F * 0.2f);
	camera->SetRotation(rot);

	// プレイヤーの現在座標を取得する
	VECTOR playerPos = targetPlayer->GetPos();

	// ヨー回転・ピッチ回転の行列を作る
	MATRIX matYaw = MyMath::MatRotationYaw(rot.y);
	MATRIX matPitch = MyMath::MatRotationPitch(rot.x);

	// 2つの回転行列を合成する
	MATRIX matRot = MyMath::MatMult(matYaw, matPitch);

	// プレイヤーから見たカメラのオフセット(高さと距離)を作る
	VECTOR offset = VGet(0.0f, CAMERA_HEIGHT, PLAYER_DISTANCE);

	// オフセットを回転行列で変換し、回転後のオフセットにする
	offset = MyMath::MatTransform(matRot, offset);

	// 遮蔽を考慮しない、本来あるべきカメラ位置（理想位置）を計算してカメラにセット
	camera->SetIdealPos(MyMath::VecAdd(playerPos, offset));

	// 注視点(プレイヤーの少し上)を計算してカメラにセット
	camera->SetTarget(VAdd(playerPos, VGet(0.0f, 1.5f, 0.0f)));

	// 上方向ベクトルをY軸方向に設定してカメラにセット
	camera->SetUpVec(VGet(0, 1, 0));



}