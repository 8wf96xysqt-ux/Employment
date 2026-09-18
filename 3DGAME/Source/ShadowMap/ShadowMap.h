#pragma once

#include "DxLib.h" // VECTOR型を使用するためインクルードを追加

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new ShadowMap; }
	static ShadowMap* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Init();
	void Fin();

	void StartDrawShadowMap();	// シャドウマップへの描画を開始
	void EndDrawShadowMap();	// シャドウマップへの描画を終了
	void StartAppearsShadowMap();	// シャドウマップの写しこみを開始
	void EndAppearsShadowMap();	// シャドウマップの写しこみを終了

private:
	static ShadowMap* m_Instance;
	int m_Handle;
};
