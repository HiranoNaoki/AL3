#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
class Player{



public:

Player();

~Player();


void Initialize(Model* madel,uint32_t textureHandle,ViewProjection* viewProjection);

void Update();

void Draw();


private:
WorldTransform  worldTransform_;

Model* model_ = nullptr;

uint32_t textureHndle_ = 0u;

ViewProjection* viewprojection_ = nullptr;
};