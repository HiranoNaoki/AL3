#pragma once
#include <Model.h>
#include <ViewProjection.h>
#include <array>
#include <WorldTransform.h>
#include <math.h>
#include "cmath"
#include <corecrt_math_defines.h>
#include <algorithm>
#include<Affine.h>
#include<MathUtilityForText.h>



class DeathParticles {
public:
	
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	
	void Draw();

	bool IsFinished() const { return finished_; }

private:

	ViewProjection* viewProjection_ = nullptr;
	
	Model* model_ = nullptr;

	
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	
	static inline const float kDuration = 2.0f;

	static inline const float kSpeed = 0.05f;
	
	static inline const float kAngleUint = 2.0f * float(M_PI) / 8.0f;
	
	bool isFinished_ = false;

	float counter_ = 0.0f;
	
	ObjectColor objectColor_;
	
	Vector4 color_;

	bool finished_ = false;
};
