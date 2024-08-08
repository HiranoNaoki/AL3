#include<map>

#include <string>
#include<fstream>
#include<sstream>
#include<assert.h>
#include"MapChipField.h"

namespace {
	std::map<std::string, MapChipType> mapChipTable = {
	{"0",MapChipType::kBlank},
	{"1",MapChipType::kBlock},
};

}





void MapChipField::ResetMapChipDate() {
	mapChipDate_.date.clear();
	mapChipDate_.date.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDateLine : mapChipDate_.date) {
		mapChipDateLine.resize(kNumBlockHorizontal);
	}
}


  void MapChipField::LoadMapChipCsv(const std::string& filePath){
	ResetMapChipDate();

	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	 


	std::stringstream mapChipCsv;

	mapChipCsv <<file.rdbuf();

	file.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv,line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string world;
			getline(line_stream,world,',');

			if(mapChipTable.contains(world)){
				mapChipDate_.date[i][j] = mapChipTable[world];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex,uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}

	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipDate_.date[yIndex][xIndex];
}
Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex,uint32_t yindex){
	return Vector3(kBlockWidth * xIndex,kBlockHeight *(kNumBlockVirtical - 1 - yindex),0);


}


MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>((position.y + kBlockHeight / 2) / kBlockHeight);
	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) { 
	
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	return rect;
}
