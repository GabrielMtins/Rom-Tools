#include "UndoSystem.hpp"

#include <cstdio>

UndoSystem::UndoSystem(void) {
	old_tiles.reserve(MAX_TILES + 256);
	undo_stack.reserve(MAX_TILES + 256);
}

void UndoSystem::beginAction(void) {
	if(action) {
		return;
	}

	action = true;
}

void UndoSystem::endAction(const Rom_Viewer& viewer) {
	if(!action) {
		return;
	}

	action = false;

	if(next_undo_stack == 0) {
		return;
	}

	cleanToFitMaxSize();

	undo_stack.push_back(next_undo_stack);

	for(size_t i = 0; i < next_undo_stack; i++) {
		UndoTile& tile = old_tiles.at(old_tiles.size() - 1 - i);

		copyFromViewer(
				viewer,
				tile.tile_index,
				tile.new_data
				);
	}

	next_undo_stack = 0;
}

void UndoSystem::addTile(const Rom_Viewer& viewer, size_t tile_index) {
	if(!action) {
		return;
	}

	if(isTileOnStack(tile_index)) {
		return;
	}

	if(next_undo_stack == 0) {
		cleanActions();
	}

	UndoTile undo_tile;

	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 8; i++) {
			undo_tile.old_data.at(i + j * 8) = Rom_GetTilePixelColor(&viewer, tile_index, i, j);
		}
	}

	undo_tile.tile_index = tile_index;

	old_tiles.push_back(undo_tile);
	next_undo_stack++;
}

void UndoSystem::undoAction(Rom_Viewer& viewer) {
	if(action) {
		return;
	}

	if(undo_stack.size() == history.num_actions) {
		return;
	}

	size_t num_tiles_undo = *(undo_stack.rbegin() + history.num_actions);

	for(size_t i = 0; i < num_tiles_undo; i++) {
		undoTile(
				viewer,
				*(old_tiles.rbegin() + history.num_tiles)
				);

		history.num_tiles++;
		//old_tiles.pop_back();
	}

	history.num_actions++;

	//undo_stack.pop_back();
}

void UndoSystem::redoAction(Rom_Viewer& viewer) {
	if(action) {
		return;
	}

	if(history.num_actions == 0) {
		return;
	}

	history.num_actions--;

	size_t num_tiles_undo = *(undo_stack.rbegin() + history.num_actions);

	for(size_t i = 0; i < num_tiles_undo; i++) {
		history.num_tiles--;

		redoTile(
				viewer,
				*(old_tiles.rbegin() + history.num_tiles)
				);
	}

}

bool UndoSystem::isOnAction(void) const {
	return action;
}

bool UndoSystem::isTileOnStack(size_t tile_index) const {
	if(old_tiles.empty()) {
		return false;
	}

	for(size_t i = 0; i < next_undo_stack; i++) {
		if(old_tiles.at(old_tiles.size() - i - 1).tile_index == tile_index) {
			return true;
		}
	}

	return false;
}

void UndoSystem::undoTile(Rom_Viewer& viewer, const UndoTile& tile) {
	copyToViewer(viewer, tile.tile_index, tile.old_data);
}

void UndoSystem::redoTile(Rom_Viewer& viewer, const UndoTile& tile) {
	copyToViewer(viewer, tile.tile_index, tile.new_data);
}

void UndoSystem::copyFromViewer(const Rom_Viewer& viewer, size_t tile_index, TileRawData& data) const {
	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 8; i++) {
			data.at(i + j * 8) = Rom_GetTilePixelColor(
					&viewer,
					tile_index,
					i,
					j
					);
		}
	}
}

void UndoSystem::copyToViewer(Rom_Viewer& viewer, size_t tile_index, const TileRawData& data) const {
	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 8; i++) {
			Rom_SetTilePixelColor(
					&viewer,
					tile_index,
					i,
					j,
					data.at(i + j * 8)
					);
		}
	}
}

void UndoSystem::cleanToFitMaxSize(void) {
	if(old_tiles.size() <= MAX_TILES) {
		return;
	}

	size_t tiles_to_remove = 0;
	size_t actions_to_remove = 0;

	for(size_t num_undo : undo_stack) {
		if(old_tiles.size() <= MAX_TILES + tiles_to_remove) {
			break;
		}

		tiles_to_remove += num_undo;
		actions_to_remove++;
	}

	if(actions_to_remove > 0) {
		old_tiles.erase(
				old_tiles.begin(),
				old_tiles.begin() + tiles_to_remove
				);

		undo_stack.erase(
				undo_stack.begin(),
				undo_stack.begin() + actions_to_remove
				);

		if(history.num_actions >= undo_stack.size()) {
			history.num_actions = 0;
			history.num_tiles = 0;
		}
	}
}

void UndoSystem::cleanActions(void) {
	if(history.num_actions > 0) {
		old_tiles.resize(old_tiles.size() - history.num_tiles);
		undo_stack.resize(undo_stack.size() - history.num_actions);
	
		history.num_actions = 0;
		history.num_tiles = 0;
	}
}
