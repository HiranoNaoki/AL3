#include "Player.h"
#include <cassert>



Player::Player(){};

Player::~Player(){};

void Player::Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position) {


	worldTransform_.Initialize();
	worldTransform_.translation_ =position;
}

void Player::Draw() {
	
}

void Player::Update() {
	
}