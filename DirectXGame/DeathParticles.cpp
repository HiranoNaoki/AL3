#include "DeathParticles.h"
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;


	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
	for (uint32_t i = 0; i < 8; ++i) {
		
		Vector3 velocity = {kSpeed, 0.0f, 0.0f};
		
		float angle = kAngleUint * i;
	
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
	
		velocity = Transform(velocity, matrixRotation);
		
		worldTransforms_[i].translation_ += velocity;
	}
	
	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration) {
		counter_ = kDuration;
		
		isFinished_ = true;
	}
	color_.w = std::clamp(1.0f - counter_, 0.0f, 1.0f);
	
	
	objectColor_.SetColor(color_);
	
	objectColor_.TransferMatrix();
	
}

void DeathParticles::Draw() {
	
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}