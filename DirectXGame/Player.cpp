#define NOMINMAX
#include "Player.h"
#include <cassert>
#include<numbers>
#include<Input.h>
#include<algorithm>
#include <MapChipField.h>
#include"MathUtilityForText.h"
#include <DebugText.h>








Player::Player(){};

Player::~Player(){};

void Player::Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection,const Vector3& position) {

	assert(model);
	model_ = model;
	textureHndle_ = textureHandle;
	viewprojection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float>/2.0f;
	model_ = Model::CreateFromOBJ("player",true);
}


void Player::Update() {
	//1
	//moov
	if(onGround_){
		
	  if (Input::GetInstance()->PushKey(DIK_RIGHT) ||
		Input::GetInstance()->PushKey(DIK_LEFT)) {


		  if(turnTimer_ > 0.0f){
		turnTimer_ -= 1.0f/60.0f;

		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float>*3.0f/2.0f
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		worldTransform_.rotation_.y = EaseInOut(destinationRotationY,turnFirstRotationY_,turnTimer_ / kTimeTurn);
	   }




		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
			if (lrDirection_ != LRDirection::KRight) {
				lrDirection_ = LRDirection::KRight;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ =kTimeTurn;
			}
			
		}
		else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}

			acceleration.x -= kAcceleration;
			if (lrDirection_ != LRDirection::KLeft) {
				lrDirection_ = LRDirection::KLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
			
		}
		
		velocity_.x += acceleration.x;

		velocity_.x = std::clamp(velocity_.x,-kLimitRunSpeed,kLimitRunSpeed);
	  }else {
			velocity_.x *= (1.0f - kAttenuation);

			
		}
	
	  if (Input::GetInstance()->PushKey(DIK_UP)) {
		velocity_.y += kJumpAccleration;
	      }
	}else{
		velocity_.y += -kGravityAcceleration;

		velocity_.y = std::max(velocity_.y,-kLmitFallSpeed);

		}




	worldTransform_.translation_.x +=velocity_.x;
	worldTransform_.translation_.y +=velocity_.y;

	

	//2
	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.move = velocity_;

	ChecMapColision(collisionMapInfo);

	



	//3
	CheckMapMove(collisionMapInfo);
	//4
	CheckMapCelling(collisionMapInfo);
	//5

	//6

	//7

	//8





	

	   bool landing = false;
	if (velocity_.y < 0.0f) {
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}


	if(onGround_){

	   if (velocity_.y > 0.0f) {
			onGround_ = false;
		}

	

	   
	}else {
		
		if (landing) {
			worldTransform_.translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
	
	worldTransform_.TransferMatrix();

	

	
	worldTransform_.UpdateMatrix();
	
}
void Player::Draw() {
	model_->Draw(worldTransform_,*viewprojection_,textureHndle_);
}


void Player::ChecMapColision(CollisionMapInfo& info) { 
	ChecMapCollisionUp(info);
	//ChecMapCollisionDown(info);
	//ChecMapCollisionRight(info);
	//ChecMapCollisionLeft(info);
}

void Player::ChecMapCollisionUp(CollisionMapInfo& info) {
	//
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	//
	if (info.move.y <= 0) {
		return;
	}

	MapChipType mapChipType;

	
	bool hit = false;
	
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_+ Vector3(0, +kHeight / 2.0f, 0));
		
		
		
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		
		info.ceiling = true;
	}
}

void Player::CheckMapMove(const CollisionMapInfo& info) {
	//
	worldTransform_.translation_ += info.move;
}


void Player::CheckMapCelling(const CollisionMapInfo& info) {
	//
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit celing\n");
		velocity_.y = 0;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}