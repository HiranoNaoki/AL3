#include<stdint.h>
#include <cstdint>
#include<vector>
#include<Vector3.h>


enum class MapChipType {
	kBlank,
	kBlock
};





struct MapChipDate {
	std::vector<std::vector<MapChipType>> date;
};

class MapChipField {
	static inline const float kBlockWidth =1.0f;
	static inline const float kBlockHeight =1.0f;

	static inline const uint32_t kNumBlockVirtical=20;
	static inline const uint32_t kNumBlockHorizontal =100;

	MapChipDate mapChipDate_;

	void ResetMapChipDate();

	


	MapChipType GetMapChipTypeByIndex(uint32_t xIndex,uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex,uint32_t yIndex);

	 

		

	public:

		void LoadMapChipCsv(const std::string& filePath);
};

