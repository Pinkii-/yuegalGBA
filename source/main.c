
#include <stdio.h>
#include <string.h>
#include "../../../tonclib/include/tonc.h"
#include "../build/all_gfx.h"

#define MAP_WIDTH 32
#define MAP_HEIGHT 32

SCR_ENTRY map[MAP_WIDTH][MAP_HEIGHT];
bool tunnel_map[1+MAP_WIDTH*2][1+MAP_HEIGHT*2];

#define SIZE_OF_MAP sizeof(map)


// TILES

#define AIR_NONE 0
#define AIR 1
#define DIRT_TOP 2
#define DIRT 3
#define TUNNEL_UP 4
#define TUNNEL_DOWN 5
#define TUNNEL_LEFT 6
#define TUNNEL_RIGHT 7
#define TUNNEL_HOLE_DOWN_LEFT 8
#define TUNNEL_HOLE_DOWN_RIGHT 9
#define TUNNEL_HOLE_TOP_LEFT 10
#define TUNNEL_HOLE_TOP_RIGHT 11
#define TUNNEL_MID 12

#define NUMTILES 16

#define MAP_POS(_background,_x,_y) *(&se_mem[_background][0] + (_y)*MAP_WIDTH + (_x))

#define SET_ENTRY_MAP(__x,__y) {\
	MAP_POS(1, (__x)  , (__y)  ) = AIR;\
	MAP_POS(1, (__x)+1, (__y)  ) = AIR;\
	MAP_POS(2, (__x)  , (__y)+1) = TUNNEL_LEFT;\
	MAP_POS(2, (__x)+1, (__y)+1) = TUNNEL_RIGHT;\
}

#define PLAYER_RIGHT(__x,__y) {\
	tunnel_map[(__x)+2][(__y)-2] = true;\
	tunnel_map[(__x)+2][(__y)-1] = true;\
	tunnel_map[(__x)+2][(__y)  ] = true;\
	tunnel_map[(__x)+2][(__y)+1] = true;\
	tunnel_map[(__x)+3][(__y)-2] = true;\
	tunnel_map[(__x)+3][(__y)-1] = true;\
	tunnel_map[(__x)+3][(__y)  ] = true;\
	tunnel_map[(__x)+3][(__y)+1] = true;\
}

#define PLAYER_LEFT(__x,__y) {\
	tunnel_map[(__x)-2][(__y)-2] = true;\
	tunnel_map[(__x)-2][(__y)-1] = true;\
	tunnel_map[(__x)-2][(__y)  ] = true;\
	tunnel_map[(__x)-2][(__y)+1] = true;\
	tunnel_map[(__x)-3][(__y)-2] = true;\
	tunnel_map[(__x)-3][(__y)-1] = true;\
	tunnel_map[(__x)-3][(__y)  ] = true;\
	tunnel_map[(__x)-3][(__y)+1] = true;\
}

#define PLAYER_DOWN(__x,__y) {\
	tunnel_map[(__x)-1][(__y)+0] = true;\
	tunnel_map[(__x)  ][(__y)+0] = true;\
	tunnel_map[(__x)+1][(__y)+0] = true;\
	tunnel_map[(__x)-1][(__y)+1] = true;\
	tunnel_map[(__x)  ][(__y)+1] = true;\
	tunnel_map[(__x)+1][(__y)+1] = true;\
	tunnel_map[(__x)-1][(__y)+2] = true;\
	tunnel_map[(__x)  ][(__y)+2] = true;\
	tunnel_map[(__x)+1][(__y)+2] = true;\
}

#define PLAYER_UP(__x,__y) {\
	tunnel_map[(__x)-1][(__y)-1] = true;\
	tunnel_map[(__x)  ][(__y)-1] = true;\
	tunnel_map[(__x)+1][(__y)-1] = true;\
	tunnel_map[(__x)-1][(__y)-2] = true;\
	tunnel_map[(__x)  ][(__y)-2] = true;\
	tunnel_map[(__x)+1][(__y)-2] = true;\
	tunnel_map[(__x)-1][(__y)-3] = true;\
	tunnel_map[(__x)  ][(__y)-3] = true;\
	tunnel_map[(__x)+1][(__y)-3] = true;\
	tunnel_map[(__x)-1][(__y)-4] = true;\
	tunnel_map[(__x)  ][(__y)-4] = true;\
	tunnel_map[(__x)+1][(__y)-4] = true;\
}

#define PLAYER_THERE(__x,__y) {\
	tunnel_map[((__x)*2)-1][((__y)*2)-1] = true;\
	tunnel_map[((__x)*2)  ][((__y)*2)-1] = true;\
	tunnel_map[((__x)*2)+1][((__y)*2)-1] = true;\
	tunnel_map[((__x)*2)-1][((__y)*2)  ] = true;\
	tunnel_map[((__x)*2)  ][((__y)*2)  ] = true;\
	tunnel_map[((__x)*2)+1][((__y)*2)  ] = true;\
	tunnel_map[((__x)*2)-1][((__y)*2)+1] = true;\
	tunnel_map[((__x)*2)  ][((__y)*2)+1] = true;\
	tunnel_map[((__x)*2)+1][((__y)*2)+1] = true;\
}

#define SET_PLAYER_PIXEL(__x, __y) {\
	obj_set_pos(&oam_mem[0], (__x), (__y));\
}

// init

void initMap() {
	memset16(&map[0][0], DIRT, SIZE_OF_MAP/2);
	for (int i = 0; i < MAP_WIDTH; ++i) {
		map[0][i] = map[1][i] = map[2][i] = AIR;
		map[3][i] = DIRT_TOP;
	}
}

void updateTunnels() {
	for (int x = 1; x < 1+(MAP_WIDTH*2); x+=2) {
		for (int y = 9; y < 1+(MAP_HEIGHT*2);y+=2) {
			if (tunnel_map[x][y]) {
				bool up    = tunnel_map[x  ][y-1];
				bool down  = tunnel_map[x  ][y+1];
				bool left  = tunnel_map[x-1][y  ];
				bool right = tunnel_map[x+1][y  ];
				int numberOfSides = up + down + left + right;
				if (numberOfSides == 4) {
					MAP_POS(2, x/2, y/2) = TUNNEL_MID;
				}
				else if (up && down) {
					if (right)
						MAP_POS(2, x/2, y/2) = TUNNEL_LEFT;
					else if (left)
						MAP_POS(2, x/2, y/2) = TUNNEL_RIGHT;
				}
				else if (right && left) {
					if (up)
						MAP_POS(2, x/2, y/2) = TUNNEL_DOWN;
					else if (down)
						MAP_POS(2, x/2, y/2) = TUNNEL_UP;
				}
				else if (left && down) {
					MAP_POS(2, x/2,y/2) = TUNNEL_HOLE_TOP_RIGHT;
				}
				else if (left && up) {
					MAP_POS(2, x/2,y/2) = TUNNEL_HOLE_DOWN_RIGHT;
				}
				else if (right && down) {
					MAP_POS(2, x/2,y/2) = TUNNEL_HOLE_TOP_LEFT;
				}
				else if (right && up) {
					MAP_POS(2, x/2,y/2) = TUNNEL_HOLE_DOWN_LEFT;
				}
			}
			else {

			}
		}
	}
}

void initTunnels() {
	memset(&tunnel_map[0][0], 0, sizeof(tunnel_map));
	for (int x = 0; x < 1+(MAP_WIDTH*2); ++x) {
		tunnel_map[x][0] = true;
		tunnel_map[x][1] = true;
		tunnel_map[x][2] = true;
		tunnel_map[x][3] = true;
		tunnel_map[x][4] = true;
		tunnel_map[x][5] = true;
		tunnel_map[x][6] = true;
		tunnel_map[x][7] = true;
		tunnel_map[x][8] = true;
	}

	SET_ENTRY_MAP(15, 3);
	PLAYER_THERE(15,3);

}


int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	oam_init(oam_mem, 128);

	obj_set_attr(&oam_mem[0],
		ATTR0_SQUARE | ATTR0_8BPP,
		ATTR1_SIZE_16, ATTR2_PRIO(0));

	obj_set_pos(&oam_mem[0], 8*15, 8*3);

	initMap();

	u16 camera_x = 0;
	u16 camera_y = 0;

	u16 player_x =2+2*15;
	u16 player_y =1+2*3;
	int count = 0;

	// Video mode 0, enable bg 0.
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;


	memcpy(&tile8_mem[0][0], tilesTiles, tilesTilesLen);
	memcpy(pal_bg_mem, tilesPal, tilesPalLen);

	memcpy(&se_mem[1][0], map, SIZE_OF_MAP);
	REG_BG0CNT = BG_CBB(0) | BG_SBB(1) | BG_8BPP | BG_REG_32x32 | BG_PRIO(2);

	memset16(&se_mem[2][0], AIR_NONE, SIZE_OF_MAP/2);
	REG_BG1CNT = BG_CBB(0) | BG_SBB(2) | BG_8BPP | BG_REG_32x32 | BG_PRIO(1);

	memcpy(&tile8_mem[4], playerTiles, playerTilesLen);
	memcpy(pal_obj_mem, playerPal, playerPalLen);


	initTunnels();

	while(1)
	{
		// Input
		key_poll();
		int dx = 0;
		int dy = 0;

		if (++count%3 == 0) {
			dx = key_tri_horz();
			dy = key_tri_vert();
		}


		// Update

		// Camera

		int camera_mov = 1;

		if (dx < 0 && ((int) camera_x+dx) >= 0) camera_x += dx * camera_mov;
		else if (dx > 0 && camera_x+dx < (MAP_WIDTH - 30) * 8) camera_x += dx * camera_mov;

		if (dy < 0 && ((int) camera_y+dy) >= 0) camera_y += dy * camera_mov;
		else if (dy > 0 && camera_y+dy < (MAP_HEIGHT - 20) * 8) camera_y += dy * camera_mov;

		REG_BG0HOFS = camera_x;
		REG_BG0VOFS = camera_y;
		REG_BG1HOFS = camera_x;
		REG_BG1VOFS = camera_y;

		// Player

		if (dx || dy) {
			// if (dy > 0) {
			// 	PLAYER_DOWN(player_x, player_y);
			// 	updateTunnels();
			// }
			// else if (dy < 0) {
			// 	PLAYER_UP(player_x, player_y);
			// 	updateTunnels();
			// }
			// if (dx > 0) {
			// 	PLAYER_RIGHT(player_x, player_y);
			// 	updateTunnels();
			// }
			// else if (dx < 0) {
			// 	PLAYER_LEFT(player_x, player_y);
			// 	updateTunnels();
			// }

			player_y+= dy*2;
			player_x+= dx*2;

			PLAYER_THERE(player_x/2, player_y/2);
			updateTunnels();

			u16 global_player_x = (((player_x/2)-1)*8) - camera_x;
			u16 global_player_y = (((player_y/2)-1)*8) - camera_y;

			SET_PLAYER_PIXEL(global_player_x, global_player_y);

		}


		// Draw
		VBlankIntrWait();
	}

	return 0;
}
