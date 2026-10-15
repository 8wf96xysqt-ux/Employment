#include "StageObjectManager.h"
#include "Floor/Floor.h"
#include "Floor/FloorParameter.h"
#include "Block/Block.h"
#include "Block/BlockParameter.h"
#include "Ground/Ground.h"
#include "Ground/GroundParameter.h"
#include "Prop/Prop.h"
#include "Prop/PropParameter.h"

StageObjectManager* StageObjectManager::m_Instance = nullptr;

StageObjectManager::StageObjectManager()
	: m_StageObjects({})
	, m_OriginalFloors(nullptr)
	, m_OriginalGround(nullptr)
	, m_OriginalBlocks(nullptr)
	, m_OriginalProp(nullptr)
{
}

StageObjectManager::~StageObjectManager()
{
	Fin();
}

void StageObjectManager::Init()
{
	// 複製元となるクラスを生成
	m_OriginalFloors = new Floor[FLOOR_MODEL_MAX];
	m_OriginalGround = new Ground[GROUND_MODEL_MAX];
	m_OriginalBlocks = new Block[BLOCK_MODEL_MAX];
	m_OriginalProp   = new Prop[PROP_MODEL_MAX];
}

/// <summary>
/// 複製元のみロードする
/// </summary>
void StageObjectManager::Load()
{
	// 床をロード
	m_OriginalFloors[FLOOR_MODEL_00].Load("Data/Floor/Floor.x");

	// 地面をロード
	m_OriginalGround[GRASS_GROUND_MODEL_01].Load("Data/Floor/GrassFloor.x");
	m_OriginalGround[DIRT_GROUND_MODEL_01].Load("Data/Floor/DirtFloor.x");

	// ブロックをロード
	m_OriginalBlocks[BLOCK_MODEL_00].Load("Data/Block/Block.x");

	//小道具をロード
	m_OriginalProp[FENCE_MODEL_00].Load("Data/Prop/Fence.x");
	m_OriginalProp[FENCE_MODEL_01].Load("Data/Prop/Fence_01.x");
}

void StageObjectManager::Start()
{
	for (auto obj : m_StageObjects)
	{
		obj->Start();
	}
}

void StageObjectManager::Update()
{
	for (auto obj : m_StageObjects)
	{
		obj->Update();
	}
}

void StageObjectManager::Draw()
{
	for (auto obj : m_StageObjects)
	{
		obj->Draw();
	}
}

void StageObjectManager::Fin()
{
	delete[] m_OriginalFloors;
	delete[] m_OriginalGround;
	delete[] m_OriginalBlocks;
	delete[] m_OriginalProp;

	m_OriginalFloors = nullptr;
	m_OriginalGround = nullptr;
	m_OriginalBlocks = nullptr;
	m_OriginalProp = nullptr;
}

Floor* StageObjectManager::CreateFloor(int id)
{
	// IDチェック
	if (id < 0 || id >= FLOOR_MODEL_MAX)
	{
		return nullptr;
	}

	// オリジナルから複製して生成
	StageObject* floor = m_OriginalFloors[id].Clone();

	if (floor == nullptr)
	{
		return nullptr;
	}

	// リストに追加
	m_StageObjects.push_back(floor);

	// 返却すれば作った後にいろいろいじれる
	return static_cast<Floor*>(floor);
}

Floor* StageObjectManager::CreateFloor(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	Floor* floor = CreateFloor(id);

	if (floor == nullptr)
	{
		return nullptr;
	}

	floor->SetTransform(pos, rot, scale);

	return floor;
}

Ground* StageObjectManager::CreateGround(int id)
{
	// IDチェック
	if (id < 0 || id >= GROUND_MODEL_MAX)
	{
		return nullptr;
	}

	// オリジナルから複製して生成
	StageObject* ground = m_OriginalGround[id].Clone();

	if (ground == nullptr)
	{
		return nullptr;
	}

	// リストに追加
	m_StageObjects.push_back(ground);

	// 返却すれば作った後にいろいろいじれる
	return static_cast<Ground*>(ground);
}

Ground* StageObjectManager::CreateGround(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	Ground* ground = CreateGround(id);

	if (ground == nullptr)
	{
		return nullptr;
	}

	ground->SetTransform(pos, rot, scale);

	return ground;
}

Block* StageObjectManager::CreateBlock(int id)
{
	// IDチェック
	if (id < 0 || id >= BLOCK_MODEL_MAX)
	{
		return nullptr;
	}

	// オリジナルから複製して生成
	StageObject* block = m_OriginalBlocks[id].Clone();

	if (block == nullptr)
	{
		return nullptr;
	}

	// リストに追加
	m_StageObjects.push_back(block);

	return static_cast<Block*>(block);
}

Block* StageObjectManager::CreateBlock(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	Block* block = CreateBlock(id);

	if (block == nullptr)
	{
		return nullptr;
	}

	block->SetTransform(pos, rot, scale);

	return block;
}

Prop* StageObjectManager::CreateProp(int id)
{
	// IDチェック
	if (id < 0 || id >= PROP_MODEL_MAX)
	{
		return nullptr;
	}

	// オリジナルから複製して生成
	StageObject* prop = m_OriginalProp[id].Clone();

	if (prop == nullptr)
	{
		return nullptr;
	}

	Prop* result = static_cast<Prop*>(prop);
	result->SetPropID(id);

	// リストに追加
	m_StageObjects.push_back(prop);

	return static_cast<Prop*>(prop);
	
}

Prop* StageObjectManager::CreateProp(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	Prop* prop = CreateProp(id);

	if (prop == nullptr)
	{
		return nullptr;
	}

	prop->SetTransform(pos, rot, scale);

	return prop;
}
