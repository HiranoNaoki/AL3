#include <WorldTransform.h>
#include <Model.h>
#include <Vector3.h>
#include <numbers>
#include <Input.h>
#include <MathUtilityForText.h>
#include <algorithm>

class MapChipField;


class Player{
public:

	
	enum class LRDirection {
		kRight,
		kLeft,
	};

	
	enum Corner {
		kRightBottom, 
		kLeftBottom,  
		kRightTop,    
		kLeftTop,     

		kNumCorner 
	};

	
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	
	void Update();


	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	private:

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.05f;

	static inline const float kJumpAcceleration = 20.0f;
	
	static inline const float kGravityAccleration = 0.98f;
	static inline const float kAttenuationWall = 0.2f;

	static inline const float kAttenuationLanding = 0.5f;

	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kLimitRunSpeed = 0.5f;
	
	static inline const float kTimeTurn = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;
	static inline const float kGroundSearchHeight = 0.06f;

	struct CollisionMapInfo {
		bool ceiling = false; 
		bool landing = false; 
		bool hitWall = false; 
		Vector3 move;
	};

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;
	
	Model* model_ = nullptr;
	
	Vector3 velocity_ = {};



	bool onGround_ = true;

	LRDirection lrDirection_ = LRDirection::kRight;

	
	float turnFirstRotationY_ = 0.0f;
	
	float turnTimer_ = 0.0f;
	
	MapChipField* mapChipField_ = nullptr;

	void inputMove();

	void ChecMapCollision(CollisionMapInfo& info);
	void ChecMapCollisionUp(CollisionMapInfo& info);
	void ChecMapCollisionDown(CollisionMapInfo& info);
	void ChecMapCollisionRight(CollisionMapInfo& info);
	void ChecMapCollisionLeft(CollisionMapInfo& info);

	void CheckMapMove(const CollisionMapInfo& info);

	void CheckMapCelling(const CollisionMapInfo& info);

	void UpdateOnGround(const CollisionMapInfo& info);

	void CheckMapHItWall(const CollisionMapInfo& info);

	void AnimateTurn();

	Vector3 CornerPosition(const Vector3& center, Corner corner);
};