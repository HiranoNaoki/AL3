#include <WorldTransform.h>
#include <Model.h>
#include <Vector3.h>
#include <numbers>
#include <Input.h>
#include <MathUtilityForText.h>
#include <algorithm>
#include <AABB.h>

class MapChipField;
class Player;

class Goal {
public:
	/// <summary>

	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	
	/// </summary>
	void Update();

	/// <summary>

	/// </summary>
	void Draw();

	
	bool IsFinished() const { return finished_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	Vector3 GetworldPosition();

	AABB GetAABB();

	
	void OnCollision(const Player* player);

private:
	
	WorldTransform worldTransform_;
	
	Model* model_ = nullptr;
	
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	
	MapChipField* mapChipField_ = nullptr;

	bool isFinished_ = false;
	
	bool finished_ = false;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};