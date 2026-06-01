#ifndef UNDO_SYSTEM_HPP
#define UNDO_SYSTEM_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>

#include <rom_tools.h>

struct UndoTile {
	std::array<uint8_t, 64> tile;
	size_t tile_index;
};

class UndoSystem {
	public:
		UndoSystem(void);

		void endAction(void);
		void addTile(const Rom_Viewer& viewer, size_t tile_index);
		void undoAction(Rom_Viewer& viewer);

	private:
		bool isTileOnStack(size_t tile_index) const;
		void undoTile(Rom_Viewer& viewer, const UndoTile& tile);

		std::vector<UndoTile> tiles;
		std::vector<size_t> undo_stack;

		size_t next_undo_stack = 0;
};

#endif
