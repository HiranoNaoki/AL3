#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector3.h"
class Player{



public:

Player();

~Player();


void Initialize(Model* madel,ViewProjection* viewProjection,const Vector3& position);

void Update();

void Draw();


private:

};