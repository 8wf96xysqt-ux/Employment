#pragma once
//前方定義
class CollisionAABB;
class CollisionSphere;
class CollisionSlope;
class CollisionOBB;

#define COLLISION_MAX 10000

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

public:
	// マネージャーインスタンス管理
	static void CreateInstance() { if (!m_Instance) m_Instance = new CollisionManager; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static CollisionManager* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Update(); //更新
	void Draw();	// 描画
	void Fin();		// 終了

public:
	// AABBを生成する
	CollisionAABB* CreateAABB();
	// AABBを削除する
	void DeleteAABB(CollisionAABB* targetAABB);
	// Sphereを生成する
	CollisionSphere* CreateSphere();
	// Sphereを削除する
	void DeleteSphere(CollisionSphere* targetSphere);
	//Slopeを生成
	CollisionSlope* CreateSlope();
	//Slopeを削除
	void DeleteSlope(CollisionSlope* targetSlope);
	//OBBを作成
	CollisionOBB* CreateOBB();
	//OBBを削除
	void DeleteOBB(CollisionOBB* targetOBB);
public:
	// 当たり判定のチェック
	void CheckCollision();


private:
	// CollisionManagerインスタンス
	static CollisionManager* m_Instance;
	// 当たり判定管理用配列
	CollisionAABB* m_AABB[COLLISION_MAX];
	CollisionSphere* m_Sphere[COLLISION_MAX];
	CollisionSlope* m_Slope[COLLISION_MAX];
	CollisionOBB* m_OBB[COLLISION_MAX];
	
};
