#pragma once
#include "DxLib.h"
#include <vector>

class StageObject;
class Floor;
class Ground;
class Block;
class Prop;


// ステージオブジェクト管理クラス
class StageObjectManager
{
public:
	StageObjectManager();	// コンストラクタ
	~StageObjectManager();	// デストラクタ

	static void CreateInstance() { if (!m_Instance) m_Instance = new StageObjectManager; }
	static StageObjectManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

	void Init();	// 初期化
	void Load();	// ロード
	void Start();	// 開始
	void Update();	// 更新
	void Draw();	// 描画
	void Fin();		// 終了

	// 床を生成する
	Floor* CreateFloor(int id);
	Floor* CreateFloor(int id, VECTOR pos, VECTOR rot, VECTOR scale);

	// 地面を生成する
	Ground* CreateGround(int id);
	Ground* CreateGround(int id, VECTOR pos, VECTOR rot, VECTOR scale);

	// ブロックを生成する
	Block* CreateBlock(int id);
	Block* CreateBlock(int id, VECTOR pos, VECTOR rot, VECTOR scale);

	//小道具を生成する
	Prop* CreateProp(int id);
	Prop* CreateProp(int id, VECTOR pos, VECTOR rot, VECTOR scale);

	// 管理中のステージオブジェクトを取得する
	std::vector<StageObject*> GetStageObjects() { return m_StageObjects; }

private:
	static StageObjectManager* m_Instance;
	std::vector<StageObject*> m_StageObjects;

	Floor* m_OriginalFloors;
	Ground* m_OriginalGround;
	Block* m_OriginalBlocks;
	Prop* m_OriginalProp;
};