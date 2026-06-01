#include "UndoSystem.hpp"

UndoSystem::UndoSystem(void) {
	tiles.reserve(4096);
	undo_stack.reserve(4096);
}

void UndoSystem::endAction(void) {
	if(next_undo_stack == 0) {
		return;
	}

	undo_stack.push_back(next_undo_stack);
	next_undo_stack = 0;
}

void UndoSystem::addTile(const Rom_Viewer& viewer, size_t tile_index) {
	UndoTile undo_tile;

	if(isTileOnStack(tile_index)) {
		return;
	}

	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 8; i++) {
			undo_tile.tile.at(i + j * 8) = Rom_GetTilePixelColor(&viewer, tile_index, i, j);
		}
	}

	undo_tile.tile_index = tile_index;

	tiles.push_back(undo_tile);
	next_undo_stack++;
}

void UndoSystem::undoAction(Rom_Viewer& viewer) {
	if(undo_stack.empty()) {
		return;
	}

	size_t num_tiles_undo = undo_stack.back();

	for(size_t i = 0; i < num_tiles_undo; i++) {
		undoTile(viewer, tiles.back());
		tiles.pop_back();
	}

	undo_stack.pop_back();
}

bool UndoSystem::isTileOnStack(size_t tile_index) const {
	if(tiles.empty()) {
		return false;
	}

	for(size_t i = 0; i < next_undo_stack; i++) {
		if(tiles.at(tiles.size() - i - 1).tile_index == tile_index) {
			return true;
		}
	}

	return false;
}

void UndoSystem::undoTile(Rom_Viewer& viewer, const UndoTile& tile) {
	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 8; i++) {
			Rom_SetTilePixelColor(
					&viewer,
					tile.tile_index,
					i,
					j,
					tile.tile.at(i + j * 8)
					);
		}
	}
}
