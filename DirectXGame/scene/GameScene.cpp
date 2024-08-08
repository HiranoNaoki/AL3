
#include "WorldTransform.h"
#include "GameScene.h"
#include "TextureManager.h"
#include<cassert>



void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; i++) {
for (uint32_t j = 0; j < numBlockHorizontal; j++) {
if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
WorldTransform* worldTransform = new WorldTransform();
worldTransform->Initialize();
worldTransformBlocks_[i][j] = worldTransform;
worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
    }
}
}

}

GameScene::GameScene() {
	
}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	//delete enemy_;
	for (Enemy* newEnemy : enemies_) {
		delete newEnemy;
	}
	enemies_.clear();

	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete modelEnemy_;

	delete modelDeathParticles;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHndle_ = TextureManager::Load("./Resources/player.png");



	model_ = Model::CreateFromOBJ("player",true);
	modelBlock_ = Model::CreateFromOBJ("block", true);


	phase_ = Phase::kPlay;



	//enemy_ = new Enemy();
	for (int32_t i = 0; i < 2; ++i) {
		modelEnemy_ = Model::CreateFromOBJ("enemy", true);
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10, 18);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}


	modelEnemy_ = Model::CreateFromOBJ("enemy",true);

	worldTransform_.Initialize();

	viewProjection_.Initialize();

	player_ = new Player();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1,18);
	
	Vector3 enemyfPosition = mapChipField_->GetMapChipPositionByIndex(15, 18);

  //  enemy_->Initialize(modelEnemy_, &viewProjection_, enemyfPosition);
	//enemy_->SetMapChipField(mapChipField_);


	modelDeathParticles = Model::CreateFromOBJ("deathParticle", true);

	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelDeathParticles, &viewProjection_, playerPosition);


	mapChipField_ =new MapChipField;
	mapChipField_->LoadMapChipCsv("./Resources/map.csv");
	GenerateBlocks();

	player_ -> Initialize(model_,&viewProjection_,playerPosition);

	player_->SetMapChipField(mapChipField_);

	debugCamera_ = new DebugCamera(1280,720);

	//カメラコントローラの生成
	cameracontroller_ = new CameraController();
	//カメラコントローラの初期化
	cameracontroller_->Intialize();
	//追従対象をセット
	cameracontroller_->SetTarget(player_);
	//リセット
	cameracontroller_->Reset();

	CameraController::Rect cameraArea = {12.0f,100-12.0f,6.0f,6.0f};

	cameracontroller_->SetMovableArea(cameraArea);
	
}



void GameScene::Update() {

switch (phase_) {
	case Phase::kPlay:

		#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif  DEBUG



	debugCamera_->Update();
	player_->Update();

	cameracontroller_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;
	}

	//enemy_->Update();

	ChangePhase();

		if (isDebugCameraActive_) {
		
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = cameracontroller_->GetViewProjection().matView;
		viewProjection_.matProjection = cameracontroller_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	
//		viewProjection_.UpdateMatrix();
	}

		for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			// アフィン変換行列の作成
			worldTransformBlockYoko->UpdateMatrix();
		}
	}

			for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
		
	

	CheckAllCollisions();

	break;

case Phase::kDeath:


	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;
			worldTransformBlockYoko->matWorld_ = MakeAffineMatrix(worldTransformBlockYoko->scale_, worldTransformBlockYoko->rotation_, worldTransformBlockYoko->translation_);


			
			worldTransformBlockYoko->UpdateMatrix();
		}
	}

	if (deathParticles_) {
		deathParticles_->Update();
	}


	for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
	//デバッグカメラの更新
	


	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
			finished_ = true;
		}

	break;
	}
	

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 自キャラ
	/// 
	/// 
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			modelBlock_->Draw(*worldTransformBlockYoko, viewProjection_);
		}
	}
	player_->Draw();

	//enemy_->Draw();
	 for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	 if (deathParticles_) {
		deathParticles_->Draw();
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	//判定対象1と2の座標
	AABB aabb1, aabb2;

	//自キャラの座標
	aabb1 = player_->GetAABB();

	//自キャラと敵弾すべての当たり判定
	for (Enemy* enemy : enemies_) {
		//敵弾の座標
		aabb2 = enemy->GetAABB();

		//AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			//自キャラの衝突時コールバックを呼び起こす
			player_->OnCollision(enemy);
			//敵弾の衝突時コールバックを呼び起こす
			enemy->OnCollision(player_);
		}
	}
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			player_->Update();
			const Vector3& deathParticlesPosition = player_->GetworldPosition();

			// パーティクルの初期化
			modelDeathParticles = Model::CreateFromOBJ("deathParticle", true);

			// パーティクルの生成処理
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelDeathParticles, &viewProjection_, deathParticlesPosition);
		}

		break;
	case Phase::kDeath:

		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}

		break;
	}
}
