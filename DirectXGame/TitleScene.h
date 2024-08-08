#pragma once
#include <Input.h>
#include "Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"


class TitleScene {
public:

	~TitleScene();
	
	void Initialize();

	void Update();

	
	void Draw();
	
	bool IsFinished() const { return finished_; }

private:

	ViewProjection viewProjection_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* model_ = nullptr;

	float counter_ = 0.0f;
	
	bool finished_ = false;
	static inline const float kTimeTitleMove = 2.0f;
};