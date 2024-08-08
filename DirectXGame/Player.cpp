#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <MapChipField.h>
#include <DebugText.h>

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	
	assert(model);

	model_ = model;
	
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;
}
void Player::Update() {
	//1
	inputMove();

	// 2
	CollisionMapInfo collisionMapInfo = {};
	
	collisionMapInfo.move = velocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;

	// 2
	ChecMapCollision(collisionMapInfo);

	//3
	CheckMapMove(collisionMapInfo);

	//4
	CheckMapCelling(collisionMapInfo);

	//5
	CheckMapHItWall(collisionMapInfo);
	

	//6
	UpdateOnGround(collisionMapInfo);
	
	//7
	AnimateTurn();


	// 8
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { 
	
	model_->Draw(worldTransform_, *viewProjection_);
}


Vector3 Player::GetworldPosition() {
	
	Vector3 worldPos;
	
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() { 
	Vector3 worldPos = GetworldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::inputMove() {
	
	if (onGround_) {
		
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				
				if (velocity_.x < 0.0f) {
					
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration / 60.0f;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
				
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				
				if (velocity_.x > 0.0f) {
					
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration / 60.0f;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
				
					turnFirstRotationY_ = worldTransform_.rotation_.y;
				
					turnTimer_ = kTimeTurn;
				}
			}
			
			velocity_ += acceleration;
			
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (std::abs(velocity_.x) <= 0.0001f) {
			velocity_.x = 0.0f;
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			
			velocity_ += Vector3(0, kJumpAcceleration / 60.0f, 0);
		}
	} else {
		
		velocity_ += Vector3(0, -kGravityAccleration / 60.0f, 0);
		
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}


void Player::ChecMapCollision(CollisionMapInfo& info) { 
	ChecMapCollisionUp(info);
	ChecMapCollisionDown(info);
	ChecMapCollisionRight(info);
	ChecMapCollisionLeft(info);
}

void Player::ChecMapCollisionUp(CollisionMapInfo& info) {
	
	if (info.move.y <= 0) {
		return;
	}

	
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	bool hit = false;

	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	
	if (hit) {
	
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			
			info.ceiling = true;
		}
	}
}

void Player::ChecMapCollisionDown(CollisionMapInfo& info) {
	
	if (info.move.y >= 0) {
		return;
	}


	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	
	bool hit = false;

	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	
	if (hit) {
	
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
	
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			info.landing = true;
		}
	}
}

void Player::ChecMapCollisionRight(CollisionMapInfo& info) { 
	
	if (info.move.x <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	
	bool hit = false;
	
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	
	if (hit) {
	
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexSet.xIndex) {
			
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			info.hitWall = true;
		}
	}
}

void Player::ChecMapCollisionLeft(CollisionMapInfo& info) {

	if (info.move.x >= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;

	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	
	if (hit) {
		
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexSet.xIndex) {
			
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
			info.hitWall = true;
		}
	}
}

void Player::CheckMapMove(const CollisionMapInfo& info) {

	worldTransform_.translation_ += info.move;
}

void Player::CheckMapCelling(const CollisionMapInfo& info) {

	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit celing\n");
		velocity_.y = 0;
	}
}

void Player::CheckMapHItWall(const CollisionMapInfo& info) {
	
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

void Player::UpdateOnGround(const CollisionMapInfo& info) {

	if (onGround_) {
		
		if (velocity_.y > 0.0f) {
		
			onGround_ = false;
		} else {
			std::array<Vector3, kNumCorner> positionNew;

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			
			bool ground = false;

			MapChipType mapChipType;
		
			MapChipField::IndexSet indexSet;

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				ground = true;
			}
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				ground = true;
			}

			if (!ground) {
				DebugText::GetInstance()->ConsolePrintf("jump");
				onGround_ = false;
			}
		}
	} else {
		
		if (info.landing) {
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;

			DebugText::GetInstance()->ConsolePrintf("onGround");
			onGround_ = true;
		}
	}
}

void Player::AnimateTurn() {
		
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
	
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	/*if (corner == kRightBottom) {
	            return center + {+kWidth / 2.0f, -kHeight / 2.0f, 0};
	        }
	        else if (corner == kLeftBottom) {
	            return center + {-kWidth / 2.0f, -kHeight / 2.0f, 0};
	        }
	        else if (corner == kRightTop) {
	            return center + {+kWidth / 2.0f, -kHeight / 2.0f, 0};
	        }
	        else {
	            return center + {-kWidth / 2.0f, -kHeight / 2.0f, 0};
	        }*/
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, 
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0} 
	};

	    return center + offsetTable[static_cast<uint32_t>(corner)];
    }

void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy;
	
	velocity_ += Vector3(0, 0.5f, 0);
	}


