#pragma once
#include <vector>
#include <string>
#include "StageParameter.h"

// ステージの読み込みと各オブジェクトの配置を管理するクラス
class StageManager
{
public:
    // デストラクタ
    ~StageManager();

    // マネージャーのインスタンス（実体）を取得する関数（シングルトン設計）
    static StageManager* GetInstance()
    {
        if (m_Instance == nullptr) {
            m_Instance = new StageManager(); // インスタンスがなければ自動生成
        }
        return m_Instance;
    }

    // 使い終わったマネージャーのインスタンスを削除する関数
    static void DeleteInstance()
    {
        if (m_Instance != nullptr) {
            delete m_Instance;
            m_Instance = nullptr;
        }
    }

    void Load(const char* fileName); 
    void Start();              
    void ResetEnemies();  // 敵を初期配置に戻す
    void Draw();                    
    void Fin();                      

private:
    // コンストラクタ（外部からの生成を禁止）
    StageManager();

    // 唯一のマネージャー実体を指す静的ポインタ変数
    static StageManager* m_Instance;

    //メンバー変数：JSONから読み込んだ初期配置データの格納リスト
    std::vector<GameObject> m_Objects;     // 読み込んだ地形オブジェクト（床・壁）データの配列
    std::vector<EnemyObject> m_Enemies;    // 読み込んだエネミーオブジェクト（経路データ付き）データの配列
};
