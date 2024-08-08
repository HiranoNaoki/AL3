#include "Enemy.h"
#include<assert.h>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);

	model_ = model;

worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	worldTransform_.translation_ += velocity_;


	walkTimer_ += 1.0f / 60.0f;

	
	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = radian;

	
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	
	model_->Draw(worldTransform_, *viewProjection_);
}