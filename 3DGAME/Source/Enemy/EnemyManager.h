#pragma once
#include <vector>
#include <memory>
#include <string>
#include "DxLib.h"

class EnemyBase;
class Player;

class EnemyManager
{
public:
    EnemyManager();
    ~EnemyManager();

    static void CreateInstance() { if (!m_Instance) m_Instance = new EnemyManager; }
    static EnemyManager* GetInstance() { return m_Instance; }
    static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

    void Init();
    void Load();
    void Start();
    void Step();
    void Update();
    void Draw();
    void DrawDebug();
    void Fin();

    void CreateEnemy(int id, const std::string& name, VECTOR pos, VECTOR rot, VECTOR scale,
        int moveType, float radius, bool clockwise, const std::vector<VECTOR>& points);

    void AddEnemy(std::unique_ptr<EnemyBase> enemy);
    // Œ»İ‚Ì“G‚ğ‚·‚×‚Äíœ
    void ClearEnemies();
    void CheckPlayerAttack(Player* player);

private:
    static EnemyManager* m_Instance;
    std::vector<std::unique_ptr<EnemyBase>> m_Enemies;
    void CheckEnemyCollision();
};
