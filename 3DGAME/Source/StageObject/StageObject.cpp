#include "StageObject.h"

#define ALPHA_EASE_SPEED 0.2f

StageObject::StageObject()
{
	m_Handle = -1;
	m_Pos = {};
	m_Rot = {};
	m_Scale = {};
	m_AABB = nullptr;
}
StageObject::~StageObject()
{
	Fin();
}
void StageObject::Start()
{
}
void StageObject::Load(const char* fileName)
{
	m_Handle = MV1LoadModel(fileName);
}
void StageObject::Update()
{
	MV1SetPosition(m_Handle, m_Pos);
	MV1SetRotationXYZ(m_Handle, m_Rot);
	MV1SetScale(m_Handle, m_Scale);

	// アルファ値を目標値に向けてイージング
	m_Alpha += (m_TargetAlpha - m_Alpha) * ALPHA_EASE_SPEED;
}
void StageObject::Draw()
{
	bool isTransparent = m_Alpha < 254.5f;

	if (isTransparent)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)m_Alpha);

		// マテリアル側のアルファも直接操作する
		int matNum = MV1GetMaterialNum(m_Handle);
		for (int i = 0; i < matNum; i++)
		{
			COLOR_F col = MV1GetMaterialDifColor(m_Handle, i);
			col.a = m_Alpha / 255.0f; // 0.0～1.0の範囲
			MV1SetMaterialDifColor(m_Handle, i, col);
		}
	}

	MV1DrawModel(m_Handle);

	if (isTransparent)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// マテリアルのアルファを元に戻す
		int matNum = MV1GetMaterialNum(m_Handle);
		for (int i = 0; i < matNum; i++)
		{
			COLOR_F col = MV1GetMaterialDifColor(m_Handle, i);
			col.a = 1.0f;
			MV1SetMaterialDifColor(m_Handle, i, col);
		}
	}
}
void StageObject::Fin()
{
	MV1DeleteModel(m_Handle);
}