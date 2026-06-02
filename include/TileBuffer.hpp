#ifndef TILE_BUFFER
#define TILE_BUFFER

#include "TileRawData.hpp"

#include <vector>
#include <rom_tools.h>

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

	void invert(bool horizontal=true);
	void invertHorizontal(void);
	void invertVertical(void);

	void appendData(const Rom_Viewer& viewer, size_t tile_id);

	static void invertTile(TileRawData& data, bool horizontal=true);
};

#endif
