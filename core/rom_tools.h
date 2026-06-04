#ifndef ROM_TOOLS_H
#define ROM_TOOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define FOR_TILE_TYPE_LIST(DO) \
	DO(ROM_TYPE_NES, Rom_NesTile, Nes_GetColor, Nes_SetColor) \
	DO(ROM_TYPE_GB, Rom_GbTile, Gb_GetColor, Gb_SetColor)

typedef enum {
	#define EXPAND_AS_ENUM(id, type, get, set) id,
	FOR_TILE_TYPE_LIST(EXPAND_AS_ENUM)
	#undef EXPAND_AS_ENUM
} Rom_Format;

typedef struct {
	size_t num_tiles;
	Rom_Format format;

	void *data;
} Rom_Viewer;

Rom_Viewer Rom_CreateViewer(Rom_Format format, void *data, size_t size);

size_t Rom_GetRomSizeByTiles(Rom_Format format, size_t num_tiles);

size_t Rom_GetFormatMaxColors(Rom_Format format);

void Rom_AdvanceOffset(Rom_Viewer *viewer, size_t offset_tiles);

uint8_t Rom_GetTilePixelColor(const Rom_Viewer *viewer, size_t tile_id, uint8_t x, uint8_t y);

void Rom_SetTilePixelColor(Rom_Viewer *viewer, size_t tile_id, uint8_t x, uint8_t y, uint8_t c);

#ifdef __cplusplus
}
#endif 

#ifdef ROM_TOOLS_IMPLEMENTATION

#define SET_BIT(byte, bit, value) ((byte) = ((byte) & ~(1 << (bit))) | (((value) & 1) << (bit)))
#define GET_BIT(byte, bit) (((byte) >> (bit)) & 1)

typedef struct {
	uint8_t plane0[8];
	uint8_t plane1[8];
} Rom_NesTile;

typedef struct {
	uint8_t data[16];
} Rom_GbTile;

#define EXPAND_AS_SIZEOF(id, type, get, set) sizeof(type),
static const size_t format_size[] = {
	FOR_TILE_TYPE_LIST(EXPAND_AS_SIZEOF)
};
#undef EXPAND_AS_SIZEOF

#define EXPAND_AS_MAX_COLORS(id, type, get, set) (1 << ((sizeof(type) * 8) / 64)),
static const size_t format_max_colors[] = {
	FOR_TILE_TYPE_LIST(EXPAND_AS_MAX_COLORS)
};
#undef EXPAND_AS_MAX_COLORS


static uint8_t Nes_GetColor(const Rom_NesTile *tile, uint8_t x, uint8_t y) {
	x = 7 - x;

	return
		(GET_BIT(tile->plane1[y], x) << 1) |
		(GET_BIT(tile->plane0[y], x));
}

static void Nes_SetColor(Rom_NesTile *tile, uint8_t x, uint8_t y, uint8_t c) {
	x = 7 - x;

	SET_BIT(tile->plane0[y], x, c);
	SET_BIT(tile->plane1[y], x, c >> 1);
}

static uint8_t Gb_GetColor(const Rom_GbTile *tile, uint8_t x, uint8_t y) {
	x = 7 - x;
	return
		(GET_BIT(tile->data[y * 2 + 1], x) << 1) |
		(GET_BIT(tile->data[y * 2], x));
}

static void Gb_SetColor(Rom_GbTile *tile, uint8_t x, uint8_t y, uint8_t c) {
	x = 7 - x;

	SET_BIT(tile->data[y * 2 + 1], x, c >> 1);
	SET_BIT(tile->data[y * 2], x, c);
}

Rom_Viewer Rom_CreateViewer(Rom_Format format, void *data, size_t size) {
	Rom_Viewer viewer;

	viewer.data = data;
	viewer.format = format;

	viewer.num_tiles = size / format_size[format];

	return viewer;
}

size_t Rom_GetRomSizeByTiles(Rom_Format format, size_t num_tiles) {
	return num_tiles * format_size[format];
}

size_t Rom_GetFormatMaxColors(Rom_Format format) {
	return format_max_colors[format];
}

void Rom_AdvanceOffset(Rom_Viewer *viewer, size_t offset_tiles) {
	size_t offset_bytes = 0;

	if(offset_tiles >= viewer->num_tiles) {
		return;
	}

	offset_bytes = offset_tiles * format_size[viewer->format];

	viewer->data = (void *) (((uint8_t *) viewer->data) + offset_bytes);

	viewer->num_tiles -= offset_tiles;
}

uint8_t Rom_GetTilePixelColor(const Rom_Viewer *viewer, size_t tile_id, uint8_t x, uint8_t y) {
	uint8_t ret_value = 0x00;

	if(tile_id >= viewer->num_tiles) {
		return 0x00;
	}

	#define EXPAND_AS_CASE(id, type, get, set) case id: ret_value = get(((const type *) viewer->data) + tile_id, x, y); break;

	switch(viewer->format) {
		FOR_TILE_TYPE_LIST(EXPAND_AS_CASE);
	}

	#undef EXPAND_AS_CASE

	return ret_value;
}

void Rom_SetTilePixelColor(Rom_Viewer *viewer, size_t tile_id, uint8_t x, uint8_t y, uint8_t c) {
	if(tile_id >= viewer->num_tiles) {
		return;
	}

	#define EXPAND_AS_CASE(id, type, get, set) case id: set(((type *) viewer->data) + tile_id, x, y, c); break;

	switch(viewer->format) {
		FOR_TILE_TYPE_LIST(EXPAND_AS_CASE)
	}

	#undef EXPAND_AS_CASE
}

#endif

#endif
