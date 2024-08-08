#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class MapChipField;

class Player{



public:

Player();

~Player();


void Initialize(Model* madel,uint32_t textureHandle,ViewProjection* viewProjection,const Vector3& position);

void Update();

void Draw();

const WorldTransform& GetWorldTransform() const {return worldTransform_;}

const Vector3& GetVelocity() const {return velocity_;}

static inline const float kWidth = 0.8f;
static inline const float kHeight = 0.8f;

void SetMapChipField(MapChipField* mapChipField){mapChipField_ = mapChipField;}

struct CollisionMapInfo {
		bool ceiling = false; 
		bool landing = false; 
		bool hitWall = false; 
		Vector3 move;
	};


void ChecMapColision(CollisionMapInfo& info);

void ChecMapCollisionUp(CollisionMapInfo& info);

enum Corner {
		kRightBottom, 
		kLeftBottom,  
		kRightTop,    
		kLeftTop,     

		kNumCorner    
	};

void CheckMapMove(const CollisionMapInfo& info);


void CheckMapCelling(const CollisionMapInfo& info);



private:
WorldTransform  worldTransform_;

Model* model_ = nullptr;

uint32_t textureHndle_ = 0u;

ViewProjection* viewprojection_ = nullptr;

Vector3 velocity_ ={};

static inline const float kAcceleration = 0.02f;

static inline const float kAttenuation = 0.2f;

static inline const float kLimitRunSpeed = 0.5f;

enum class LRDirection {
	KRight,
	KLeft,
};

LRDirection lrDirection_ = LRDirection::KRight;

float turnFirstRotationY_ = 0.0f;

float turnTimer_ = 0.0f;

static inline const float kTimeTurn = 0.3f;

bool onGround_ = true;

static inline const float kGravityAcceleration = 0.09f;

static inline const float kLmitFallSpeed = 1.0f;

static inline const float kJumpAccleration =1.2f;

MapChipField* mapChipField_ = nullptr;



Vector3 CornerPosition(const Vector3& center, Corner corner);

static inline const float kBlank = 1.0f;

};