#define NOMINMAX
#include "Player.h"
#include <cassert>
#include<numbers>
#include<Input.h>
#include<algorithm>

float Lerp(float x1,float x2,float t){return (1.0f - t) * x1 + t * x2;}

float EaseInOut(float x1, float x2, float t) {
	float x;
    float easedT = -(std::cosf(std::numbers::pi_v<float> *t)- 1.0f)/2.0f;
   
	
	x = (1.0f - easedT) * x1 + easedT*x2;
	
	
	return x;

}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
    return Vector3(Lerp(v1.x,v2.x,t),Lerp(v1.y,v2.y,t),Lerp(v1.z,v2.z,t));
}

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

void Player::Draw() {
	model_->Draw(worldTransform_,*viewprojection_,textureHndle_);
}

void Player::Update() {
	worldTransform_.TransferMatrix();

	

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

	worldTransform_.UpdateMatrix();

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
	
	
	
}