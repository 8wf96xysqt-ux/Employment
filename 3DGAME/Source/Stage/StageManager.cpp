#include "StageManager.h"
#include "StageParameter.h"
#include "../StageObject/StageObjectManager.h"
#include <fstream>
#include "../StageObject/StageObjectManager.h"


StageManager* StageManager::m_Instance = nullptr;

// usingして使いやすくする
using json = nlohmann::json;
// データがまとまっている階層のキー名
constexpr const char* KEY_ITEMS = "items";
StageManager::StageManager()
{
}

StageManager::~StageManager()
{
    Fin();
}

void StageManager::Load(const char* fileName)
{
    //jsonファイルを開く
    std::ifstream file(fileName);
    if (!file.is_open()){
        return;
    }
    //開いたjsonファイルをjsonクラスに取り込み
    json stangeJson;
    file >> stangeJson;

    //from_json関数を元にjsonをvectorに格納
    m_Objects = stangeJson[KEY_ITEMS].get<std::vector<GameObject>>();

    file.close();


}

/// <summary>
/// ステージ開始処理
/// 主に各オブジェクトを配置する
/// </summary>
void StageManager::Start()
{
    //jsonデータをもとに配置
    for (GameObject& obj : m_Objects)
    {
        if (obj.id <= FLOOR_00)
        {
            int id = obj.id - FLOOR_00;

            StageObjectManager::GetInstance()->CreateFloor(id, obj.pos, obj.rot, obj.scale);
        }
        else if (obj.id <= BLOCK_00)
        {
            int id = obj.id - BLOCK_00;
            StageObjectManager::GetInstance()->CreateBlock(id, obj.pos, obj.rot, obj.scale);
        }
        else if (obj.id <= SLOPE_00)
        {
            int id = obj.id - SLOPE_00;
            StageObjectManager::GetInstance()->CreateSlope(id, obj.pos, obj.rot, obj.scale);
        }
    }

}

void StageManager::Draw()
{
}

void StageManager::Fin()
{

}




