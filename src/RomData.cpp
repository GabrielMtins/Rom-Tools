#include "RomData.hpp"

#include <fstream>
#include <unordered_map>

static const std::unordered_map<std::string, Rom_Format> extension_to_format = {
	{".nes", ROM_TYPE_NES},
	{".gb", ROM_TYPE_GB},
};

bool RomData::load(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if(!file.is_open()) {
		return false;
	} 

	name = filename;
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	data.resize(size);

	if(!file.read(reinterpret_cast<char *>(data.data()), size)) {
		return false;
	}

	setViewerFormat(getExpectedFormat());

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

Rom_Format RomData::getExpectedFormat(void) {
	std::string file_format = name.substr(name.find_last_of("."));

	if(extension_to_format.find(file_format) == extension_to_format.end()) {
		return ROM_TYPE_NES;
	}

	return extension_to_format.at(file_format);
}
