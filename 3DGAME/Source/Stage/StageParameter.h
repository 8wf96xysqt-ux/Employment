#pragma once
#include "DxLib.h"
#include "../Library/json/json.hpp"
#include <string>
#include <vector>

/// <summary>
/// Unityから出力された地形・背景などの一般オブジェクト用データ構造体
/// </summary>
struct GameObject
{
	int id = -1;            // 配置オブジェクトID（LcateObjectIDに対応）
	VECTOR pos = {};        // 配置座標（x, y, z）
	VECTOR rot = {};        // 回転角（x, y, z）
	VECTOR scale = {};      // スケール（x, y, z）
	std::string name = "";  // Unityのヒエラルキー名
};

/// <summary>
/// Unityから出力されたエネミー専用の移動経路パラメータ付きデータ構造体
/// </summary>
struct EnemyObject
{
	int id = -1;                        // エネミー個別ID（EnemyObjectIDに対応）
	VECTOR pos = {};                    // 初期配置座標（x, y, z）
	VECTOR rot = {};                    // 初期回転角（x, y, z）
	VECTOR scale = {};                  // 初期スケール（x, y, z）
	std::string name = "";              // Unityのヒエラルキー名

	// --- 移動経路制御パラメータ ---
	int movementType = 0;               // 移動タイプ（0: サークル移動, 1: 地点巡回）
	float circleRadius = 0.0f;          // サークル移動の半径（Unity側で設定）
	bool clockwise = true;              // 時計回りフラグ（true: 時計回り, false: 反時計回り）
	std::vector<VECTOR> patrolPoints;   // 巡回する座標のリスト（Patrol時に使用）
};

/// <summary>
/// Unityの配置アセットに対応する地形オブジェクトID定義
/// </summary>
enum LcateObjectID
{
	FLOOR_00,       // 通常の床
	BLOCK_00,       // 壁・ブロック
	DIRT_FLOOR_01,  // 土の床
	GRASS_FLOOR_01,  // 草の床
	FENCE_00,       //フェンス
	FENCE_01,       //90度版(フェンス)
	LOCATE_OBJECT_MAX
};

/// <summary>
/// Unityのエネミー個別アセットに対応するID定義
/// </summary>
enum EnemyObjectID
{
	ENEMY_ID_00,    // 通常エネミー（ノーマル）
	ENEMY_ID_01     // 拡張用エネミー
};

/// <summary>
/// エネミーの巡回移動タイプ定義
/// </summary>
enum EnemyMovementType
{
	Circle = 0,     // 円周周回移動
	Patrol = 1      // 複数ポイント巡回移動
};

/// <summary>
/// nlohmann/jsonの内包データをDxLibのVECTOR構造体に変換する
/// </summary>
/// <param name="j">対象のjsonオブジェクト</param>
/// <param name="v">代入先のVECTOR構造体</param>
inline void jsonConvXYZ(const nlohmann::json& j, VECTOR& v)
{
	v.x = j.value("x", 0.0f);
	v.y = j.value("y", 0.0f);
	v.z = j.value("z", 0.0f);
}

inline void ConvertRotation(VECTOR& rot)
{
	rot.x *= DX_PI_F / 180.0f;
	rot.y = (rot.y + 180.0f) * DX_PI_F / 180.0f;
	rot.z *= DX_PI_F / 180.0f;
}


/// <summary>
/// jsonから一般オブジェクト（GameObject）への自動マッピング関数
/// </summary>
inline void from_json(const nlohmann::json& j, GameObject& obj)
{
	obj.id = j.value("id", 0);
	obj.name = j.value("name", "");
	jsonConvXYZ(j.at("position"), obj.pos);
	jsonConvXYZ(j.at("rotation"), obj.rot);
	jsonConvXYZ(j.at("scale"), obj.scale);

	ConvertRotation(obj.rot);
}

/// <summary>
/// jsonからデータ拡張されたエネミーオブジェクト（EnemyObject）への自動マッピング関数
/// </summary>
inline void from_json(const nlohmann::json& j, EnemyObject& enemy)
{
	enemy.id = j.value("id", 0);
	enemy.name = j.value("name", "");
	jsonConvXYZ(j.at("position"), enemy.pos);
	jsonConvXYZ(j.at("rotation"), enemy.rot);
	jsonConvXYZ(j.at("scale"), enemy.scale);

	ConvertRotation(enemy.rot);

	// 移動パラメータの解析
	enemy.movementType = j.value("movementType", 0);
	enemy.circleRadius = j.value("circleRadius", 0.0f);
	enemy.clockwise = j.value("clockwise", true);

	// 可変長配列（巡回地点リスト）の動的解析
	enemy.patrolPoints.clear();
	if (j.contains("patrolPoints") && j.at("patrolPoints").is_array())
	{
		for (const auto& pointJson : j.at("patrolPoints"))
		{
			VECTOR pt = {};
			jsonConvXYZ(pointJson, pt);
			enemy.patrolPoints.push_back(pt);
		}
	}
}

