#ifndef ROM_DATA_HPP
#define ROM_DATA_HPP

#include <vector>
#include <cstdint>
#include <string>

#include <rom_tools.h>

#include "Palette.hpp"

struct RomData {
	std::string name = "tmp";
	std::vector<uint8_t> data;
	Palette palette = default_palette;
	Rom_Viewer viewer;

	bool load(const std::string& filename);
	void setViewerFormat(Rom_Format format);
	size_t getMaxColors(void);
};

#endif
