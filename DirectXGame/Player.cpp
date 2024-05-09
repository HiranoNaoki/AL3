#include "Player.h"
#include <cassert>



Player::Player(){};

Player::~Player(){};

void Player::Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection) {

	assert(model);
	model_ = model;
	textureHndle_ = textureHandle;
	viewprojection_ = viewProjection;

	worldTransform_.Initialize();
}

void Player::Draw() {
	model_->Draw(worldTransform_,*viewprojection_,textureHndle_);
}

void Player::Update() {
	worldTransform_.TransferMatrix();
}