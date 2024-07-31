#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>


class Player;

class CameraController{
	public:

		void Intialize();

		void Update();

		void SetTarget(Player* target) {target_ = target;}

		void Reset();

		const ViewProjection& GetViewProjection()const{return viewProjection_;}

		struct Rect {
			float left =0.0f;
			float right = 1.0f;
			float bottom = 0.0f;
			float top = 1.0f;
		};

		Rect movableArea_ = {0,100,0,100,};

		void SetMovableArea(Rect area){movableArea_ = area;}

		private:

		ViewProjection viewProjection_;

		Player* target_ = nullptr;

		Vector3 cameraTarget;

		static inline const float kInterpolationRate = 0.35f;

		Vector3 targetOffset_ = {0,0,-15.0f};

		static inline const float kVelocityBias = 10;

		static inline const Rect margin = {-50,50,-50,50};


};