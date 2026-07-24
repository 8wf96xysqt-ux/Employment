#include "FollowCameraStrategy.h"
#include "Camera.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"

#define CAMERA_HEIGHT 3.0f
#define ROTATION_SPEED 0.05f

void FollowCameraStrategy::Start(Camera* camera)
{
	m_AutoRotate = false;

	// カメラ距離の初期値
	m_CameraDistance = -3.0f;
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
		// カメラ回転
		float stickX = Input::GetStickRX();
		float stickY = Input::GetStickRY();

		rot.y += stickX * ROTATION_SPEED;


		// カメラ上下角度
		rot.x += stickY * ROTATION_SPEED;


		// カメラ距離変更
		// 下入力で近づく、上入力で遠ざかる
		m_CameraDistance -= stickY * 0.1f;


		// 距離制限
		m_CameraDistance = MyMath::Clamp(
			m_CameraDistance,
			-5.0f, // ファー
			-1.5f // ニア
		);
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


	// プレイヤーから見たカメラのオフセット
	VECTOR offset = VGet(
		0.0f,
		CAMERA_HEIGHT,
		m_CameraDistance
	);


	// オフセットを回転
	offset = MyMath::MatTransform(matRot, offset);


	// 理想カメラ位置
	camera->SetIdealPos(MyMath::VecAdd(playerPos, offset));


	// 注視点
	camera->SetTarget(
		VAdd(playerPos, VGet(0.0f, 1.5f, 0.0f))
	);


	// 上方向
	camera->SetUpVec(VGet(0, 1, 0));
}