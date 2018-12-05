// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef PATH_HPP
#define PATH_HPP

#include "../common/cbasetypes.hpp"
#include "../common/mmo.hpp"

enum cell_chk : uint8;

#define MOVE_COST 10
#define MOVE_DIAGONAL_COST 14

#define MAX_WALKPATH 32

#define MAX_ASTARTPATH 128
struct astart_path_data {
	unsigned char path_len, path_pos;
	unsigned short x[MAX_ASTARTPATH];
	unsigned short y[MAX_ASTARTPATH];
};

#define MAX_PATHSTEP 128
struct step_path {
	char pathname[24];
	unsigned short x[MAX_PATHSTEP];
	unsigned short y[MAX_PATHSTEP];
	int number;
	unsigned int uid;
};

enum autowolk_type {
	PH_NONE,
	PH_WALKBACK,       /// 离开路径30格或回到路径遇到障碍时, 不再追踪, 自动行走返回路径. (默认)
	PH_WARPBACK,       /// 离开路径直到失去目标后, 传送回路径继续走完路径.
	PH_IGNOREWALK,     /// 永远不追踪目标, 但如果目标在攻击(拾取)范围内, 依然会攻击(拾取).
	PH_IGNOREALL,      /// 永远不做任何其他事情, 一路走到路点结束.
	PH_ASTARTWALK,     /// 离开路径后, 调用A*算法计算路径, 并走到最终规定的路点终点(A*算法比较消耗CPU慎用).
	PH_ASTARTWALKBACK, /// 离开路径后, 调用A*算法计算返回路径(A*算法比较消耗CPU慎用).
	PH_STAYPOS,        /// 留在某个坐标点
	PH_MAX,
};

struct autowalk_data {
	struct step_path *path;
	unsigned int step;
	unsigned short step_ai;
	unsigned short step_range;
	char eventname[EVENT_NAME_LENGTH];
};
struct step_path *get_step(const char *key);
struct step_path *path_Astart_steppath(int16 m, int16 x0, int16 y0, int16 x1, int16 y1, unsigned int uid);
void path_free(struct step_path *sp);

enum directions : int8 {
	DIR_CENTER = -1,
	DIR_NORTH = 0,
	DIR_NORTHWEST = 1,
	DIR_WEST = 2,
	DIR_SOUTHWEST = 3,
	DIR_SOUTH = 4,
	DIR_SOUTHEAST = 5,
	DIR_EAST = 6,
	DIR_NORTHEAST = 7,
	DIR_MAX
};

struct walkpath_data {
	unsigned char path_len,path_pos;
	enum directions path[MAX_WALKPATH];
};

struct shootpath_data {
	int rx,ry,len;
	int x[MAX_WALKPATH];
	int y[MAX_WALKPATH];
};

#define check_distance_bl(bl1, bl2, distance) check_distance((bl1)->x - (bl2)->x, (bl1)->y - (bl2)->y, distance)
#define check_distance_blxy(bl, x1, y1, distance) check_distance((bl)->x-(x1), (bl)->y-(y1), distance)
#define check_distance_xy(x0, y0, x1, y1, distance) check_distance((x0)-(x1), (y0)-(y1), distance)

#define distance_bl(bl1, bl2) distance((bl1)->x - (bl2)->x, (bl1)->y - (bl2)->y)
#define distance_blxy(bl, x1, y1) distance((bl)->x-(x1), (bl)->y-(y1))
#define distance_xy(x0, y0, x1, y1) distance((x0)-(x1), (y0)-(y1))

#define check_distance_client_bl(bl1, bl2, distance) check_distance_client((bl1)->x - (bl2)->x, (bl1)->y - (bl2)->y, distance)
#define check_distance_client_blxy(bl, x1, y1, distance) check_distance_client((bl)->x-(x1), (bl)->y-(y1), distance)
#define check_distance_client_xy(x0, y0, x1, y1, distance) check_distance_client((x0)-(x1), (y0)-(y1), distance)

#define distance_client_bl(bl1, bl2) distance_client((bl1)->x - (bl2)->x, (bl1)->y - (bl2)->y)
#define distance_client_blxy(bl, x1, y1) distance_client((bl)->x-(x1), (bl)->y-(y1))
#define distance_client_xy(x0, y0, x1, y1) distance_client((x0)-(x1), (y0)-(y1))

// calculates destination cell for knockback
int path_blownpos(int16 m,int16 x0,int16 y0,int16 dx,int16 dy,int count);

// tries to find a walkable path
bool path_search(struct walkpath_data *wpd,int16 m,int16 x0,int16 y0,int16 x1,int16 y1,int flag,cell_chk cell);

// tries to find a shootable path
bool path_search_long(struct shootpath_data *spd,int16 m,int16 x0,int16 y0,int16 x1,int16 y1,cell_chk cell);

// distance related functions
bool check_distance(int dx, int dy, int distance);
unsigned int distance(int dx, int dy);
bool check_distance_client(int dx, int dy, int distance);
int distance_client(int dx, int dy);

bool direction_diagonal( enum directions direction );

//
void do_init_path();
void do_final_path();

#endif /* PATH_HPP */
