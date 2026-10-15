#include "DxLib.h"
#include "ShadowMap.h"

#define SHADOW_MAP_SIZE_X 4096
#define SHADOW_MAP_SIZE_Y 4096

#define SHADOW_MAP_AREA_MIN VGet(-20.0f, -20.0f, -20.0f)
#define SHADOW_MAP_AREA_MAX VGet(20.0f, 20.0f, 20.0f)
ShadowMap* ShadowMap::m_Instance = nullptr;

ShadowMap::ShadowMap()
{
	m_Handle = 0;
}

ShadowMap::~ShadowMap()
{
	Fin();
}

void ShadowMap::Init()
{

	m_Handle = MakeShadowMap(SHADOW_MAP_SIZE_X,SHADOW_MAP_SIZE_Y);

	// 上から斜めに照らす
	VECTOR lightDirection = VGet(0.0f,-1.0f,-0.0f);

	SetShadowMapLightDirection(m_Handle,lightDirection);

	SetShadowMapDrawArea(m_Handle,SHADOW_MAP_AREA_MIN,SHADOW_MAP_AREA_MAX);
}

void ShadowMap::Fin()
{
	DeleteShadowMap(m_Handle);
}

void ShadowMap::StartDrawShadowMap()
{
	// シャドウマップへの描画を開始する
	// 終了までに描画されたモデルの影がシャドウマップに描画される
	ShadowMap_DrawSetup(m_Handle);

}

void ShadowMap::EndDrawShadowMap()
{
	ShadowMap_DrawEnd();
}

void ShadowMap::StartAppearsShadowMap()
{
	// シャドウマップの映しこみを開始する
	// 終了までに描画されるモデルには
	// シャドウマップに映っている影が映る
	SetUseShadowMap(0, m_Handle);

}

void ShadowMap::EndAppearsShadowMap()
{
	// これ以上映すモデルがない場合は-1を渡して終了する
	SetUseShadowMap(0, -1);

}
