#include "Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"

class skydome {
	public:
		void Intialize(Model* model, ViewProjection* viewProjection);

		void Update();

		void Draw();



	private:
		WorldTransform worldTransform_;
		ViewProjection* viewProjection_ = nullptr;

		Model* model_ =nullptr;


};