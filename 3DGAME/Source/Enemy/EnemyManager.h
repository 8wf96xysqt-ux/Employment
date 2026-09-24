#pragma once

#include <vector>
#include <memory>
#include <string>
#include "DxLib.h"

class EnemyBase;

class EnemyManager
{
public:
    EnemyManager();    // コンストラクタ
    ~EnemyManager();   // デストラクタ

public:
    // 敵マネージャーを生成する
    // EnemyManagerはゲーム上に１つのみとするので
    // ２回目以降はnewが通らないようになっている
    // （デザインパターン：シングルトン）
    static void CreateInstance() { if (!m_Instance) m_Instance = new EnemyManager; }

    // マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
    static EnemyManager* GetInstance() { return m_Instance; }

    // 使わなくなったら削除する際の削除関数
    static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
    void Init();	 // 初期化
    void Load();	 // ロード
    void Start();	 // 開始
    void Step();	 // ステップ
    void Update();	 // 更新
    void Draw();	 // 描画
    void DrawDebug();//デバック
    void Fin();		 // 終了
    // 敵を生成
    void CreateEnemy(
        const std::string& name,
        VECTOR pos,
        VECTOR rot,
        VECTOR scale);

    // 敵を追加
    void AddEnemy(std::unique_ptr<EnemyBase> enemy);

private:
    // 生成されたEnemyManager自身を格納する変数
    // EnemyManagerはゲーム上に１つのみなのでstaticにしている
    static EnemyManager* m_Instance;

    // 敵一覧
    std::vector<std::unique_ptr<EnemyBase>> m_Enemies;
};