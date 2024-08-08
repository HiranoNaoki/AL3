#pragma once
#include <Model.h>
#include <Vector3.h>
#include <Input.h>
#include <MathUtilityForText.h>
#include <algorithm>
#include <numbers>
#include <WorldTransform.h>

class MapChipField;

class Enemy {

	public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }


private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	MapChipField* mapChipField_ = nullptr;
	
	static inline const float kWalkSpeed = 0.02f;
	
	Vector3 velocity_ = {};
	
	static inline const float kWalkMotionAngleStart = 0.0f;
	
	static inline const float kWalkMotionAngleEnd = 0.4f;
	
	static inline const float kWalkMotionTime = 1.0f;
	
	float walkTimer_ = 0.0f;

};