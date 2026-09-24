#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/StageObject.h"
#include "../Collision/CollisionAABB.h"
#include "Camera.h"
#include "CameraStrategy.h"
#include "../GameSetting/Color.h"

constexpr float CAMERA_NEAR_CLIP = 0.01f;
constexpr float CAMERA_FAR_CLIP = 1000.0f;
constexpr float OCCLUSION_ALPHA = 70.0f;
constexpr float NORMAL_ALPHA = 255.0f;
constexpr float CAMERA_PULL_MARGIN = 0.3f;
constexpr float CAMERA_MIN_DISTANCE = 0.5f;

Camera::Camera()
	:m_Pos(VGet(0.0f, 0.0f, 0.0f))
	, m_Rot(VGet(0.0f, 0.0f, 0.0f))
	, m_Target(VGet(0.0f, 0.0f, 0.0f))
	, m_RaycastTarget(VGet(0.0f, 0.0f, 0.0f))
	, m_UpVec(VGet(0.0f, 0.0f, 0.0f))
	, m_IdealPos(VGet(0.0f, 0.0f, 0.0f))
	, m_TargetPlayer(nullptr)
	, m_TargetYaw(0.0f)
{
}
Camera::~Camera()
{
	Fin();
}

void Camera::Init()
{
}

void Camera::Load()
{
}

void Camera::Start()
{
	// カメラのニア・ファークリップを設定する
	SetCameraNearFar(CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);

	// 上方向ベクトルをY軸方向に設定する
	m_UpVec = VGet(0.0f, 1.0f, 0.0f);

	// 追従対象のプレイヤーを取得する
	m_TargetPlayer = PlayerManager::GetInstance()->GetPlayer();

	if (m_TargetPlayer)
	{
		VECTOR playerPos = m_TargetPlayer->GetPos();
		m_RaycastTarget =VAdd(playerPos,VGet(0.0f, 1.5f, 0.0f));
	}
}

void Camera::Step()
{
}

void Camera::SetStrategy(std::unique_ptr<CameraStrategy> strategy)
{
	m_Strategy = std::move(strategy);

	if (m_Strategy)
	{
		m_Strategy->Start(this);
	}
}

void Camera::Update()
{
	// 現在のストラテジーを実行する
	if (m_Strategy)
	{
		m_Strategy->Update(this);
	}

	// ストラテジーが計算した理想位置を元に遮蔽処理を行う
	float nearestT = UpdateOcclusion();

	// 最終的なカメラ座標を決定する
	m_Pos = CalcPulledCameraPos(m_RaycastTarget,m_IdealPos,nearestT);

	// DxLibのカメラに反映する
	SetCameraPositionAndTargetAndUpVec(m_Pos,m_Target,m_UpVec);
}

float Camera::UpdateOcclusion()
{
	// ステージ上の全オブジェクトを取得する
	std::vector<StageObject*> objects = StageObjectManager::GetInstance()->GetStageObjects();

	int hitCount = 0;

	// ヒットが無ければ理想位置まで届く
	float nearestT = 1.0f;

	for (auto obj : objects)
	{
		// 遮蔽対象でないオブジェクトはスキップする
		if (!obj->IsOcclusionTarget())
		{
			continue;
		}

		// オブジェクトの当たり判定を取得する
		CollisionAABB* aabb = obj->GetAABB();

		// 当たり判定が無い場合はスキップする
		if (aabb == nullptr)
		{
			continue;
		}

		float t = 1.0f;

		// 注視点から理想カメラ位置へレイを飛ばす
		bool isHit =aabb->CheckRayHit(m_RaycastTarget,m_IdealPos,&t);

		if (isHit)
		{
			hitCount++;

			// 最も注視点に近いヒット位置を使用する
			if (t < nearestT)
			{
				nearestT = t;
			}
		}

		// カメラを遮っているオブジェクトを透明にする
		obj->SetTransparency(isHit ? OCCLUSION_ALPHA : NORMAL_ALPHA);
	}

	// デバッグ用にヒット数を保存する
	m_DebugHitCount = hitCount;

	return nearestT;
}

VECTOR Camera::CalcPulledCameraPos(
	const VECTOR& target,
	const VECTOR& idealPos,
	float nearestT
) const
{
	// 遮蔽されていなければ理想位置をそのまま返す
	if (nearestT >= 1.0f)
	{
		return idealPos;
	}

	// 注視点から理想カメラ位置までのベクトル
	VECTOR toIdeal =MyMath::VecSub(idealPos,target);

	// 注視点からカメラまでの距離
	float idealDistance = VSize(toIdeal);

	// ゼロ除算を防ぐ
	if (idealDistance < 0.0001f)
	{
		return idealPos;
	}

	// ヒット位置までの距離を計算
	float hitDistance = idealDistance * nearestT;

	// 壁から少し手前で止める
	float pulledDistance = hitDistance - CAMERA_PULL_MARGIN;

	// カメラが近づきすぎないように制限する
	pulledDistance =MyMath::Clamp(pulledDistance,CAMERA_MIN_DISTANCE,idealDistance);

	// 注視点からカメラ方向への単位ベクトル
	VECTOR dirToIdeal =VScale(toIdeal,1.0f / idealDistance);

	// 壁の手前まで移動したカメラ位置を返す
	return MyMath::VecAdd(target,VScale(dirToIdeal,pulledDistance));
}

void Camera::Draw()
{
#ifdef _DEBUG

	// カメラ座標をデバッグ表示する
	DrawFormatString(0, 160, Color::Yellow(), "カメラの座標[%f, %f, %f]", m_Pos.x, m_Pos.y, m_Pos.z);

	// 注視点をデバッグ表示する
	DrawFormatString(0, 180, Color::Yellow(), "ターゲット[%f, %f, %f]", m_Target.x, m_Target.y, m_Target.z);

	// 遮蔽ヒット数をデバッグ表示する
	DrawFormatString(0, 200, Color::Yellow(), "遮蔽ヒット数[%d]", m_DebugHitCount);

	// カメラ座標からX方向へテスト線を描画する
	VECTOR testEnd = VAdd(m_Pos, VGet(5.0f, 0.0f, 0.0f));

	DrawLine3D(m_Pos, testEnd, Color::Red());

	// 注視点に球を描画する
	DrawSphere3D(m_Target, 0.25f, 8, Color::White(), Color::White(), FALSE);

#endif // DEBUG
}

void Camera::Fin()
{
}