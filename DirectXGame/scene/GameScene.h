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
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"





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


	void ChangePhase();

	void GenerateBlocks();


	void CheckAllCollisions();

std::vector<std::vector<WorldTransform*>>  worldTransformBlocks_;

bool IsFinished() const { return finished_; }

private: // メンバ変数




	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHndle_ =0;

	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticles = nullptr;

	WorldTransform worldTransform_;
	
	ViewProjection viewProjection_;

	Player* player_ = nullptr;

	MapChipField* mapChipField_;



	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	CameraController* cameracontroller_ = nullptr;
	

	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;


	DeathParticles* deathParticles_ = nullptr;

	enum class Phase {
		kPlay,  
		kDeath, 
	};

	Phase phase_;

	bool finished_ = false;

	float time =0;
};
