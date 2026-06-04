#include "RomData.hpp"

#include <fstream>

bool RomData::load(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if(!file.is_open()) {
		return false;
	} 

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	data.resize(size);

	if(!file.read(reinterpret_cast<char *>(data.data()), size)) {
		return false;
	}

	return true;
}

void RomData::setViewerFormat(Rom_Format format) {
	viewer = Rom_CreateViewer(
			format,
			(void *) data.data(),
			data.size()
			);
}

size_t RomData::getMaxColors(void) {
	return Rom_GetFormatMaxColors(viewer.format);
}
