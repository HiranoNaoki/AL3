#define NOMINMAX
#include "CameraController.h"
#include "Player.h"

float Lerp(float x1, float x2, float t);

void CameraController::Intialize(){
	viewProjection_.Initialize();
};

void CameraController::Update(){
	const Vector3& targetVelocity = target_->GetVelocity();

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	cameraTarget.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x* kVelocityBias;
	cameraTarget.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y*kVelocityBias;
	cameraTarget.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z*kVelocityBias;

	viewProjection_.translation_.x = Lerp(viewProjection_.translation_.x,cameraTarget.x,kInterpolationRate);
	viewProjection_.translation_.y = Lerp(viewProjection_.translation_.y,cameraTarget.y,kInterpolationRate);
	viewProjection_.translation_.z = Lerp(viewProjection_.translation_.z,cameraTarget.z,kInterpolationRate);
	
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin.top);
	





	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x,movableArea_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x,movableArea_.right);
	viewProjection_.translation_.y= std::max(viewProjection_.translation_.y,movableArea_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y,movableArea_.top);

	viewProjection_.UpdateMatrix();
};

void CameraController::Reset(){
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

};
