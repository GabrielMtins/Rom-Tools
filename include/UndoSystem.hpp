#ifndef UNDO_SYSTEM_HPP
#define UNDO_SYSTEM_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>

#include <rom_tools.h>

#include "TileRawData.hpp"

struct UndoTile {
	TileRawData old_data;
	TileRawData new_data;
	size_t tile_index;
};

class UndoSystem {
	public:
		UndoSystem(void);

		void beginAction(void);
		void endAction(const Rom_Viewer& viewer);
		void addTile(const Rom_Viewer& viewer, size_t tile_index);
		void undoAction(Rom_Viewer& viewer);
		void redoAction(Rom_Viewer& viewer);
		bool isOnAction(void) const;

	private:
		bool isTileOnStack(size_t tile_index) const;
		void undoTile(Rom_Viewer& viewer, const UndoTile& tile);
		void redoTile(Rom_Viewer& viewer, const UndoTile& tile);
		void copyFromViewer(const Rom_Viewer& viewer, size_t tile_index, TileRawData& data) const;
		void copyToViewer(Rom_Viewer& viewer, size_t tile_index, const TileRawData& data) const;
		void cleanToFitMaxSize(void);
		void cleanActions(void);

		std::vector<UndoTile> old_tiles;
		std::vector<size_t> undo_stack;

		size_t next_undo_stack = 0;

		bool action;

		struct {
			size_t num_actions = 0;
			size_t num_tiles = 0;
		} history;

		static constexpr size_t MAX_TILES = 4096;
};

#endif
