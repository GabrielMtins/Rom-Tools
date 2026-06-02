#ifndef TILE_BUFFER
#define TILE_BUFFER

#include "TileRawData.hpp"

#include <vector>

struct TileBuffer {
	std::vector<TileRawData> raw_data;
	size_t width = 0, height = 0;

	inline TileBuffer(void) {
		raw_data.reserve(64);
	}

	inline void reset(void) {
		raw_data.resize(0);
		width = 0;
		height = 0;
	}

	void invertHorizontal(void);

	void invertVertical(void);

	static void invertTile(TileRawData& data, bool horizontal=true);
};

#endif
