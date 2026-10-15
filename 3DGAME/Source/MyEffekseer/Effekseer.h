#pragma once

#include "DxLib.h"

// Effekseerエフェクト１つ分のクラス
class EffekseerEffect
{
public:
	EffekseerEffect();	// コンストラクタ
	~EffekseerEffect();	// デストラクタ

public:
	void Step();	// ステップ
	void Update();	// 更新
	void Fin();		// 終了

	// 再生関数
	// efkefcデータのリソースハンドルを渡す
	void Play(int handle);
	// エフェクト停止
	void Stop();

	void Reset();

	// 必要なアクセサ
	bool IsActive() const { return m_Active; }

	void SetActive(bool active) { m_Active = active; }
	void SetPos(VECTOR pos) { m_Pos = pos; }

private:
	bool m_Active;	// アクティブフラグ
	int m_Handle;	// プレイハンドル（注意：リソースハンドルではない）
	VECTOR m_Pos;	// 座標
};