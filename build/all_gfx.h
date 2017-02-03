//
// all_gfx.h
//
// One header to rule them and in the darkness bind them
// Date: 2017-02-03 12:22:05

#ifdef __cplusplus
extern "C" {
#endif

//{{BLOCK(player)

//======================================================================
//
//	player, 16x16@8, 
//	Transparent color : 00,FF,00
//	+ palette 256 entries, not compressed
//	+ 4 tiles Metatiled by 2x2 not compressed
//	Total size: 512 + 256 = 768
//
//	Time-stamp: 2017-02-02, 12:25:46
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.13
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_H
#define GRIT_PLAYER_H

#define playerTilesLen 256
extern const unsigned int playerTiles[64];

#define playerPalLen 512
extern const unsigned short playerPal[256];

#endif // GRIT_PLAYER_H

//}}BLOCK(player)

//{{BLOCK(tiles)

//======================================================================
//
//	tiles, 32x32@8, 
//	Transparent color : 00,FF,00
//	+ palette 256 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 512 + 1024 = 1536
//
//	Time-stamp: 2017-02-03, 12:22:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.13
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TILES_H
#define GRIT_TILES_H

#define tilesTilesLen 1024
extern const unsigned int tilesTiles[256];

#define tilesPalLen 512
extern const unsigned short tilesPal[256];

#endif // GRIT_TILES_H

//}}BLOCK(tiles)

#ifdef __cplusplus
};
#endif

