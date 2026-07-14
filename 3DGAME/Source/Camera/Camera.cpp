#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/StageObject.h"
#include "../Collision/CollisionAABB.h"
#include "Camera.h"
#include "CameraStrategy.h" 

#define CAMERA_NEAR_CLIP 0.01f
#define CAMERA_FAR_CLIP 1000.0f
#define OCCLUSION_ALPHA 70.0f 
#define NORMAL_ALPHA 255.0f
#define CAMERA_PULL_MARGIN 0.3f  
#define CAMERA_MIN_DISTANCE 0.5f

Camera::Camera()
{
	// 各メンバ変数を初期化する
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Target = VGet(0.0f, 0.0f, 0.0f);
	m_UpVec = VGet(0.0f, 0.0f, 0.0f);
	m_TargetPlayer = nullptr;
	m_TargetYaw = 0.0f;
	
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
}
void Camera::Step()
{
}

void Camera::SetStrategy(std::unique_ptr<CameraStrategy> strategy)
{
	m_Strategy = std::move(strategy);
	if (m_Strategy)
	{
		m_Strategy->Start(this); // 切り替わった瞬間の初期化処理を呼ぶ
	}
}
void Camera::Update()
{
	//現在のストラテジー（3D追従や2D固定など）を実行する
	//各ストラテジー内で m_Rot, m_IdealPos, m_Target, m_UpVec が更新される
	if (m_Strategy)
	{
		m_Strategy->Update(this);
	}

	

	//ストラテジーが計算した理想位置を元に、共通の壁抜け・遮蔽処理を通す
	float nearestT = UpdateOcclusion();

	//最終的なカメラ座標を決定
	m_Pos = CalcPulledCameraPos(m_Target, m_IdealPos, nearestT);

	//DxLibのカメラに反映
	SetCameraPositionAndTargetAndUpVec(m_Pos, m_Target, m_UpVec);

	
}
// 遮蔽オブジェクトの透明化処理 + ターゲット?理想カメラ位置間の最近ヒット位置をt値で返す
float Camera::UpdateOcclusion()
{
	// ステージ上の全オブジェクトを取得する
	std::vector<StageObject*> objects = StageObjectManager::GetInstance()->GetStageObjects();
	int hitCount = 0;
	float nearestT = 1.0f; // ヒットが無ければ1.0(理想位置まで届く)のまま
	for (auto obj : objects)
	{
		// 遮蔽対象でないオブジェクトはスキップする
		if (!obj->IsOcclusionTarget()) continue;
		// オブジェクトの当たり判定(AABB)を取得する
		CollisionAABB* aabb = obj->GetAABB();
		// 当たり判定が無いオブジェクトはスキップする
		if (aabb == nullptr) continue;
		// ターゲットから理想カメラ位置へのレイで判定し、ヒット位置をt値で取得する
		float t = 1.0f;
		bool isHit = aabb->CheckRayHit(m_Target, m_IdealPos, &t);
		if (isHit)
		{
			// ヒット数を加算する
			hitCount++;
			// 複数ヒットした場合、ターゲットに最も近いt値を採用する
			if (t < nearestT)
			{
				nearestT = t;
			}
		}
		// ヒットしていれば半透明、していなければ不透明にする
		obj->SetTransparency(isHit ? OCCLUSION_ALPHA : NORMAL_ALPHA);
	}
	// デバッグ表示用にヒット数を保存する
	m_DebugHitCount = hitCount;
	return nearestT;
}
// t値を使って、壁の手前まで寄せたカメラ位置を計算する
VECTOR Camera::CalcPulledCameraPos(const VECTOR& target, const VECTOR& idealPos, float nearestT) const
{
	// ヒットが無ければそのまま理想位置を返す
	if (nearestT >= 1.0f)
	{
		return idealPos;
	}
	// ターゲットから理想カメラ位置への方向と距離を求める
	VECTOR toIdeal = MyMath::VecSub(idealPos, target);
	float idealDistance = VSize(toIdeal);
	// ゼロ除算防止
	if (idealDistance < 0.0001f)
	{
		return idealPos;
	}
	// t値を実距離に変換する
	float hitDistance = idealDistance * nearestT;
	// ヒット距離からさらにマージン分手前に止める
	float pulledDistance = hitDistance - CAMERA_PULL_MARGIN;
	// 距離を下限?理想距離の範囲にクランプする
	pulledDistance = MyMath::Clamp(pulledDistance, CAMERA_MIN_DISTANCE, idealDistance);
	// ターゲットから計算した距離分だけ理想位置方向に進んだ座標を返す
	VECTOR dirToIdeal = VScale(toIdeal, 1.0f / idealDistance);
	return MyMath::VecAdd(target, VScale(dirToIdeal, pulledDistance));
}
void Camera::Draw()
{
	// カメラ座標をデバッグ表示する
	DrawFormatString(0, 60, GetColor(255, 255, 255), "カメラの座標[%f, %f, %f]", m_Pos.x, m_Pos.y, m_Pos.z);
	// 注視点をデバッグ表示する
	DrawFormatString(0, 80, GetColor(255, 255, 255), "ターゲット[%f, %f, %f]", m_Target.x, m_Target.y, m_Target.z);
	// 遮蔽ヒット数をデバッグ表示する
	DrawFormatString(0, 100, GetColor(255, 255, 0), "遮蔽ヒット数[%d]", m_DebugHitCount);
	// カメラ位置からX方向に伸びるテスト用の線の終点を作る
	VECTOR testEnd = VAdd(m_Pos, VGet(5.0f, 0.0f, 0.0f));
	// テスト用の線を描画する
	DrawLine3D(m_Pos, testEnd, GetColor(0, 255, 0));
	// 注視点に球を描画する
	DrawSphere3D(m_Target, 0.25f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
}
void Camera::Fin()
{
}