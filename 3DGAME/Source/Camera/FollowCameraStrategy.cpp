#include "FollowCameraStrategy.h"
#include "Camera.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"

constexpr float CAMERA_HEIGHT = 3.0f;
constexpr float ROTATION_SPEED = 0.05f;
constexpr float CAMERA_Y_FOLLOW_SPEED = 0.045f;

void FollowCameraStrategy::Start(Camera* camera)
{
	m_AutoRotate = false;
	m_CameraDistance = -3.0f;

	auto* targetPlayer = camera->GetTargetPlayer();

	if (targetPlayer)
	{
		VECTOR playerPos = targetPlayer->GetPos();

		// ゲーム開始時はプレイヤーの高さにカメラを合わせる
		m_CameraY = playerPos.y;
	}
}

void FollowCameraStrategy::Update(Camera* camera)
{
	auto* targetPlayer = camera->GetTargetPlayer();

	if (!targetPlayer)
	{
		return;
	}

	VECTOR rot = camera->GetRotation();

	// Rボタンを押したらプレイヤーの向いている方向へカメラを戻す
	if (Input::IsCameraReset())
	{
		m_TargetYaw = targetPlayer->GetRot().y;
		m_AutoRotate = true;
	}

	// カメラ自動回転
	if (m_AutoRotate)
	{
		// スティック操作をしたら自動回転をキャンセル
		if (fabsf(Input::GetStickRX()) > 0.1f || fabsf(Input::GetStickRY()) > 0.1f)
		{
			m_AutoRotate = false;
		}
		else
		{
			float diff = m_TargetYaw - rot.y;

			// 角度差を -PI ～ PI に収める
			if (diff > DX_PI_F)
			{
				diff -= DX_PI_F * 2.0f;
			}

			if (diff < -DX_PI_F)
			{
				diff += DX_PI_F * 2.0f;
			}

			// 目標方向へ徐々に回転
			rot.y += diff * 0.1f;

			// ほぼ目標方向になったら終了
			if (fabsf(diff) < 0.01f)
			{
				rot.y = m_TargetYaw;
				m_AutoRotate = false;
			}
		}
	}
	else
	{
		// 右スティックからカメラ操作量を取得
		float stickX = Input::GetStickRX();
		float stickY = Input::GetStickRY();

		// 左右でカメラを回転
		rot.y += stickX * ROTATION_SPEED;

		// 上下でカメラを回転
		rot.x += stickY * ROTATION_SPEED;

		// 上下操作に合わせてカメラ距離を変更
		m_CameraDistance -= stickY * 0.1f;

		// カメラ距離を制限
		m_CameraDistance = MyMath::Clamp(m_CameraDistance, -4.0f, -1.0f);
	}

	// カメラの上下回転を制限
	rot.x = MyMath::Clamp(rot.x, -DX_PI_F * 0.3f, DX_PI_F * 0.2f);

	camera->SetRotation(rot);

	VECTOR playerPos = targetPlayer->GetPos();

	// プレイヤーが最後に着地した（あるいは現在立っている）床のリアルタイムな高さを取得
	float groundY = targetPlayer->GetGroundY();

	// カメラのベースY座標は、ジャンプ中のプレイヤーの高さではなく、
	// 常に「床の高さ」に向かって滑らかに追従させる
	m_CameraY += (groundY - m_CameraY) * CAMERA_Y_FOLLOW_SPEED;


	// カメラの基準位置
	VECTOR cameraPlayerPos = playerPos;

	// プレイヤー自身のYではなく、
	// カメラが追従している「床のY座標」を使用する
	cameraPlayerPos.y = m_CameraY;

	// カメラの回転行列を作成
	MATRIX matYaw = MyMath::MatRotationYaw(rot.y);
	MATRIX matPitch = MyMath::MatRotationPitch(rot.x);
	MATRIX matRot = MyMath::MatMult(matYaw, matPitch);

	// プレイヤーから見たカメラの相対位置
	VECTOR offset = VGet(0.0f, CAMERA_HEIGHT, m_CameraDistance);

	// カメラの回転に合わせてオフセットを回転
	offset = MyMath::MatTransform(matRot, offset);

	// カメラの理想位置を設定
	camera->SetIdealPos(MyMath::VecAdd(cameraPlayerPos, offset));

	// 注視点を作成
	VECTOR targetPos = cameraPlayerPos;

	// プレイヤーより1.2f上を注視
	targetPos.y += 1.2f;

	// カメラY（床ベース）から注視点が離れすぎないように制限
	// ※ジャンプ中にアストロがはみ出すぎる場合は、ここの制限（+1.0f）を少し広げてください
	targetPos.y = MyMath::Clamp(targetPos.y, m_CameraY - 1.0f, m_CameraY + 2.0f);

	// 注視点を設定
	camera->SetTarget(targetPos);


	// レイキャスト専用の注視点はプレイヤーに追従させる
	camera->SetRaycastTarget(VAdd(playerPos, VGet(0.0f, 1.5f, 0.0f)));

	// カメラの上方向
	camera->SetUpVec(VGet(0.0f, 1.0f, 0.0f));
}
