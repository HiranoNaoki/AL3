#include"skydome.h"

void skydome::Intialize(Model *model,ViewProjection* viewProjection) {
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;

}

void skydome::Update(){}

void skydome::Draw() {
	model_->Draw(worldTransform_,*viewProjection_);
}