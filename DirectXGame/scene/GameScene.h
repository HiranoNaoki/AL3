#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "MapChipField.h"
#include "DebugCamera.h"




/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();




	void GenerateBlocks();

std::vector<std::vector<WorldTransform*>>  worldTransformBlocks_;

private: // メンバ変数




	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHndle_ =0;

	Model* model_ = nullptr;

	ViewProjection viewProjection_;

	Player* player_ = nullptr;

	MapChipField* mapChipField_;

	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
