#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


class Player{



public:

Player();

~Player();


void Initialize(Model* madel,uint32_t textureHandle,ViewProjection* viewProjection,const Vector3& position);

void Update();

void Draw();


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
};