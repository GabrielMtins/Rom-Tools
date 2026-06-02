#include "TileBuffer.hpp"

void TileBuffer::invert(bool horizontal) {
	if(horizontal) {
		invertHorizontal();
	} else {
		invertVertical();
	}
}

void TileBuffer::invertHorizontal(void) {
	if(width * height == 0) {
		return;
	}

	for(auto& tile : raw_data) {
		invertTile(tile, true);
	}

	for(size_t j = 0; j < height; j++) {
		for(size_t i = 0; i < width / 2; i++) {
			std::swap(
					raw_data.at(i + j * width),
					raw_data.at(width - 1 - i + j * width)
					);
		}
	}
}

void TileBuffer::invertVertical(void) {
	if(width * height == 0) {
		return;
	}

	for(auto& tile : raw_data) {
		invertTile(tile, false);
	}

	for(size_t j = 0; j < height / 2; j++) {
		for(size_t i = 0; i < width; i++) {
			std::swap(
					raw_data.at(i + (j) * width),
					raw_data.at(i + (height - 1 - j) * width)
					);
		}
	}
}

void TileBuffer::appendData(const Rom_Viewer& viewer, size_t tile_id) {
	raw_data.emplace_back();
	auto& raw_tile = raw_data.back();

	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 8; i++) {
			raw_tile.at(i + j * 8) = Rom_GetTilePixelColor(
					&viewer,
					tile_id,
					i,
					j
					);
		}
	}

	width++;
}

void TileBuffer::invertTile(TileRawData& data, bool horizontal) {
	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 4; i++) {
			int to_index_1, to_index_2;

			if(horizontal) {
				to_index_1 = i + j * 8;
				to_index_2 = (7 - i) + j * 8;
			} else {
				to_index_1 = j + i * 8;
				to_index_2 = j + (7 - i) * 8;
			}

			std::swap(data.at(to_index_1), data.at(to_index_2));
		}
	}
}
