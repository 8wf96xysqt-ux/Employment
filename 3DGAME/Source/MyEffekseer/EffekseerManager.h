#pragma once

#include "DxLib.h"
#include "../ObjectPool/ObjectPool.h"

class EffekseerEffect;

#define EFFEKSEER_MAX_PARTICLE 2000
#define EFFEKSEER_EFFECT_MAX 1000

enum EffekseerEffectType
{
	EFFEKSEER_BEAM,// ホバービームエフェクト
	EFFEKSEER_BEAM_HIT,
	EFFEKSEER_EFFECT_TYPE_MAX
};

class EffekseerManager
{
public:
	EffekseerManager();
	~EffekseerManager();

public:
	static void CreateInstence() { if (!m_Instance) m_Instance = new EffekseerManager; }
	static EffekseerManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }	

public:
	// Effekseerを使うのに必要なセットアップ処理
	// DxLibInitよりも前にやる処理をまとめたもの
	void Setup();

	// Effekseerを使う前に必要な初期化処理
	// DxLibInitよりも後にやる処理をまとめたもの
	bool Init();

	// Effekseerのエフェクトデータをロードする
	void Load();

	// Effekseer使用開始処理
	// Effekseerのエフェクトを実際に使うシーンの初期化処理などで呼ぶ
	void Start();

	// エフェクト再生
	EffekseerEffect* PlayEffect(int type, VECTOR pos);

	// ステップ処理
	void Step();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// Effekseer終了処理
	// Effekseerのエフェクトが不要になったら呼ぶ
	void Fin();

private:
	static EffekseerManager* m_Instance;

	int* m_EffectHandles;
	ObjectPool<EffekseerEffect>* m_Effects;
};