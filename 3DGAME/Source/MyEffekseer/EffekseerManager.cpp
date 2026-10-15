#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"
#include "Effekseer.h"

EffekseerManager* EffekseerManager::m_Instance = nullptr;

EffekseerManager::EffekseerManager()
	:m_EffectHandles(nullptr)
	, m_Effects(nullptr)
{
}

EffekseerManager::~EffekseerManager()
{
	Fin();
}

void EffekseerManager::Setup()
{
	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);
}

bool EffekseerManager::Init()
{
	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(EFFEKSEER_MAX_PARTICLE) == -1)
	{
		return false;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	return true;
}

void EffekseerManager::Load()
{
	// 各エフェクトデータをロード
	m_EffectHandles = new int[EFFEKSEER_EFFECT_TYPE_MAX];
	m_EffectHandles[EFFEKSEER_BEAM] = LoadEffekseerEffect("Data/Effect/Beam.efkefc");
	m_EffectHandles[EFFEKSEER_BEAM_HIT] =LoadEffekseerEffect("Data/Effect/BeamHit1.efkefc");
	if (m_EffectHandles[EFFEKSEER_BEAM_HIT] == -1)
	{
		OutputDebugStringA("BeamHit1 Load Failed\n");
	}
	else
	{
		OutputDebugStringA("BeamHit1 Load Success\n");
	}
}

void EffekseerManager::Start()
{
	// 管理用のPoolを生成
	m_Effects = new ObjectPool<EffekseerEffect>(EFFEKSEER_EFFECT_MAX);
}

EffekseerEffect* EffekseerManager::PlayEffect(int type, VECTOR pos)
{
	EffekseerEffect* effect = m_Effects->Get();

	if (!effect)
	{
		return nullptr;
	}

	effect->SetActive(true);

	// エフェクト再生
	effect->Play(m_EffectHandles[type]);

	// 位置設定
	effect->SetPos(pos);

	// 再生するエフェクトを返却
	return effect;
}

void EffekseerManager::Step()
{
	// 各エフェクトを更新
	for (auto& effect : m_Effects->GetObjects())
	{
		effect->Step();
	}
}

void EffekseerManager::Update()
{
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// 各エフェクトを更新
	for (auto& effect : m_Effects->GetObjects())
	{
		effect->Update();
	}

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
}

void EffekseerManager::Draw()
{
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

void EffekseerManager::Fin()
{
	OutputDebugStringA("Effekseer Fin 1\n");

	if (m_EffectHandles)
	{
		OutputDebugStringA("Effekseer Fin 2\n");

		for (int i = 0; i < EFFEKSEER_EFFECT_TYPE_MAX; i++)
		{
			DeleteEffekseerEffect(m_EffectHandles[i]);
		}

		delete[] m_EffectHandles;
		m_EffectHandles = nullptr;
	}

	OutputDebugStringA("Effekseer Fin 3\n");

	if (m_Effects)
	{
		delete m_Effects;
		m_Effects = nullptr;
	}

	OutputDebugStringA("Effekseer Fin 4\n");

	Effkseer_End();

	OutputDebugStringA("Effekseer Fin 5\n");
}