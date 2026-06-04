#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <array>
#include <cstdint>

constexpr int PALETTE_SIZE = 16;

using Palette = std::array<uint32_t, PALETTE_SIZE>;

constexpr Palette default_palette = {
	/*
	0xffffffff,
	0xffc0c0c0,
	0xff808080,
	0xff000000,
	*/
	0xff000000,
	0xffff0000,
	0xfffadbb3,
	0xff0000ff,

	0xffff0000,
	0xffc00000,
	0xff800000,
	0xff000000,
	0xff00ff00,
	0xff00c000,
	0xff008000,
	0xff000000,
	0xff0000ff,
	0xff0000c0,
	0xff000080,
	0xff000000,
};

#define PALETTE_GET_R(x) (((x) >> 16) & 0xff)
#define PALETTE_GET_G(x) (((x) >> 8) & 0xff)
#define PALETTE_GET_B(x) (((x)) & 0xff)

#endif
