// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "map.hpp"

#include <stdlib.h>
#include <math.h>

#include "../common/cbasetypes.hpp"
#include "../common/cli.hpp"
#include "../common/core.hpp"
#include "../common/ers.hpp"
#include "../common/grfio.hpp"
#include "../common/malloc.hpp"
#include "../common/nullpo.hpp"
#include "../common/random.hpp"
#include "../common/showmsg.hpp"
#include "../common/socket.hpp" // WFIFO*()
#include "../common/strlib.hpp"
#include "../common/timer.hpp"
#include "../common/utilities.hpp"
#include "../common/utils.hpp"

#include "achievement.hpp"
#include "atcommand.hpp"
#include "battle.hpp"
#include "battleground.hpp"
#include "cashshop.hpp"
#include "channel.hpp"
#include "chat.hpp"
#include "chrif.hpp"
#include "clan.hpp"
#include "clif.hpp"
#include "duel.hpp"
#include "elemental.hpp"
#include "guild.hpp"
#include "homunculus.hpp"
#include "instance.hpp"
#include "intif.hpp"
#include "log.hpp"
#include "mapreg.hpp"
#include "mercenary.hpp"
#include "mob.hpp"
#include "npc.hpp"
#include "party.hpp"
#include "path.hpp"
#include "pc.hpp"
#include "pet.hpp"
#include "quest.hpp"
#include "storage.hpp"
#include "trade.hpp"

using namespace rathena;

char default_codepage[32] = "";

int map_server_port = 3306;
char map_server_ip[64] = "127.0.0.1";
char map_server_id[32] = "ragnarok";
char map_server_pw[32] = "";
char map_server_db[32] = "ragnarok";
Sql* mmysql_handle;
Sql* qsmysql_handle; /// For query_sql

int db_use_sqldbs = 0;
char buyingstores_table[32] = "buyingstores";
char buyingstore_items_table[32] = "buyingstore_items";
char item_cash_table[32] = "item_cash_db";
char item_cash2_table[32] = "item_cash_db2";
#ifdef RENEWAL
char item_table[32] = "item_db_re";
char item2_table[32] = "item_db2_re";
char mob_table[32] = "mob_db_re";
char mob2_table[32] = "mob_db2_re";
char mob_skill_table[32] = "mob_skill_db_re";
char mob_skill2_table[32] = "mob_skill_db2_re";
#else
char item_table[32] = "item_db";
char item2_table[32] = "item_db2";
char mob_table[32] = "mob_db";
char mob2_table[32] = "mob_db2";
char mob_skill_table[32] = "mob_skill_db";
char mob_skill2_table[32] = "mob_skill_db2";
#endif
char sales_table[32] = "sales";
char vendings_table[32] = "vendings";
char vending_items_table[32] = "vending_items";
char market_table[32] = "market";
char roulette_table[32] = "db_roulette";
char guild_storage_log_table[32] = "guild_storage_log";

// log database
char log_db_ip[64] = "127.0.0.1";
int log_db_port = 3306;
char log_db_id[32] = "ragnarok";
char log_db_pw[32] = "";
char log_db_db[32] = "log";
Sql* logmysql_handle;

// DBMap declaration
static DBMap* id_db=NULL; /// int id -> struct block_list*
static DBMap* pc_db=NULL; /// int id -> struct map_session_data*
static DBMap* bot_db = NULL;
static DBMap* hombot_db = NULL;
static DBMap* mobid_db=NULL; /// int id -> struct mob_data*
static DBMap* bossid_db=NULL; /// int id -> struct mob_data* (MVP db)
static DBMap* map_db=NULL; /// unsigned int mapindex -> struct map_data*
static DBMap* nick_db=NULL; /// uint32 char_id -> struct charid2nick* (requested names of offline characters)
static DBMap* charid_db=NULL; /// uint32 char_id -> struct map_session_data*
static DBMap* regen_db=NULL; /// int id -> struct block_list* (status_natural_heal processing)
static DBMap* map_msg_db=NULL;

static int map_users=0;

#define BLOCK_SIZE 8
#define block_free_max 1048576
struct block_list *block_free[block_free_max];
static int block_free_count = 0, block_free_lock = 0;

#define BL_LIST_MAX 1048576
static struct block_list *bl_list[BL_LIST_MAX];
static int bl_list_count = 0;

#ifndef MAP_MAX_MSG
	#define MAP_MAX_MSG 2500
#endif

struct map_data map[MAX_MAP_PER_SERVER];
int map_num = 0;

int map_port=0;

int autosave_interval = DEFAULT_AUTOSAVE_INTERVAL;
int minsave_interval = 100;
int16 save_settings = CHARSAVE_ALL;
bool agit_flag = false;
bool agit2_flag = false;
bool agit3_flag = false;
int night_flag = 0; // 0=day, 1=night [Yor]

struct charid_request {
	struct charid_request* next;
	int charid;// who want to be notified of the nick
};
struct charid2nick {
	char nick[NAME_LENGTH];
	struct charid_request* requests;// requests of notification on this nick
};

// This is the main header found at the very beginning of the map cache
struct map_cache_main_header {
	uint32 file_size;
	uint16 map_count;
};

// This is the header appended before every compressed map cells info in the map cache
struct map_cache_map_info {
	char name[MAP_NAME_LENGTH];
	int16 xs;
	int16 ys;
	int32 len;
};

char motd_txt[256] = CONF_PATH "/motd.txt";
char charhelp_txt[256] = CONF_PATH "/charhelp.txt";
char channel_conf[256] = CONF_PATH "/channels.conf";

const char *MSG_CONF_NAME_RUS;
const char *MSG_CONF_NAME_SPN;
const char *MSG_CONF_NAME_GRM;
const char *MSG_CONF_NAME_CHN;
const char *MSG_CONF_NAME_MAL;
const char *MSG_CONF_NAME_IDN;
const char *MSG_CONF_NAME_FRN;
const char *MSG_CONF_NAME_POR;
const char *MSG_CONF_NAME_THA;

char wisp_server_name[NAME_LENGTH] = "Server"; // can be modified in char-server configuration file

struct s_map_default map_default;

int console = 0;
int enable_spy = 0; //To enable/disable @spy commands, which consume too much cpu time when sending packets. [Skotlex]
int enable_grf = 0;	//To enable/disable reading maps from GRF files, bypassing mapcache [blackhole89]

/**
 * Get the map data
 * @param mapid: Map ID to lookup
 * @return map_data on success or nullptr on failure
 */
struct map_data *map_getmapdata(int16 mapid)
{
	if (mapid < 0 || mapid >= MAX_MAP_PER_SERVER)
		return nullptr;

	return &map[mapid];
}

struct map_data *map_mapdata_exists_(int16 mapid, const char *func)
{
	if (mapid < 0 || mapid >= MAX_MAP_PER_SERVER)
		return nullptr;

	struct map_data *mapdata = &map[mapid];

	if (mapdata == nullptr) {
		ShowError("%s: Try to reach a null map (mapid: %d)\n", func, mapid);
		return nullptr;
	}

	if (mapdata->m != mapid || mapdata->name[0] == '\0' || mapdata->xs == 0 || mapdata->ys == 0)
		return nullptr;

	if (mapdata->block == nullptr || mapdata->block_mob == nullptr) {
		ShowError("%s: Map data block is free param (map: \"%s\")\n", func, mapdata->name);
		return nullptr;
	}

	return mapdata;
}

/*==========================================
 * server player count (of all mapservers)
 *------------------------------------------*/
void map_setusers(int users)
{
	map_users = users;
}

int map_getusers(void)
{
	return map_users;
}

/*==========================================
 * server player count (this mapserver only)
 *------------------------------------------*/
int map_usercount(void)
{
	return pc_db->size(pc_db);
}


/*==========================================
 * Attempt to free a map blocklist
 *------------------------------------------*/
int map_freeblock (struct block_list *bl)
{
	nullpo_retr(block_free_lock, bl);
	if (block_free_lock == 0 || block_free_count >= block_free_max)
	{
		aFree(bl);
		bl = NULL;
		if (block_free_count >= block_free_max)
			ShowWarning("map_freeblock: too many free block! %d %d\n", block_free_count, block_free_lock);
	} else
		block_free[block_free_count++] = bl;

	return block_free_lock;
}
/*==========================================
 * Lock blocklist, (prevent map_freeblock usage)
 *------------------------------------------*/
int map_freeblock_lock (void)
{
	return ++block_free_lock;
}

/*==========================================
 * Remove the lock on map_bl
 *------------------------------------------*/
int map_freeblock_unlock (void)
{
	if ((--block_free_lock) == 0) {
		int i;
		for (i = 0; i < block_free_count; i++)
		{
			aFree(block_free[i]);
			block_free[i] = NULL;
		}
		block_free_count = 0;
	} else if (block_free_lock < 0) {
		ShowError("map_freeblock_unlock: lock count < 0 !\n");
		block_free_lock = 0;
	}

	return block_free_lock;
}

// Timer function to check if there some remaining lock and remove them if so.
// Called each 1s
TIMER_FUNC(map_freeblock_timer){
	if (block_free_lock > 0) {
		ShowError("map_freeblock_timer: block_free_lock(%d) is invalid.\n", block_free_lock);
		block_free_lock = 1;
		map_freeblock_unlock();
	}

	return 0;
}

//
// blocklist
//
/*==========================================
 * Handling of map_bl[]
 * The address of bl_heal is set in bl->prev
 *------------------------------------------*/
static struct block_list bl_head;

#ifdef CELL_NOSTACK
/*==========================================
 * These pair of functions update the counter of how many objects
 * lie on a tile.
 *------------------------------------------*/
static void map_addblcell(struct block_list *bl)
{
	struct map_data *mapdata = map_getmapdata(bl->m);

	if( bl->m<0 || bl->x<0 || bl->x>=mapdata->xs || bl->y<0 || bl->y>=mapdata->ys || !(bl->type&BL_CHAR) )
		return;
	mapdata->cell[bl->x+bl->y*mapdata->xs].cell_bl++;
	return;
}

static void map_delblcell(struct block_list *bl)
{
	struct map_data *mapdata = map_getmapdata(bl->m);

	if( bl->m <0 || bl->x<0 || bl->x>=mapdata->xs || bl->y<0 || bl->y>=mapdata->ys || !(bl->type&BL_CHAR) )
		return;
	mapdata->cell[bl->x+bl->y*mapdata->xs].cell_bl--;
}
#endif

/*==========================================
 * Adds a block to the map.
 * Returns 0 on success, 1 on failure (illegal coordinates).
 *------------------------------------------*/
int map_addblock(struct block_list* bl)
{
	int16 m, x, y;
	int pos;

	nullpo_ret(bl);

	if (bl->prev != NULL) {
		ShowError("map_addblock: bl->prev != NULL\n");
		return 1;
	}

	m = bl->m;
	x = bl->x;
	y = bl->y;

	if( m < 0 )
	{
		ShowError("map_addblock: invalid map id (%d), only %d are loaded.\n", m, map_num);
		return 1;
	}

	struct map_data *mapdata = map_getmapdata(m);

	if (mapdata == nullptr) {
		ShowError("map_addblock: Try to warp to a null map (mapid: %d,x: %d,y: %d)\n", m, x, y);
		return 1;
	}

	if( x < 0 || x >= mapdata->xs || y < 0 || y >= mapdata->ys )
	{
		ShowError("map_addblock: out-of-bounds coordinates (\"%s\",%d,%d), map is %dx%d\n", mapdata->name, x, y, mapdata->xs, mapdata->ys);
		return 1;
	}
	
	if (mapdata->instance_src_map && mapdata->name[0] == '\0')
	{
		ShowError("map_addblock: Try to warp to a freed instance map (src map: \"%s\")\n", map_getmapdata(mapdata->instance_src_map)->name);
		return 1;
	}

	if (mapdata->block == nullptr || (bl->type == BL_MOB && mapdata->block_mob == nullptr)) {
		ShowError("map_addblock: Map data block is free param (map: \"%s\")\n", mapdata->name);
		return 1;
	}

	pos = x/BLOCK_SIZE+(y/BLOCK_SIZE)*mapdata->bxs;

	if (bl->type == BL_MOB) {
		bl->next = mapdata->block_mob[pos];
		bl->prev = &bl_head;
		if (bl->next) bl->next->prev = bl;
		mapdata->block_mob[pos] = bl;
	} else {
		bl->next = mapdata->block[pos];
		bl->prev = &bl_head;
		if (bl->next) bl->next->prev = bl;
		mapdata->block[pos] = bl;
	}

#ifdef CELL_NOSTACK
	map_addblcell(bl);
#endif

	return 0;
}

/*==========================================
 * Removes a block from the map.
 *------------------------------------------*/
int map_delblock(struct block_list* bl)
{
	int pos;
	nullpo_ret(bl);

	// blocklist (2ways chainlist)
	if (bl->prev == NULL) {
		if (bl->next != NULL) {
			// can't delete block (already at the beginning of the chain)
			ShowError("map_delblock error : bl->next!=NULL\n");
		}
		return 0;
	}

	struct map_data *mapdata = map_getmapdata(bl->m);

	if (mapdata == nullptr) {
		ShowError("map_delblock: Try to warp to a null map (mapid: %d,x: %d,y: %d)\n", bl->m, bl->x, bl->y);
		return 0;
	}

	if (bl->x < 0 || bl->x >= mapdata->xs || bl->y < 0 || bl->y >= mapdata->ys)
	{
		ShowError("map_delblock: out-of-bounds coordinates (\"%s\",%d,%d), map is %dx%d\n", mapdata->name, bl->x, bl->y, mapdata->xs, mapdata->ys);
		return 0;
	}

	if (mapdata->instance_src_map && mapdata->name[0] == '\0')
	{
		ShowError("map_delblock: Try to warp to a freed instance map (src map: \"%s\")\n", map_getmapdata(mapdata->instance_src_map)->name);
		return 0;
	}

	if (mapdata->block == nullptr || (bl->type == BL_MOB && mapdata->block_mob == nullptr)) {
		ShowError("map_delblock: Map data block is free param (map: \"%s\")\n", mapdata->name);
		return 0;
	}

#ifdef CELL_NOSTACK
	map_delblcell(bl);
#endif

	pos = bl->x/BLOCK_SIZE+(bl->y/BLOCK_SIZE)*mapdata->bxs;

	if (bl->next)
		bl->next->prev = bl->prev;
	if (bl->prev == &bl_head) {
	//Since the head of the list, update the block_list map of []
		if (bl->type == BL_MOB) {
			mapdata->block_mob[pos] = bl->next;
		} else {
			mapdata->block[pos] = bl->next;
		}
	} else {
		bl->prev->next = bl->next;
	}
	bl->next = NULL;
	bl->prev = NULL;

	return 0;
}

/**
 * Moves a block a x/y target position. [Skotlex]
 * Pass flag as 1 to prevent doing skill_unit_move checks
 * (which are executed by default on BL_CHAR types)
 * @param bl : block(object) to move
 * @param x1 : new x position
 * @param y1 : new y position
 * @param tick : when this was scheduled
 * @return 0:success, 1:fail
 */
int map_moveblock(struct block_list *bl, int x1, int y1, t_tick tick)
{
	int x0 = bl->x, y0 = bl->y;
	struct status_change *sc = NULL;
	int moveblock = ( x0/BLOCK_SIZE != x1/BLOCK_SIZE || y0/BLOCK_SIZE != y1/BLOCK_SIZE);

	if (!bl->prev) {
		//Block not in map, just update coordinates, but do naught else.
		bl->x = x1;
		bl->y = y1;
		return 0;
	}

	struct map_data *mapdata = map_getmapdata(bl->m);

	if (x1 < 0 || y1 < 0 || (x1 >= mapdata->xs) || (y1 >= mapdata->ys))
		return 0;

	//TODO: Perhaps some outs of bounds checking should be placed here?
	if (bl->type&BL_CHAR) {
		sc = status_get_sc(bl);

		skill_unit_move(bl,tick,2);
		if ( sc && sc->count ) //at least one to cancel
		{
			status_change_end(bl, SC_CLOSECONFINE, INVALID_TIMER);
			status_change_end(bl, SC_CLOSECONFINE2, INVALID_TIMER);
			status_change_end(bl, SC_TINDER_BREAKER, INVALID_TIMER);
			status_change_end(bl, SC_TINDER_BREAKER2, INVALID_TIMER);
	//		status_change_end(bl, SC_BLADESTOP, INVALID_TIMER); //Won't stop when you are knocked away, go figure...
			status_change_end(bl, SC_TATAMIGAESHI, INVALID_TIMER);
			status_change_end(bl, SC_MAGICROD, INVALID_TIMER);
			status_change_end(bl, SC_SU_STOOP, INVALID_TIMER);
			if (sc->data[SC_PROPERTYWALK] &&
				sc->data[SC_PROPERTYWALK]->val3 >= skill_get_maxcount(sc->data[SC_PROPERTYWALK]->val1,sc->data[SC_PROPERTYWALK]->val2) )
				status_change_end(bl,SC_PROPERTYWALK,INVALID_TIMER);
		}
	} else
	if (bl->type == BL_NPC)
		npc_unsetcells((TBL_NPC*)bl);

	if (moveblock) map_delblock(bl);
#ifdef CELL_NOSTACK
	else map_delblcell(bl);
#endif
	bl->x = x1;
	bl->y = y1;
	if (moveblock) {
		if(map_addblock(bl))
			return 1;
	}
#ifdef CELL_NOSTACK
	else map_addblcell(bl);
#endif

	if (bl->type&BL_CHAR) {

		skill_unit_move(bl,tick,3);

		if( bl->type == BL_PC && ((TBL_PC*)bl)->shadowform_id ) {//Shadow Form Target Moving
			struct block_list *d_bl;
			if( (d_bl = map_id2bl(((TBL_PC*)bl)->shadowform_id)) == NULL || !check_distance_bl(bl,d_bl,10) ) {
				if( d_bl )
					status_change_end(d_bl,SC__SHADOWFORM,INVALID_TIMER);
				((TBL_PC*)bl)->shadowform_id = 0;
			}
		}

		if (sc && sc->count) {
			if (sc->data[SC_DANCING])
				skill_unit_move_unit_group(skill_id2group(sc->data[SC_DANCING]->val2), bl->m, x1-x0, y1-y0);
			else {
				if (sc->data[SC_CLOAKING] && sc->data[SC_CLOAKING]->val1 < 3 && !skill_check_cloaking(bl, NULL))
					status_change_end(bl, SC_CLOAKING, INVALID_TIMER);
				if (sc->data[SC_WARM])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_WARM]->val4), bl->m, x1-x0, y1-y0);
				if (sc->data[SC_BANDING])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_BANDING]->val4), bl->m, x1-x0, y1-y0);

				if (sc->data[SC_NEUTRALBARRIER_MASTER])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_NEUTRALBARRIER_MASTER]->val2), bl->m, x1-x0, y1-y0);
				else if (sc->data[SC_STEALTHFIELD_MASTER])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_STEALTHFIELD_MASTER]->val2), bl->m, x1-x0, y1-y0);

				if( sc->data[SC__SHADOWFORM] ) {//Shadow Form Caster Moving
					struct block_list *d_bl;
					if( (d_bl = map_id2bl(sc->data[SC__SHADOWFORM]->val2)) == NULL || !check_distance_bl(bl,d_bl,10) )
						status_change_end(bl,SC__SHADOWFORM,INVALID_TIMER);
				}

				if (sc->data[SC_PROPERTYWALK]
					&& sc->data[SC_PROPERTYWALK]->val3 < skill_get_maxcount(sc->data[SC_PROPERTYWALK]->val1,sc->data[SC_PROPERTYWALK]->val2)
					&& map_find_skill_unit_oncell(bl,bl->x,bl->y,SO_ELECTRICWALK,NULL,0) == NULL
					&& map_find_skill_unit_oncell(bl,bl->x,bl->y,SO_FIREWALK,NULL,0) == NULL
					&& skill_unitsetting(bl,sc->data[SC_PROPERTYWALK]->val1,sc->data[SC_PROPERTYWALK]->val2,x0, y0,0)) {
						sc->data[SC_PROPERTYWALK]->val3++;
				}


			}
			/* Guild Aura Moving */
			if( bl->type == BL_PC && ((TBL_PC*)bl)->state.gmaster_flag ) {
				if (sc->data[SC_LEADERSHIP])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_LEADERSHIP]->val4), bl->m, x1-x0, y1-y0);
				if (sc->data[SC_GLORYWOUNDS])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_GLORYWOUNDS]->val4), bl->m, x1-x0, y1-y0);
				if (sc->data[SC_SOULCOLD])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_SOULCOLD]->val4), bl->m, x1-x0, y1-y0);
				if (sc->data[SC_HAWKEYES])
					skill_unit_move_unit_group(skill_id2group(sc->data[SC_HAWKEYES]->val4), bl->m, x1-x0, y1-y0);
			}
		}
	} else
	if (bl->type == BL_NPC)
		npc_setcells((TBL_NPC*)bl);

	return 0;
}

/*==========================================
 * Counts specified number of objects on given cell.
 * flag:
 *		0x1 - only count standing units
 *------------------------------------------*/
int map_count_oncell(int16 m, int16 x, int16 y, int type, int flag)
{
	int bx,by;
	struct block_list *bl;
	int count = 0;
	struct map_data *mapdata = map_getmapdata(m);

	if (x < 0 || y < 0 || (x >= mapdata->xs) || (y >= mapdata->ys))
		return 0;

	bx = x/BLOCK_SIZE;
	by = y/BLOCK_SIZE;

	if (type&~BL_MOB)
		for( bl = mapdata->block[bx+by*mapdata->bxs] ; bl != NULL ; bl = bl->next )
			if(bl->x == x && bl->y == y && bl->type&type) {
				if(flag&1) {
					struct unit_data *ud = unit_bl2ud(bl);
					if(!ud || ud->walktimer == INVALID_TIMER)
						count++;
				} else {
					count++;
				}
			}

	if (type&BL_MOB)
		for( bl = mapdata->block_mob[bx+by*mapdata->bxs] ; bl != NULL ; bl = bl->next )
			if(bl->x == x && bl->y == y) {
				if(flag&1) {
					struct unit_data *ud = unit_bl2ud(bl);
					if(!ud || ud->walktimer == INVALID_TIMER)
						count++;
				} else {
					count++;
				}
			}

	return count;
}

/*
 * Looks for a skill unit on a given cell
 * flag&1: runs battle_check_target check based on unit->group->target_flag
 */
struct skill_unit* map_find_skill_unit_oncell(struct block_list* target,int16 x,int16 y,uint16 skill_id,struct skill_unit* out_unit, int flag) {
	int16 bx,by;
	struct block_list *bl;
	struct skill_unit *unit;
	struct map_data *mapdata = map_getmapdata(target->m);

	if (x < 0 || y < 0 || (x >= mapdata->xs) || (y >= mapdata->ys))
		return NULL;

	bx = x/BLOCK_SIZE;
	by = y/BLOCK_SIZE;

	for( bl = mapdata->block[bx+by*mapdata->bxs] ; bl != NULL ; bl = bl->next )
	{
		if (bl->x != x || bl->y != y || bl->type != BL_SKILL)
			continue;

		unit = (struct skill_unit *) bl;
		if( unit == out_unit || !unit->alive || !unit->group || unit->group->skill_id != skill_id )
			continue;
		if( !(flag&1) || battle_check_target(&unit->bl,target,unit->group->target_flag) > 0 )
			return unit;
	}
	return NULL;
}

/*==========================================
 * Adapted from foreachinarea for an easier invocation. [Skotlex]
 *------------------------------------------*/
int map_foreachinrangeV(int (*func)(struct block_list*,va_list),struct block_list* center, int16 range, int type, va_list ap, bool wall_check)
{
	int bx, by, m;
	int returnCount = 0;	//total sum of returned values of func() [Skotlex]
	struct block_list *bl;
	int blockcount = bl_list_count, i;
	int x0, x1, y0, y1;
	va_list ap_copy;

	m = center->m;
	if( m < 0 )
		return 0;

	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	x0 = i16max(center->x - range, 0);
	y0 = i16max(center->y - range, 0);
	x1 = i16min(center->x + range, mapdata->xs - 1);
	y1 = i16min(center->y + range, mapdata->ys - 1);

	if ( type&~BL_MOB ) {
		for( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ ) {
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ ) {
				for(bl = mapdata->block[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
					if( bl->type&type
						&& bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1
#ifdef CIRCULAR_AREA
						&& check_distance_bl(center, bl, range)
#endif
						&& ( !wall_check || path_search_long(NULL, center->m, center->x, center->y, bl->x, bl->y, CELL_CHKWALL) )
					  	&& bl_list_count < BL_LIST_MAX )
						bl_list[ bl_list_count++ ] = bl;
				}
			}
		}
	}

	if ( type&BL_MOB ) {
		for( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ ) {
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ ) {
				for(bl = mapdata->block_mob[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
					if( bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1
#ifdef CIRCULAR_AREA
						&& check_distance_bl(center, bl, range)
#endif
						&& ( !wall_check || path_search_long(NULL, center->m, center->x, center->y, bl->x, bl->y, CELL_CHKWALL) )
					  	&& bl_list_count < BL_LIST_MAX )
						bl_list[ bl_list_count++ ] = bl;
				}
			}
		}
	}

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_foreachinrange: block count too many!\n");

	map_freeblock_lock();

	for( i = blockcount; i < bl_list_count; i++ ) {
		if( bl_list[ i ]->prev ) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_copy(ap_copy, ap);
			returnCount += func(bl_list[i], ap_copy);
			va_end(ap_copy);
		}
	}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;	//[Skotlex]
}

int map_foreachinrange(int (*func)(struct block_list*,va_list), struct block_list* center, int16 range, int type, ...)
{
	int returnCount = 0;
	va_list ap;
 	va_start(ap,type);
	returnCount = map_foreachinrangeV(func,center,range,type,ap,battle_config.skill_wall_check>0);
 	va_end(ap);
	return returnCount;
}

int map_foreachinallrange(int (*func)(struct block_list*,va_list), struct block_list* center, int16 range, int type, ...)
{
	int returnCount = 0;
	va_list ap;
 	va_start(ap,type);
	returnCount = map_foreachinrangeV(func,center,range,type,ap,false);
 	va_end(ap);
	return returnCount;
}

/*==========================================
 * Same as foreachinrange, but there must be a shoot-able range between center and target to be counted in. [Skotlex]
 *------------------------------------------*/
int map_foreachinshootrange(int (*func)(struct block_list*,va_list),struct block_list* center, int16 range, int type,...)
{
	int returnCount = 0;
	va_list ap;
 	va_start(ap,type);
	returnCount = map_foreachinrangeV(func,center,range,type,ap,true);
 	va_end(ap);
	return returnCount;
}


/*========================================== [Playtester]
 * range = map m (x0,y0)-(x1,y1)
 * Apply *func with ... arguments for the range.
 * @param m: ID of map
 * @param x0: West end of area
 * @param y0: South end of area
 * @param x1: East end of area
 * @param y1: North end of area
 * @param type: Type of bl to search for
*------------------------------------------*/
int map_foreachinareaV(int(*func)(struct block_list*, va_list), int16 m, int16 x0, int16 y0, int16 x1, int16 y1, int type, va_list ap, bool wall_check)
{
	int bx, by, cx, cy;
	int returnCount = 0;	//total sum of returned values of func()
	struct block_list *bl;
	int blockcount = bl_list_count, i;
	va_list ap_copy;

	if (m < 0)
		return 0;

	if (x1 < x0)
		SWAP(x0, x1);
	if (y1 < y0)
		SWAP(y0, y1);

	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	x0 = i16max(x0, 0);
	y0 = i16max(y0, 0);
	x1 = i16min(x1, mapdata->xs - 1);
	y1 = i16min(y1, mapdata->ys - 1);

	if( wall_check ) {
		cx = x0 + (x1 - x0) / 2;
		cy = y0 + (y1 - y0) / 2;
	}

	if( type&~BL_MOB ) {
		for (by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++) {
			for (bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++) {
				for(bl = mapdata->block[bx + by * mapdata->bxs]; bl != NULL; bl = bl->next) {
					if ( bl->type&type
						&& bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1
						&& ( !wall_check || path_search_long(NULL, m, cx, cy, bl->x, bl->y, CELL_CHKWALL) )
						&& bl_list_count < BL_LIST_MAX )
						bl_list[bl_list_count++] = bl;
				}
			}
		}
	}

	if( type&BL_MOB ) {
		for (by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++) {
			for (bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++) {
				for(bl = mapdata->block_mob[bx + by * mapdata->bxs]; bl != NULL; bl = bl->next) {
					if ( bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1
						&& ( !wall_check || path_search_long(NULL, m, cx, cy, bl->x, bl->y, CELL_CHKWALL) )
						&& bl_list_count < BL_LIST_MAX )
						bl_list[bl_list_count++] = bl;
				}
			}
		}
	}

	if (bl_list_count >= BL_LIST_MAX)
		ShowWarning("map_foreachinarea: block count too many!\n");

	map_freeblock_lock();

	for (i = blockcount; i < bl_list_count; i++) {
		if (bl_list[i]->prev) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_copy(ap_copy, ap);
			returnCount += func(bl_list[i], ap_copy);
			va_end(ap_copy);
		}
	}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;
}

int map_foreachinallarea(int (*func)(struct block_list*,va_list), int16 m, int16 x0, int16 y0, int16 x1, int16 y1, int type, ...)
{
	int returnCount = 0;
	va_list ap;
 	va_start(ap,type);
	returnCount = map_foreachinareaV(func,m,x0,y0,x1,y1,type,ap,false);
 	va_end(ap);
	return returnCount;
}

int map_foreachinshootarea(int(*func)(struct block_list*, va_list), int16 m, int16 x0, int16 y0, int16 x1, int16 y1, int type, ...)
{
	int returnCount = 0;
	va_list ap;
 	va_start(ap,type);
	returnCount = map_foreachinareaV(func,m,x0,y0,x1,y1,type,ap,true);
 	va_end(ap);
	return returnCount;
}
int map_foreachinarea(int(*func)(struct block_list*, va_list), int16 m, int16 x0, int16 y0, int16 x1, int16 y1, int type, ...)
{
	int returnCount = 0;
	va_list ap;
 	va_start(ap,type);
	returnCount = map_foreachinareaV(func,m,x0,y0,x1,y1,type,ap,battle_config.skill_wall_check>0);
 	va_end(ap);
	return returnCount;
}

/*==========================================
 * Adapted from forcountinarea for an easier invocation. [pakpil]
 *------------------------------------------*/
int map_forcountinrange(int (*func)(struct block_list*,va_list), struct block_list* center, int16 range, int count, int type, ...)
{
	int bx, by, m;
	int returnCount = 0;	//total sum of returned values of func() [Skotlex]
	struct block_list *bl;
	int blockcount = bl_list_count, i;
	int x0, x1, y0, y1;
	struct map_data *mapdata;
	va_list ap;

	m = center->m;
	mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	x0 = i16max(center->x - range, 0);
	y0 = i16max(center->y - range, 0);
	x1 = i16min(center->x + range, mapdata->xs - 1);
	y1 = i16min(center->y + range, mapdata->ys - 1);

	if ( type&~BL_MOB )
		for ( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ ) {
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ ) {
				for( bl = mapdata->block[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
					if( bl->type&type
						&& bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1
#ifdef CIRCULAR_AREA
						&& check_distance_bl(center, bl, range)
#endif
					  	&& bl_list_count < BL_LIST_MAX )
						bl_list[ bl_list_count++ ] = bl;
				}
			}
		}
	if( type&BL_MOB )
		for( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ ) {
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ ){
				for( bl = mapdata->block_mob[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
					if( bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1
#ifdef CIRCULAR_AREA
						&& check_distance_bl(center, bl, range)
#endif
						&& bl_list_count < BL_LIST_MAX )
						bl_list[ bl_list_count++ ] = bl;
				}
			}
		}

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_forcountinrange: block count too many!\n");

	map_freeblock_lock();

	for( i = blockcount; i < bl_list_count; i++ )
		if( bl_list[ i ]->prev ) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_start(ap, type);
			returnCount += func(bl_list[ i ], ap);
			va_end(ap);
			if( count && returnCount >= count )
				break;
		}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;	//[Skotlex]
}
int map_forcountinarea(int (*func)(struct block_list*,va_list), int16 m, int16 x0, int16 y0, int16 x1, int16 y1, int count, int type, ...)
{
	int bx, by;
	int returnCount = 0;	//total sum of returned values of func() [Skotlex]
	struct block_list *bl;
	int blockcount = bl_list_count, i;
	va_list ap;

	if ( m < 0 )
		return 0;

	if ( x1 < x0 )
		SWAP(x0, x1);
	if ( y1 < y0 )
		SWAP(y0, y1);

	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	x0 = i16max(x0, 0);
	y0 = i16max(y0, 0);
	x1 = i16min(x1, mapdata->xs - 1);
	y1 = i16min(y1, mapdata->ys - 1);

	if ( type&~BL_MOB )
		for( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ )
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ )
				for( bl = mapdata->block[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next )
					if( bl->type&type && bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1 && bl_list_count < BL_LIST_MAX )
						bl_list[ bl_list_count++ ] = bl;

	if( type&BL_MOB )
		for( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ )
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ )
				for( bl = mapdata->block_mob[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next )
					if( bl->x >= x0 && bl->x <= x1 && bl->y >= y0 && bl->y <= y1 && bl_list_count < BL_LIST_MAX )
						bl_list[ bl_list_count++ ] = bl;

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_forcountinarea: block count too many!\n");

	map_freeblock_lock();

	for( i = blockcount; i < bl_list_count; i++ )
		if(bl_list[ i ]->prev) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_start(ap, type);
			returnCount += func(bl_list[ i ], ap);
			va_end(ap);
			if( count && returnCount >= count )
				break;
		}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;	//[Skotlex]
}

/*==========================================
 * Move bl and do func* with va_list while moving.
 * Movement is set by dx dy which are distance in x and y
 *------------------------------------------*/
int map_foreachinmovearea(int (*func)(struct block_list*,va_list), struct block_list* center, int16 range, int16 dx, int16 dy, int type, ...)
{
	int bx, by, m;
	int returnCount = 0;  //total sum of returned values of func() [Skotlex]
	struct block_list *bl;
	int blockcount = bl_list_count, i;
	int16 x0, x1, y0, y1;
	va_list ap;

	if ( !range ) return 0;
	if ( !dx && !dy ) return 0; //No movement.

	m = center->m;

	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	x0 = center->x - range;
	x1 = center->x + range;
	y0 = center->y - range;
	y1 = center->y + range;

	if ( x1 < x0 )
		SWAP(x0, x1);
	if ( y1 < y0 )
		SWAP(y0, y1);

	if( dx == 0 || dy == 0 ) {
		//Movement along one axis only.
		if( dx == 0 ){
			if( dy < 0 ) //Moving south
				y0 = y1 + dy + 1;
			else //North
				y1 = y0 + dy - 1;
		} else { //dy == 0
			if( dx < 0 ) //West
				x0 = x1 + dx + 1;
			else //East
				x1 = x0 + dx - 1;
		}

		x0 = i16max(x0, 0);
		y0 = i16max(y0, 0);
		x1 = i16min(x1, mapdata->xs - 1);
		y1 = i16min(y1, mapdata->ys - 1);

		for( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ ) {
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ ) {
				if ( type&~BL_MOB ) {
					for( bl = mapdata->block[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
						if( bl->type&type &&
							bl->x >= x0 && bl->x <= x1 &&
							bl->y >= y0 && bl->y <= y1 &&
							bl_list_count < BL_LIST_MAX )
							bl_list[ bl_list_count++ ] = bl;
					}
				}
				if ( type&BL_MOB ) {
					for( bl = mapdata->block_mob[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
						if( bl->x >= x0 && bl->x <= x1 &&
							bl->y >= y0 && bl->y <= y1 &&
							bl_list_count < BL_LIST_MAX )
							bl_list[ bl_list_count++ ] = bl;
					}
				}
			}
		}
	} else { // Diagonal movement
		x0 = i16max(x0, 0);
		y0 = i16max(y0, 0);
		x1 = i16min(x1, mapdata->xs - 1);
		y1 = i16min(y1, mapdata->ys - 1);

		for( by = y0 / BLOCK_SIZE; by <= y1 / BLOCK_SIZE; by++ ) {
			for( bx = x0 / BLOCK_SIZE; bx <= x1 / BLOCK_SIZE; bx++ ) {
				if ( type & ~BL_MOB ) {
					for( bl = mapdata->block[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
						if( bl->type&type &&
							bl->x >= x0 && bl->x <= x1 &&
							bl->y >= y0 && bl->y <= y1 &&
							bl_list_count < BL_LIST_MAX )
						if( ( dx > 0 && bl->x < x0 + dx) ||
							( dx < 0 && bl->x > x1 + dx) ||
							( dy > 0 && bl->y < y0 + dy) ||
							( dy < 0 && bl->y > y1 + dy) )
							bl_list[ bl_list_count++ ] = bl;
					}
				}
				if ( type&BL_MOB ) {
					for( bl = mapdata->block_mob[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
						if( bl->x >= x0 && bl->x <= x1 &&
							bl->y >= y0 && bl->y <= y1 &&
							bl_list_count < BL_LIST_MAX)
						if( ( dx > 0 && bl->x < x0 + dx) ||
							( dx < 0 && bl->x > x1 + dx) ||
							( dy > 0 && bl->y < y0 + dy) ||
							( dy < 0 && bl->y > y1 + dy) )
							bl_list[ bl_list_count++ ] = bl;
					}
				}
			}
		}

	}

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_foreachinmovearea: block count too many!\n");

	map_freeblock_lock();	// Prohibit the release from memory

	for( i = blockcount; i < bl_list_count; i++ )
		if( bl_list[ i ]->prev ) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_start(ap, type);
			returnCount += func(bl_list[ i ], ap);
			va_end(ap);
		}

	map_freeblock_unlock();	// Allow Free

	bl_list_count = blockcount;
	return returnCount;
}

// -- moonsoul	(added map_foreachincell which is a rework of map_foreachinarea but
//			 which only checks the exact single x/y passed to it rather than an
//			 area radius - may be more useful in some instances)
//
int map_foreachincell(int (*func)(struct block_list*,va_list), int16 m, int16 x, int16 y, int type, ...)
{
	int bx, by;
	int returnCount = 0;  //total sum of returned values of func() [Skotlex]
	struct block_list *bl;
	int blockcount = bl_list_count, i;
	struct map_data *mapdata = map_mapdata_exists(m);
	va_list ap;

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	if ( x < 0 || y < 0 || x >= mapdata->xs || y >= mapdata->ys ) return 0;

	by = y / BLOCK_SIZE;
	bx = x / BLOCK_SIZE;

	if( type&~BL_MOB )
		for( bl = mapdata->block[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next )
			if( bl->type&type && bl->x == x && bl->y == y && bl_list_count < BL_LIST_MAX )
				bl_list[ bl_list_count++ ] = bl;
	if( type&BL_MOB )
		for( bl = mapdata->block_mob[ bx + by * mapdata->bxs]; bl != NULL; bl = bl->next )
			if( bl->x == x && bl->y == y && bl_list_count < BL_LIST_MAX)
				bl_list[ bl_list_count++ ] = bl;

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_foreachincell: block count too many!\n");

	map_freeblock_lock();

	for( i = blockcount; i < bl_list_count; i++ )
		if( bl_list[ i ]->prev ) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_start(ap, type);
			returnCount += func(bl_list[ i ], ap);
			va_end(ap);
		}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;
}

/*============================================================
* For checking a path between two points (x0, y0) and (x1, y1)
*------------------------------------------------------------*/
int map_foreachinpath(int (*func)(struct block_list*,va_list),int16 m,int16 x0,int16 y0,int16 x1,int16 y1,int16 range,int length, int type,...)
{
	int returnCount = 0;  //total sum of returned values of func() [Skotlex]
//////////////////////////////////////////////////////////////
//
// sharp shooting 3 [Skotlex]
//
//////////////////////////////////////////////////////////////
// problem:
// Same as Sharp Shooting 1. Hits all targets within range of
// the line.
// (t1,t2 t3 and t4 get hit)
//
//     target 1
//      x t4
//     t2
// t3 x
//   x
//  S
//////////////////////////////////////////////////////////////
// Methodology:
// My trigonometrics and math are a little rusty... so the approach I am writing
// here is basically do a double for to check for all targets in the square that
// contains the initial and final positions (area range increased to match the
// radius given), then for each object to test, calculate the distance to the
// path and include it if the range fits and the target is in the line (0<k<1,
// as they call it).
// The implementation I took as reference is found at
// http://astronomy.swin.edu.au/~pbourke/geometry/pointline/
// (they have a link to a C implementation, too)
// This approach is a lot like #2 commented on this function, which I have no
// idea why it was commented. I won't use doubles/floats, but pure int math for
// speed purposes. The range considered is always the same no matter how
// close/far the target is because that's how SharpShooting works currently in
// kRO.

	//Generic map_foreach* variables.
	int i, blockcount = bl_list_count;
	struct block_list *bl;
	int bx, by;
	//method specific variables
	int magnitude2, len_limit; //The square of the magnitude
	int k, xi, yi, xu, yu;
	int mx0 = x0, mx1 = x1, my0 = y0, my1 = y1;
	va_list ap;

	//Avoid needless calculations by not getting the sqrt right away.
	#define MAGNITUDE2(x0, y0, x1, y1) ( ( ( x1 ) - ( x0 ) ) * ( ( x1 ) - ( x0 ) ) + ( ( y1 ) - ( y0 ) ) * ( ( y1 ) - ( y0 ) ) )

	if ( m < 0 )
		return 0;

	len_limit = magnitude2 = MAGNITUDE2(x0, y0, x1, y1);
	if ( magnitude2 < 1 ) //Same begin and ending point, can't trace path.
		return 0;

	if ( length ) { //Adjust final position to fit in the given area.
		//TODO: Find an alternate method which does not requires a square root calculation.
		k = (int)sqrt((float)magnitude2);
		mx1 = x0 + (x1 - x0) * length / k;
		my1 = y0 + (y1 - y0) * length / k;
		len_limit = MAGNITUDE2(x0, y0, mx1, my1);
	}
	//Expand target area to cover range.
	if ( mx0 > mx1 ) {
		mx0 += range;
		mx1 -= range;
	} else {
		mx0 -= range;
		mx1 += range;
	}
	if (my0 > my1) {
		my0 += range;
		my1 -= range;
	} else {
		my0 -= range;
		my1 += range;
	}

	//The two fors assume mx0 < mx1 && my0 < my1
	if ( mx0 > mx1 )
		SWAP(mx0, mx1);
	if ( my0 > my1 )
		SWAP(my0, my1);

	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	mx0 = max(mx0, 0);
	my0 = max(my0, 0);
	mx1 = min(mx1, mapdata->xs - 1);
	my1 = min(my1, mapdata->ys - 1);

	range *= range << 8; //Values are shifted later on for higher precision using int math.

	if ( type&~BL_MOB )
		for ( by = my0 / BLOCK_SIZE; by <= my1 / BLOCK_SIZE; by++ ) {
			for( bx = mx0 / BLOCK_SIZE; bx <= mx1 / BLOCK_SIZE; bx++ ) {
				for( bl = mapdata->block[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
					if( bl->prev && bl->type&type && bl_list_count < BL_LIST_MAX ) {
						xi = bl->x;
						yi = bl->y;

						k = ( xi - x0 ) * ( x1 - x0 ) + ( yi - y0 ) * ( y1 - y0 );

						if ( k < 0 || k > len_limit ) //Since more skills use this, check for ending point as well.
							continue;

						if ( k > magnitude2 && !path_search_long(NULL, m, x0, y0, xi, yi, CELL_CHKWALL) )
							continue; //Targets beyond the initial ending point need the wall check.

						//All these shifts are to increase the precision of the intersection point and distance considering how it's
						//int math.
						k  = ( k << 4 ) / magnitude2; //k will be between 1~16 instead of 0~1
						xi <<= 4;
						yi <<= 4;
						xu = ( x0 << 4 ) + k * ( x1 - x0 );
						yu = ( y0 << 4 ) + k * ( y1 - y0 );
						k  = MAGNITUDE2(xi, yi, xu, yu);

						//If all dot coordinates were <<4 the square of the magnitude is <<8
						if ( k > range )
							continue;

						bl_list[ bl_list_count++ ] = bl;
					}
				}
			}
		}
	 if( type&BL_MOB )
		for( by = my0 / BLOCK_SIZE; by <= my1 / BLOCK_SIZE; by++ ) {
			for( bx = mx0 / BLOCK_SIZE; bx <= mx1 / BLOCK_SIZE; bx++ ) {
				for( bl = mapdata->block_mob[ bx + by * mapdata->bxs ]; bl != NULL; bl = bl->next ) {
					if( bl->prev && bl_list_count < BL_LIST_MAX ) {
						xi = bl->x;
						yi = bl->y;
						k = ( xi - x0 ) * ( x1 - x0 ) + ( yi - y0 ) * ( y1 - y0 );

						if ( k < 0 || k > len_limit )
							continue;

						if ( k > magnitude2 && !path_search_long(NULL, m, x0, y0, xi, yi, CELL_CHKWALL) )
							continue; //Targets beyond the initial ending point need the wall check.

						k  = ( k << 4 ) / magnitude2; //k will be between 1~16 instead of 0~1
						xi <<= 4;
						yi <<= 4;
						xu = ( x0 << 4 ) + k * ( x1 - x0 );
						yu = ( y0 << 4 ) + k * ( y1 - y0 );
						k  = MAGNITUDE2(xi, yi, xu, yu);

						//If all dot coordinates were <<4 the square of the magnitude is <<8
						if ( k > range )
							continue;

						bl_list[ bl_list_count++ ] = bl;
					}
				}
			}
		}

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_foreachinpath: block count too many!\n");

	map_freeblock_lock();

	for( i = blockcount; i < bl_list_count; i++ )
		if( bl_list[ i ]->prev ) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_start(ap, type);
			returnCount += func(bl_list[ i ], ap);
			va_end(ap);
		}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;	//[Skotlex]

}

/*========================================== [Playtester]
* Calls the given function for every object of a type that is on a path.
* The path goes into one of the eight directions and the direction is determined by the given coordinates.
* The path has a length, a width and an offset.
* The cost for diagonal movement is the same as for horizontal/vertical movement.
* @param m: ID of map
* @param x0: Start X
* @param y0: Start Y
* @param x1: X to calculate direction against
* @param y1: Y to calculate direction against
* @param range: Determines width of the path (width = range*2+1 cells)
* @param length: Length of the path
* @param offset: Moves the whole path, half-length for diagonal paths
* @param type: Type of bl to search for
*------------------------------------------*/
int map_foreachindir(int(*func)(struct block_list*, va_list), int16 m, int16 x0, int16 y0, int16 x1, int16 y1, int16 range, int length, int offset, int type, ...)
{
	int returnCount = 0;  //Total sum of returned values of func()

	int i, blockcount = bl_list_count;
	struct block_list *bl;
	int bx, by;
	int mx0, mx1, my0, my1, rx, ry;
	uint8 dir = map_calc_dir_xy(x0, y0, x1, y1, 6);
	short dx = dirx[dir];
	short dy = diry[dir];
	va_list ap;

	if (m < 0)
		return 0;

	if (range < 0)
		return 0;
	if (length < 1)
		return 0;
	if (offset < 0)
		return 0;

	//Special offset handling for diagonal paths
	if (offset && (dir % 2)) {
		//So that diagonal paths can attach to each other, we have to work with half-tile offsets
		offset = (2 * offset) - 1;
		//To get the half-tiles we need to increase length by one
		length++;
	}

	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	//Get area that needs to be checked
	mx0 = x0 + dx*(offset / ((dir % 2) + 1));
	my0 = y0 + dy*(offset / ((dir % 2) + 1));
	mx1 = x0 + dx*(offset / ((dir % 2) + 1) + length - 1);
	my1 = y0 + dy*(offset / ((dir % 2) + 1) + length - 1);

	//The following assumes mx0 < mx1 && my0 < my1
	if (mx0 > mx1)
		SWAP(mx0, mx1);
	if (my0 > my1)
		SWAP(my0, my1);

	//Apply width to the path by turning 90 degrees
	mx0 -= abs(range*dirx[(dir + 2) % 8]);
	my0 -= abs(range*diry[(dir + 2) % 8]);
	mx1 += abs(range*dirx[(dir + 2) % 8]);
	my1 += abs(range*diry[(dir + 2) % 8]);

	mx0 = max(mx0, 0);
	my0 = max(my0, 0);
	mx1 = min(mx1, mapdata->xs - 1);
	my1 = min(my1, mapdata->ys - 1);

	if (type&~BL_MOB) {
		for (by = my0 / BLOCK_SIZE; by <= my1 / BLOCK_SIZE; by++) {
			for (bx = mx0 / BLOCK_SIZE; bx <= mx1 / BLOCK_SIZE; bx++) {
				for (bl = mapdata->block[bx + by * mapdata->bxs]; bl != NULL; bl = bl->next) {
					if (bl->prev && bl->type&type && bl_list_count < BL_LIST_MAX) {
						//Check if inside search area
						if (bl->x < mx0 || bl->x > mx1 || bl->y < my0 || bl->y > my1)
							continue;
						//What matters now is the relative x and y from the start point
						rx = (bl->x - x0);
						ry = (bl->y - y0);
						//Do not hit source cell
						if (rx == 0 && ry == 0)
							continue;
						//This turns it so that the area that is hit is always with positive rx and ry
						rx *= dx;
						ry *= dy;
						//These checks only need to be done for diagonal paths
						if (dir % 2) {
							//Check for length
							if ((rx + ry < offset) || (rx + ry > 2 * (length + (offset/2) - 1)))
								continue;
							//Check for width
							if (abs(rx - ry) > 2 * range)
								continue;
						}
						//Everything else ok, check for line of sight from source
						if (!path_search_long(NULL, m, x0, y0, bl->x, bl->y, CELL_CHKWALL))
							continue;
						//All checks passed, add to list
						bl_list[bl_list_count++] = bl;
					}
				}
			}
		}
	}
	if (type&BL_MOB) {
		for (by = my0 / BLOCK_SIZE; by <= my1 / BLOCK_SIZE; by++) {
			for (bx = mx0 / BLOCK_SIZE; bx <= mx1 / BLOCK_SIZE; bx++) {
				for (bl = mapdata->block_mob[bx + by * mapdata->bxs]; bl != NULL; bl = bl->next) {
					if (bl->prev && bl_list_count < BL_LIST_MAX) {
						//Check if inside search area
						if (bl->x < mx0 || bl->x > mx1 || bl->y < my0 || bl->y > my1)
							continue;
						//What matters now is the relative x and y from the start point
						rx = (bl->x - x0);
						ry = (bl->y - y0);
						//Do not hit source cell
						if (rx == 0 && ry == 0)
							continue;
						//This turns it so that the area that is hit is always with positive rx and ry
						rx *= dx;
						ry *= dy;
						//These checks only need to be done for diagonal paths
						if (dir % 2) {
							//Check for length
							if ((rx + ry < offset) || (rx + ry > 2 * (length + (offset / 2) - 1)))
								continue;
							//Check for width
							if (abs(rx - ry) > 2 * range)
								continue;
						}
						//Everything else ok, check for line of sight from source
						if (!path_search_long(NULL, m, x0, y0, bl->x, bl->y, CELL_CHKWALL))
							continue;
						//All checks passed, add to list
						bl_list[bl_list_count++] = bl;
					}
				}
			}
		}
	}

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_foreachindir: block count too many!\n");

	map_freeblock_lock();

	for( i = blockcount; i < bl_list_count; i++ )
		if( bl_list[ i ]->prev ) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_start(ap, type);
			returnCount += func(bl_list[ i ], ap);
			va_end(ap);
		}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;
}

// Copy of map_foreachincell, but applied to the whole map. [Skotlex]
int map_foreachinmap(int (*func)(struct block_list*,va_list), int16 m, int type,...)
{
	int b, bsize;
	int returnCount = 0;  //total sum of returned values of func() [Skotlex]
	struct block_list *bl;
	int blockcount = bl_list_count, i;
	struct map_data *mapdata = map_mapdata_exists(m);
	va_list ap;

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	bsize = mapdata->bxs * mapdata->bys;

	if( type&~BL_MOB )
		for( b = 0; b < bsize; b++ )
			for( bl = mapdata->block[ b ]; bl != NULL; bl = bl->next )
				if( bl->type&type && bl_list_count < BL_LIST_MAX )
					bl_list[ bl_list_count++ ] = bl;

	if( type&BL_MOB )
		for( b = 0; b < bsize; b++ )
			for( bl = mapdata->block_mob[ b ]; bl != NULL; bl = bl->next )
				if( bl_list_count < BL_LIST_MAX )
					bl_list[ bl_list_count++ ] = bl;

	if( bl_list_count >= BL_LIST_MAX )
		ShowWarning("map_foreachinmap: block count too many!\n");

	map_freeblock_lock();

	for( i = blockcount; i < bl_list_count ; i++ )
		if( bl_list[ i ]->prev ) { //func() may delete this bl_list[] slot, checking for prev ensures it wasn't queued for deletion.
			va_start(ap, type);
			returnCount += func(bl_list[ i ], ap);
			va_end(ap);
		}

	map_freeblock_unlock();

	bl_list_count = blockcount;
	return returnCount;
}


/// Generates a new flooritem object id from the interval [MIN_FLOORITEM, MAX_FLOORITEM).
/// Used for floor items, skill units and chatroom objects.
/// @return The new object id
int map_get_new_object_id(void)
{
	static int last_object_id = MIN_FLOORITEM - 1;
	int i;

	// find a free id
	i = last_object_id + 1;
	while( i != last_object_id ) {
		if( i == MAX_FLOORITEM )
			i = MIN_FLOORITEM;

		if( !idb_exists(id_db, i) )
			break;

		++i;
	}

	if( i == last_object_id ) {
		ShowError("map_addobject: no free object id!\n");
		return 0;
	}

	// update cursor
	last_object_id = i;

	return i;
}

/*==========================================
 * Timered function to clear the floor (remove remaining item)
 * Called each flooritem_lifetime ms
 *------------------------------------------*/
TIMER_FUNC(map_clearflooritem_timer){
	struct flooritem_data* fitem = (struct flooritem_data*)idb_get(id_db, id);

	if (fitem == NULL || fitem->bl.type != BL_ITEM || (fitem->cleartimer != tid)) {
		ShowError("map_clearflooritem_timer : error\n");
		return 1;
	}


	if (pet_db_search(fitem->item.nameid, PET_EGG))
		intif_delete_petdata(MakeDWord(fitem->item.card[1], fitem->item.card[2]));

	clif_clearflooritem(fitem, 0);
	map_deliddb(&fitem->bl);
	map_delblock(&fitem->bl);
	map_freeblock(&fitem->bl);
	return 0;
}

/*
 * clears a single bl item out of the map.
 */
void map_clearflooritem(struct block_list *bl) {
	struct flooritem_data* fitem = (struct flooritem_data*)bl;

	if( fitem->cleartimer != INVALID_TIMER )
		delete_timer(fitem->cleartimer,map_clearflooritem_timer);

	clif_clearflooritem(fitem, 0);
	map_deliddb(&fitem->bl);
	map_delblock(&fitem->bl);
	map_freeblock(&fitem->bl);
}

/*==========================================
 * (m,x,y) locates a random available free cell around the given coordinates
 * to place an BL_ITEM object. Scan area is 9x9, returns 1 on success.
 * x and y are modified with the target cell when successful.
 *------------------------------------------*/
int map_searchrandfreecell(int16 m,int16 *x,int16 *y,int stack) {
	int free_cell,i,j;
	int free_cells[9][2];
	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	for(free_cell=0,i=-1;i<=1;i++){
		if(i+*y<0 || i+*y>=mapdata->ys)
			continue;
		for(j=-1;j<=1;j++){
			if(j+*x<0 || j+*x>=mapdata->xs)
				continue;
			if(map_getcell(m,j+*x,i+*y,CELL_CHKNOPASS) && !map_getcell(m,j+*x,i+*y,CELL_CHKICEWALL))
				continue;
			//Avoid item stacking to prevent against exploits. [Skotlex]
			if(stack && map_count_oncell(m,j+*x,i+*y, BL_ITEM, 0) > stack)
				continue;
			free_cells[free_cell][0] = j+*x;
			free_cells[free_cell++][1] = i+*y;
		}
	}
	if(free_cell==0)
		return 0;
	free_cell = rnd()%free_cell;
	*x = free_cells[free_cell][0];
	*y = free_cells[free_cell][1];
	return 1;
}


static int map_count_sub(struct block_list *bl,va_list ap)
{
	return 1;
}

/*==========================================
 * Locates a random spare cell around the object given, using range as max
 * distance from that spot. Used for warping functions. Use range < 0 for
 * whole map range.
 * Returns 1 on success. when it fails and src is available, x/y are set to src's
 * src can be null as long as flag&1
 * when ~flag&1, m is not needed.
 * Flag values:
 * &1 = random cell must be around given m,x,y, not around src
 * &2 = the target should be able to walk to the target tile.
 * &4 = there shouldn't be any players around the target tile (use the no_spawn_on_player setting)
 *------------------------------------------*/
int map_search_freecell(struct block_list *src, int16 m, int16 *x,int16 *y, int16 rx, int16 ry, int flag)
{
	int tries, spawn=0;
	int bx, by;
	int rx2 = 2*rx+1;
	int ry2 = 2*ry+1;

	if( !src && (!(flag&1) || flag&2) )
	{
		ShowDebug("map_search_freecell: Incorrect usage! When src is NULL, flag has to be &1 and can't have &2\n");
		return 0;
	}

	if (flag&1) {
		bx = *x;
		by = *y;
	} else {
		bx = src->x;
		by = src->y;
		m = src->m;
	}
	if (!rx && !ry) {
		//No range? Return the target cell then....
		*x = bx;
		*y = by;
		return map_getcell(m,*x,*y,CELL_CHKREACH);
	}

	struct map_data *mapdata = map_mapdata_exists(m);

	if( mapdata == nullptr || mapdata->block == nullptr ){
		return 0;
	}

	if (rx >= 0 && ry >= 0) {
		tries = rx2*ry2;
		if (tries > 100) tries = 100;
	} else {
		tries = mapdata->xs*mapdata->ys;
		if (tries > 500) tries = 500;
	}

	while(tries--) {
		*x = (rx >= 0)?(rnd()%rx2-rx+bx):(rnd()%(mapdata->xs-2)+1);
		*y = (ry >= 0)?(rnd()%ry2-ry+by):(rnd()%(mapdata->ys-2)+1);

		if (*x == bx && *y == by)
			continue; //Avoid picking the same target tile.

		if (map_getcell(m,*x,*y,CELL_CHKREACH))
		{
			if(flag&2 && !unit_can_reach_pos(src, *x, *y, 1))
				continue;
			if(flag&4) {
				if (spawn >= 100) return 0; //Limit of retries reached.
				if (spawn++ < battle_config.no_spawn_on_player &&
					map_foreachinallarea(map_count_sub, m,
						*x-AREA_SIZE, *y-AREA_SIZE,
					  	*x+AREA_SIZE, *y+AREA_SIZE, BL_PC)
				)
				continue;
			}
			return 1;
		}
	}
	*x = bx;
	*y = by;
	return 0;
}

/*==========================================
 * Locates the closest, walkable cell with no blocks of a certain type on it
 * Returns true on success and sets x and y to cell found.
 * Otherwise returns false and x and y are not changed.
 * type: Types of block to count
 * flag: 
 *		0x1 - only count standing units
 *------------------------------------------*/
bool map_closest_freecell(int16 m, int16 *x, int16 *y, int type, int flag)
{
	uint8 dir = 6;
	int16 tx = *x;
	int16 ty = *y;
	int costrange = 10;

	if(!map_count_oncell(m, tx, ty, type, flag))
		return true; //Current cell is free

	//Algorithm only works up to costrange of 34
	while(costrange <= 34) {
		short dx = dirx[dir];
		short dy = diry[dir];

		//Linear search
		if(dir%2 == 0 && costrange%MOVE_COST == 0) {
			tx = *x+dx*(costrange/MOVE_COST);
			ty = *y+dy*(costrange/MOVE_COST);
			if(!map_count_oncell(m, tx, ty, type, flag) && map_getcell(m,tx,ty,CELL_CHKPASS)) {
				*x = tx;
				*y = ty;
				return true;
			}
		} 
		//Full diagonal search
		else if(dir%2 == 1 && costrange%MOVE_DIAGONAL_COST == 0) {
			tx = *x+dx*(costrange/MOVE_DIAGONAL_COST);
			ty = *y+dy*(costrange/MOVE_DIAGONAL_COST);
			if(!map_count_oncell(m, tx, ty, type, flag) && map_getcell(m,tx,ty,CELL_CHKPASS)) {
				*x = tx;
				*y = ty;
				return true;
			}
		}
		//One cell diagonal, rest linear (TODO: Find a better algorithm for this)
		else if(dir%2 == 1 && costrange%MOVE_COST == 4) {
			tx = *x+dx*((dir%4==3)?(costrange/MOVE_COST):1);
			ty = *y+dy*((dir%4==1)?(costrange/MOVE_COST):1);
			if(!map_count_oncell(m, tx, ty, type, flag) && map_getcell(m,tx,ty,CELL_CHKPASS)) {
				*x = tx;
				*y = ty;
				return true;
			}
			tx = *x+dx*((dir%4==1)?(costrange/MOVE_COST):1);
			ty = *y+dy*((dir%4==3)?(costrange/MOVE_COST):1);
			if(!map_count_oncell(m, tx, ty, type, flag) && map_getcell(m,tx,ty,CELL_CHKPASS)) {
				*x = tx;
				*y = ty;
				return true;
			}
		}

		//Get next direction
		if (dir == 5) {
			//Diagonal search complete, repeat with higher cost range
			if(costrange == 14) costrange += 6;
			else if(costrange == 28 || costrange >= 38) costrange += 2;
			else costrange += 4;
			dir = 6;
		} else if (dir == 4) {
			//Linear search complete, switch to diagonal directions
			dir = 7;
		} else {
			dir = (dir+2)%8;
		}
	}

	return false;
}

/*==========================================
 * Add an item in floor to location (m,x,y) and add restriction for those who could pickup later
 * NB : If charids are null their no restriction for pickup
 * @param item_data : item attributes
 * @param amount : items quantity
 * @param m : mapid
 * @param x : x coordinates
 * @param y : y coordinates
 * @param first_charid : 1st player that could loot the item (only charid that could loot for first_get_tick duration)
 * @param second_charid :  2nd player that could loot the item (2nd charid that could loot for second_get_charid duration)
 * @param third_charid : 3rd player that could loot the item (3rd charid that could loot for third_get_charid duration)
 * @param flag: &1 MVP item. &2 do stacking check. &4 bypass droppable check.
 * @param mob_id: Monster ID if dropped by monster
 * @return 0:failure, x:item_gid [MIN_FLOORITEM;MAX_FLOORITEM]==[2;START_ACCOUNT_NUM]
 *------------------------------------------*/
int map_addflooritem(struct item *item, int amount, int16 m, int16 x, int16 y, int first_charid, int second_charid, int third_charid, int flags, unsigned short mob_id, bool canShowEffect)
{
	int r;
	struct flooritem_data *fitem = NULL;

	nullpo_ret(item);

	if (!(flags&4) && battle_config.item_onfloor && (itemdb_traderight(item->nameid)&1))
		return 0; //can't be dropped

	if (!map_searchrandfreecell(m,&x,&y,flags&2?1:0))
		return 0;
	r = rnd();

	CREATE(fitem, struct flooritem_data, 1);
	fitem->bl.type=BL_ITEM;
	fitem->bl.prev = fitem->bl.next = NULL;
	fitem->bl.m=m;
	fitem->bl.x=x;
	fitem->bl.y=y;
	fitem->bl.id = map_get_new_object_id();
	if (fitem->bl.id==0) {
		aFree(fitem);
		return 0;
	}

	fitem->first_get_charid = first_charid;
	fitem->first_get_tick = gettick() + (flags&1 ? battle_config.mvp_item_first_get_time : battle_config.item_first_get_time);
	fitem->second_get_charid = second_charid;
	fitem->second_get_tick = fitem->first_get_tick + (flags&1 ? battle_config.mvp_item_second_get_time : battle_config.item_second_get_time);
	fitem->third_get_charid = third_charid;
	fitem->third_get_tick = fitem->second_get_tick + (flags&1 ? battle_config.mvp_item_third_get_time : battle_config.item_third_get_time);
	fitem->mob_id = mob_id;

	memcpy(&fitem->item,item,sizeof(*item));
	fitem->item.amount = amount;
	fitem->subx = (r&3)*3+3;
	fitem->suby = ((r>>2)&3)*3+3;
	fitem->cleartimer = add_timer(gettick()+battle_config.flooritem_lifetime,map_clearflooritem_timer,fitem->bl.id,0);

	map_addiddb(&fitem->bl);
	if (map_addblock(&fitem->bl))
		return 0;
	clif_dropflooritem(fitem,canShowEffect);

	return fitem->bl.id;
}

/**
 * @see DBCreateData
 */
static DBData create_charid2nick(DBKey key, va_list args)
{
	struct charid2nick *p;
	CREATE(p, struct charid2nick, 1);
	return db_ptr2data(p);
}

/// Adds(or replaces) the nick of charid to nick_db and fulfils pending requests.
/// Does nothing if the character is online.
void map_addnickdb(int charid, const char* nick)
{
	struct charid2nick* p;
	
	if( map_charid2sd(charid) )
		return;// already online

	p = (struct charid2nick*)idb_ensure(nick_db, charid, create_charid2nick);
	safestrncpy(p->nick, nick, sizeof(p->nick));

	while( p->requests ) {
		struct map_session_data* sd;
		struct charid_request* req;
		req = p->requests;
		p->requests = req->next;
		sd = map_charid2sd(req->charid);
		if( sd )
			clif_solved_charname(sd->fd, charid, p->nick);
		aFree(req);
	}
}

/// Removes the nick of charid from nick_db.
/// Sends name to all pending requests on charid.
void map_delnickdb(int charid, const char* name)
{
	struct charid2nick* p;
	DBData data;

	if (!nick_db->remove(nick_db, db_i2key(charid), &data) || (p = (struct charid2nick*)db_data2ptr(&data)) == NULL)
		return;

	while( p->requests ) {
		struct charid_request* req;
		struct map_session_data* sd;
		req = p->requests;
		p->requests = req->next;
		sd = map_charid2sd(req->charid);
		if( sd )
			clif_solved_charname(sd->fd, charid, name);
		aFree(req);
	}
	aFree(p);
}

/// Notifies sd of the nick of charid.
/// Uses the name in the character if online.
/// Uses the name in nick_db if offline.
void map_reqnickdb(struct map_session_data * sd, int charid)
{
	struct charid2nick* p;
	struct charid_request* req;
	struct map_session_data* tsd;

	nullpo_retv(sd);

	tsd = map_charid2sd(charid);
	if( tsd )
	{
		clif_solved_charname(sd->fd, charid, tsd->status.name);
		return;
	}

	p = (struct charid2nick*)idb_ensure(nick_db, charid, create_charid2nick);
	if( *p->nick )
	{
		clif_solved_charname(sd->fd, charid, p->nick);
		return;
	}
	// not in cache, request it
	CREATE(req, struct charid_request, 1);
	req->next = p->requests;
	p->requests = req;
	chrif_searchcharid(charid);
}

/*==========================================
 * add bl to id_db
 *------------------------------------------*/
void map_addiddb(struct block_list *bl)
{
	nullpo_retv(bl);

	if( bl->type == BL_PC )
	{
		TBL_PC* sd = (TBL_PC*)bl;
		idb_put(pc_db,sd->bl.id,sd);
		uidb_put(charid_db,sd->status.char_id,sd);
	}
	else if( bl->type == BL_MOB )
	{
		TBL_MOB* md = (TBL_MOB*)bl;
		idb_put(mobid_db,bl->id,bl);

		if( md->state.boss )
			idb_put(bossid_db, bl->id, bl);
	}

	if( bl->type & BL_REGEN )
		idb_put(regen_db, bl->id, bl);

	idb_put(id_db,bl->id,bl);
}

/*==========================================
 * remove bl from id_db
 *------------------------------------------*/
void map_deliddb(struct block_list *bl)
{
	nullpo_retv(bl);

	if( bl->type == BL_PC )
	{
		TBL_PC* sd = (TBL_PC*)bl;
		idb_remove(pc_db,sd->bl.id);
		uidb_remove(charid_db,sd->status.char_id);
	}
	else if( bl->type == BL_MOB )
	{
		idb_remove(mobid_db,bl->id);
		idb_remove(bossid_db,bl->id);
	}

	if( bl->type & BL_REGEN )
		idb_remove(regen_db,bl->id);

	idb_remove(id_db,bl->id);
}

/*==========================================
 * Standard call when a player connection is closed.
 *------------------------------------------*/
int map_quit(struct map_session_data *sd) {
	int i;

	if (sd->state.keepshop == false) { // Close vending/buyingstore
		if (sd->state.vending)
			vending_closevending(sd);
		else if (sd->state.buyingstore)
			buyingstore_close(sd);
	}

	if(!sd->state.active) { //Removing a player that is not active.
		struct auth_node *node = chrif_search(sd->status.account_id);
		if (node && node->char_id == sd->status.char_id &&
			node->state != ST_LOGOUT)
			//Except when logging out, clear the auth-connect data immediately.
			chrif_auth_delete(node->account_id, node->char_id, node->state);
		//Non-active players should not have loaded any data yet (or it was cleared already) so no additional cleanups are needed.
		return 0;
	}

	if (map_getmapdata(sd->bl.m)->m != sd->bl.m) {
		pc_setpos(sd, sd->status.save_point.map, sd->status.save_point.x, sd->status.save_point.y, CLR_TELEPORT);
		if (map_getmapdata(sd->bl.m)->m == sd->bl.m)
			map_quit(sd);
		return 0;
	}

	if (sd->state.bot)
		pcbot_stop(sd, 0);

	if (sd->aggro.head)
		mob_aggro_clear(sd);

	if (sd->state.hombot)
		map_delhombot(sd);

	if (sd->expiration_tid != INVALID_TIMER)
		delete_timer(sd->expiration_tid, pc_expiration_timer);

	if (sd->npc_timer_id != INVALID_TIMER) //Cancel the event timer.
		npc_timerevent_quit(sd);

	if (sd->autotrade_tid != INVALID_TIMER)
		delete_timer(sd->autotrade_tid, pc_autotrade_timer);

	if (sd->npc_id)
		npc_event_dequeue(sd);

	if (sd->bg_id)
		bg_team_leave(sd, true, true);

	if (sd->bg_queue_id > 0)
		bg_queue_leave(sd);

	if( sd->status.clan_id )
		clan_member_left(sd);

	pc_itemcd_do(sd,false);

	npc_script_event(sd, NPCE_LOGOUT);

	//Unit_free handles clearing the player related data,
	//map_quit handles extra specific data which is related to quitting normally
	//(changing map-servers invokes unit_free but bypasses map_quit)
	if( sd->sc.count ) {
		//Status that are not saved...
		status_change_end(&sd->bl, SC_BOSSMAPINFO, INVALID_TIMER);
		status_change_end(&sd->bl, SC_AUTOTRADE, INVALID_TIMER);
		status_change_end(&sd->bl, SC_SPURT, INVALID_TIMER);
		status_change_end(&sd->bl, SC_BERSERK, INVALID_TIMER);
		status_change_end(&sd->bl, SC__BLOODYLUST, INVALID_TIMER);
		status_change_end(&sd->bl, SC_TRICKDEAD, INVALID_TIMER);
		status_change_end(&sd->bl, SC_LEADERSHIP, INVALID_TIMER);
		status_change_end(&sd->bl, SC_GLORYWOUNDS, INVALID_TIMER);
		status_change_end(&sd->bl, SC_SOULCOLD, INVALID_TIMER);
		status_change_end(&sd->bl, SC_HAWKEYES, INVALID_TIMER);
		status_change_end(&sd->bl, SC_CHASEWALK2, INVALID_TIMER);
		if(sd->sc.data[SC_PROVOKE] && sd->sc.data[SC_PROVOKE]->timer == INVALID_TIMER)
			status_change_end(&sd->bl, SC_PROVOKE, INVALID_TIMER); //Infinite provoke ends on logout
		status_change_end(&sd->bl, SC_WEIGHT50, INVALID_TIMER);
		status_change_end(&sd->bl, SC_WEIGHT90, INVALID_TIMER);
		status_change_end(&sd->bl, SC_SATURDAYNIGHTFEVER, INVALID_TIMER);
		status_change_end(&sd->bl, SC_KYOUGAKU, INVALID_TIMER);
		status_change_end(&sd->bl, SC_C_MARKER, INVALID_TIMER);
		status_change_end(&sd->bl, SC_READYSTORM, INVALID_TIMER);
		status_change_end(&sd->bl, SC_READYDOWN, INVALID_TIMER);
		status_change_end(&sd->bl, SC_READYTURN, INVALID_TIMER);
		status_change_end(&sd->bl, SC_READYCOUNTER, INVALID_TIMER);
		status_change_end(&sd->bl, SC_DODGE, INVALID_TIMER);
		status_change_end(&sd->bl, SC_CBC, INVALID_TIMER);
		status_change_end(&sd->bl, SC_EQC, INVALID_TIMER);
		status_change_end(&sd->bl, SC_SPRITEMABLE, INVALID_TIMER);
		status_change_end(&sd->bl, SC_SV_ROOTTWIST, INVALID_TIMER);
		// Remove visuals effect from headgear
		status_change_end(&sd->bl, SC_MOONSTAR, INVALID_TIMER); 
		status_change_end(&sd->bl, SC_SUPER_STAR, INVALID_TIMER); 
		status_change_end(&sd->bl, SC_STRANGELIGHTS, INVALID_TIMER); 
		status_change_end(&sd->bl, SC_DECORATION_OF_MUSIC, INVALID_TIMER); 
		if (battle_config.debuff_on_logout&1) { //Remove negative buffs
			status_change_end(&sd->bl, SC_ORCISH, INVALID_TIMER);
			status_change_end(&sd->bl, SC_STRIPWEAPON, INVALID_TIMER);
			status_change_end(&sd->bl, SC_STRIPARMOR, INVALID_TIMER);
			status_change_end(&sd->bl, SC_STRIPSHIELD, INVALID_TIMER);
			status_change_end(&sd->bl, SC_STRIPHELM, INVALID_TIMER);
			status_change_end(&sd->bl, SC_EXTREMITYFIST, INVALID_TIMER);
			status_change_end(&sd->bl, SC_EXPLOSIONSPIRITS, INVALID_TIMER);
			if(sd->sc.data[SC_REGENERATION] && sd->sc.data[SC_REGENERATION]->val4)
				status_change_end(&sd->bl, SC_REGENERATION, INVALID_TIMER);
			//TO-DO Probably there are way more NPC_type negative status that are removed
			status_change_end(&sd->bl, SC_CHANGEUNDEAD, INVALID_TIMER);
			// Both these statuses are removed on logout. [L0ne_W0lf]
			status_change_end(&sd->bl, SC_SLOWCAST, INVALID_TIMER);
			status_change_end(&sd->bl, SC_CRITICALWOUND, INVALID_TIMER);
			status_change_end(&sd->bl, SC_H_MINE, INVALID_TIMER);
			status_change_end(&sd->bl, SC_ANTI_M_BLAST, INVALID_TIMER);
			status_change_end(&sd->bl, SC_B_TRAP, INVALID_TIMER);
		}
		if (battle_config.debuff_on_logout&2) { //Remove positive buffs
			status_change_end(&sd->bl, SC_MAXIMIZEPOWER, INVALID_TIMER);
			status_change_end(&sd->bl, SC_MAXOVERTHRUST, INVALID_TIMER);
			status_change_end(&sd->bl, SC_STEELBODY, INVALID_TIMER);
			status_change_end(&sd->bl, SC_PRESERVE, INVALID_TIMER);
			status_change_end(&sd->bl, SC_KAAHI, INVALID_TIMER);
			status_change_end(&sd->bl, SC_SPIRIT, INVALID_TIMER);
			status_change_end(&sd->bl, SC_HEAT_BARREL, INVALID_TIMER);
			status_change_end(&sd->bl, SC_P_ALTER, INVALID_TIMER);
			status_change_end(&sd->bl, SC_E_CHAIN, INVALID_TIMER);
			status_change_end(&sd->bl, SC_SIGHTBLASTER, INVALID_TIMER);
			status_change_end(&sd->bl, SC_BENEDICTIO, INVALID_TIMER);
			status_change_end(&sd->bl, SC_GLASTHEIM_ATK, INVALID_TIMER);
			status_change_end(&sd->bl, SC_GLASTHEIM_DEF, INVALID_TIMER);
			status_change_end(&sd->bl, SC_GLASTHEIM_HEAL, INVALID_TIMER);
			status_change_end(&sd->bl, SC_GLASTHEIM_HIDDEN, INVALID_TIMER);
			status_change_end(&sd->bl, SC_GLASTHEIM_STATE, INVALID_TIMER);
			status_change_end(&sd->bl, SC_GLASTHEIM_ITEMDEF, INVALID_TIMER);
			status_change_end(&sd->bl, SC_GLASTHEIM_HPSP, INVALID_TIMER);
			status_change_end(&sd->bl, SC_SOULGOLEM, INVALID_TIMER);
			status_change_end(&sd->bl, SC_SOULSHADOW, INVALID_TIMER);
			status_change_end(&sd->bl, SC_SOULFALCON, INVALID_TIMER);
			status_change_end(&sd->bl, SC_SOULFAIRY, INVALID_TIMER);
		}
	}

	for (i = 0; i < EQI_MAX; i++) {
		if (sd->equip_index[i] >= 0)
			if (pc_isequip(sd,sd->equip_index[i]))
				pc_unequipitem(sd,sd->equip_index[i],2);
	}

	// Return loot to owner
	if( sd->pd ) pet_lootitem_drop(sd->pd, sd);

	if (sd->ed) // Remove effects here rather than unit_remove_map_pc so we don't clear on Teleport/map change.
		elemental_clean_effect(sd->ed);

	if (sd->state.permanent_speed == 1) sd->state.permanent_speed = 0; // Remove lock so speed is set back to normal at login.

	struct map_data *mapdata = map_getmapdata(sd->bl.m);

	if (mapdata->instance_id && (battle_config.instance_leave_record & 1))
		instance_left_point(sd, mapdata->instance_id);

	if( mapdata->instance_id > 0 )
		instance_delusers(mapdata->instance_id);

	unit_remove_map_pc(sd,CLR_RESPAWN);

	if( mapdata->instance_id > 0 ) { // Avoid map conflicts and warnings on next login
		int16 m;
		struct point *pt;
		if( mapdata->save.map )
			pt = &mapdata->save;
		else
			pt = &sd->status.save_point;

		if( (m=map_mapindex2mapid(pt->map)) >= 0 )
		{
			sd->bl.m = m;
			sd->bl.x = pt->x;
			sd->bl.y = pt->y;
			sd->mapindex = pt->map;
		}
	}

	if (sd->state.vending)
		idb_remove(vending_getdb(), sd->status.char_id);

	if (sd->state.buyingstore)
		idb_remove(buyingstore_getdb(), sd->status.char_id);

	pc_damage_log_clear(sd,0);
	party_booking_delete(sd); // Party Booking [Spiria]
	pc_makesavestatus(sd);
	pc_clean_skilltree(sd);
	pc_crimson_marker_clear(sd);
	chrif_save(sd, CSAVE_QUIT|CSAVE_INVENTORY|CSAVE_CART);
	unit_free_pc(sd);
	return 0;
}

/*==========================================
 * Lookup, id to session (player,mob,npc,homon,merc..)
 *------------------------------------------*/
struct map_session_data * map_id2sd(int id){
	if (id <= 0) return NULL;
	return (struct map_session_data*)idb_get(pc_db,id);
}

struct mob_data * map_id2md(int id){
	if (id <= 0) return NULL;
	return (struct mob_data*)idb_get(mobid_db,id);
}

struct npc_data * map_id2nd(int id){
	struct block_list* bl = map_id2bl(id);
	return BL_CAST(BL_NPC, bl);
}

struct homun_data* map_id2hd(int id){
	struct block_list* bl = map_id2bl(id);
	return BL_CAST(BL_HOM, bl);
}

struct mercenary_data* map_id2mc(int id){
	struct block_list* bl = map_id2bl(id);
	return BL_CAST(BL_MER, bl);
}

struct pet_data* map_id2pd(int id){
	struct block_list* bl = map_id2bl(id);
	return BL_CAST(BL_PET, bl);
}

struct elemental_data* map_id2ed(int id) {
	struct block_list* bl = map_id2bl(id);
	return BL_CAST(BL_ELEM, bl);
}

struct chat_data* map_id2cd(int id){
	struct block_list* bl = map_id2bl(id);
	return BL_CAST(BL_CHAT, bl);
}

/// Returns the nick of the target charid or NULL if unknown (requests the nick to the char server).
const char* map_charid2nick(int charid)
{
	struct charid2nick *p;
	struct map_session_data* sd;

	sd = map_charid2sd(charid);
	if( sd )
		return sd->status.name;// character is online, return it's name

	p = (struct charid2nick*)idb_ensure(nick_db, charid, create_charid2nick);
	if( *p->nick )
		return p->nick;// name in nick_db

	chrif_searchcharid(charid);// request the name
	return NULL;
}

/// Returns the struct map_session_data of the charid or NULL if the char is not online.
struct map_session_data* map_charid2sd(int charid)
{
	return (struct map_session_data*)uidb_get(charid_db, charid);
}

/*==========================================
 * Search session data from a nick name
 * (without sensitive case if necessary)
 * return map_session_data pointer or NULL
 *------------------------------------------*/
struct map_session_data * map_nick2sd(const char *nick, bool allow_partial)
{
	struct map_session_data* sd;
	struct map_session_data* found_sd;
	struct s_mapiterator* iter;
	size_t nicklen;
	int qty = 0;

	if( nick == NULL )
		return NULL;

	nicklen = strlen(nick);
	iter = mapit_getallusers();

	found_sd = NULL;
	for( sd = (TBL_PC*)mapit_first(iter); mapit_exists(iter); sd = (TBL_PC*)mapit_next(iter) )
	{
		if( allow_partial && battle_config.partial_name_scan )
		{// partial name search
			if( strnicmp(sd->status.name, nick, nicklen) == 0 )
			{
				found_sd = sd;

				if( strcmp(sd->status.name, nick) == 0 )
				{// Perfect Match
					qty = 1;
					break;
				}

				qty++;
			}
		}
		else if( strcasecmp(sd->status.name, nick) == 0 )
		{// exact search only
			found_sd = sd;
			qty = 1;
			break;
		}
	}
	mapit_free(iter);

	if( battle_config.partial_name_scan && qty != 1 )
		found_sd = NULL;

	return found_sd;
}

/*==========================================
 * Looksup id_db DBMap and returns BL pointer of 'id' or NULL if not found
 *------------------------------------------*/
struct block_list * map_id2bl(int id) {
	return (struct block_list*)idb_get(id_db,id);
}

/**
 * Same as map_id2bl except it only checks for its existence
 **/
bool map_blid_exists( int id ) {
	return (idb_exists(id_db,id));
}

/*==========================================
 * Convex Mirror
 *------------------------------------------*/
struct mob_data * map_getmob_boss(int16 m)
{
	DBIterator* iter;
	struct mob_data *md = NULL;
	bool found = false;

	iter = db_iterator(bossid_db);
	for( md = (struct mob_data*)dbi_first(iter); dbi_exists(iter); md = (struct mob_data*)dbi_next(iter) )
	{
		if( md->bl.m == m )
		{
			found = true;
			break;
		}
	}
	dbi_destroy(iter);

	return (found)? md : NULL;
}

struct mob_data * map_id2boss(int id)
{
	if (id <= 0) return NULL;
	return (struct mob_data*)idb_get(bossid_db,id);
}

/// Applies func to all the players in the db.
/// Stops iterating if func returns -1.
void map_foreachpc(int (*func)(struct map_session_data* sd, va_list args), ...)
{
	DBIterator* iter;
	struct map_session_data* sd;

	iter = db_iterator(pc_db);
	for( sd = (struct map_session_data*)dbi_first(iter); dbi_exists(iter); sd = (struct map_session_data*)dbi_next(iter) )
	{
		va_list args;
		int ret;

		va_start(args, func);
		ret = func(sd, args);
		va_end(args);
		if( ret == -1 )
			break;// stop iterating
	}
	dbi_destroy(iter);
}

/// Applies func to all the mobs in the db.
/// Stops iterating if func returns -1.
void map_foreachmob(int (*func)(struct mob_data* md, va_list args), ...)
{
	DBIterator* iter;
	struct mob_data* md;

	iter = db_iterator(mobid_db);
	for( md = (struct mob_data*)dbi_first(iter); dbi_exists(iter); md = (struct mob_data*)dbi_next(iter) )
	{
		va_list args;
		int ret;

		va_start(args, func);
		ret = func(md, args);
		va_end(args);
		if( ret == -1 )
			break;// stop iterating
	}
	dbi_destroy(iter);
}

void map_foreachboss(int(*func)(struct mob_data* md, va_list args), ...)
{
	DBIterator* iter;
	struct mob_data* md;

	iter = db_iterator(bossid_db);
	for (md = (struct mob_data*)dbi_first(iter); dbi_exists(iter); md = (struct mob_data*)dbi_next(iter))
	{
		va_list args;
		int ret;

		va_start(args, func);
		ret = func(md, args);
		va_end(args);
		if (ret == -1)
			break;// stop iterating
	}
	dbi_destroy(iter);
}

/// Applies func to all the npcs in the db.
/// Stops iterating if func returns -1.
void map_foreachnpc(int (*func)(struct npc_data* nd, va_list args), ...)
{
	DBIterator* iter;
	struct block_list* bl;

	iter = db_iterator(id_db);
	for( bl = (struct block_list*)dbi_first(iter); dbi_exists(iter); bl = (struct block_list*)dbi_next(iter) )
	{
		if( bl->type == BL_NPC )
		{
			struct npc_data* nd = (struct npc_data*)bl;
			va_list args;
			int ret;

			va_start(args, func);
			ret = func(nd, args);
			va_end(args);
			if( ret == -1 )
				break;// stop iterating
		}
	}
	dbi_destroy(iter);
}

/// Applies func to everything in the db.
/// Stops iterating if func returns -1.
void map_foreachregen(int (*func)(struct block_list* bl, va_list args), ...)
{
	DBIterator* iter;
	struct block_list* bl;

	iter = db_iterator(regen_db);
	for( bl = (struct block_list*)dbi_first(iter); dbi_exists(iter); bl = (struct block_list*)dbi_next(iter) )
	{
		va_list args;
		int ret;

		va_start(args, func);
		ret = func(bl, args);
		va_end(args);
		if( ret == -1 )
			break;// stop iterating
	}
	dbi_destroy(iter);
}

/// Applies func to everything in the db.
/// Stops iterating if func returns -1.
void map_foreachiddb(int (*func)(struct block_list* bl, va_list args), ...)
{
	DBIterator* iter;
	struct block_list* bl;

	iter = db_iterator(id_db);
	for( bl = (struct block_list*)dbi_first(iter); dbi_exists(iter); bl = (struct block_list*)dbi_next(iter) )
	{
		va_list args;
		int ret;

		va_start(args, func);
		ret = func(bl, args);
		va_end(args);
		if( ret == -1 )
			break;// stop iterating
	}
	dbi_destroy(iter);
}

/// Iterator.
/// Can filter by bl type.
struct s_mapiterator
{
	enum e_mapitflags flags;// flags for special behaviour
	enum bl_type types;// what bl types to return
	DBIterator* dbi;// database iterator
};

/// Returns true if the block_list matches the description in the iterator.
///
/// @param _mapit_ Iterator
/// @param _bl_ block_list
/// @return true if it matches
#define MAPIT_MATCHES(_mapit_,_bl_) \
	( \
		( (_bl_)->type & (_mapit_)->types /* type matches */ ) \
	)

/// Allocates a new iterator.
/// Returns the new iterator.
/// types can represent several BL's as a bit field.
/// TODO should this be expanded to allow filtering of map/guild/party/chat/cell/area/...?
///
/// @param flags Flags of the iterator
/// @param type Target types
/// @return Iterator
struct s_mapiterator* mapit_alloc(enum e_mapitflags flags, enum bl_type types)
{
	struct s_mapiterator* mapit;

	CREATE(mapit, struct s_mapiterator, 1);
	mapit->flags = flags;
	mapit->types = types;
	if( types == BL_PC )       mapit->dbi = db_iterator(pc_db);
	else if( types == BL_MOB ) mapit->dbi = db_iterator(mobid_db);
	else                       mapit->dbi = db_iterator(id_db);
	return mapit;
}

/// Frees the iterator.
///
/// @param mapit Iterator
void mapit_free(struct s_mapiterator* mapit)
{
	nullpo_retv(mapit);

	dbi_destroy(mapit->dbi);
	aFree(mapit);
}

/// Returns the first block_list that matches the description.
/// Returns NULL if not found.
///
/// @param mapit Iterator
/// @return first block_list or NULL
struct block_list* mapit_first(struct s_mapiterator* mapit)
{
	struct block_list* bl;

	nullpo_retr(NULL,mapit);

	for( bl = (struct block_list*)dbi_first(mapit->dbi); bl != NULL; bl = (struct block_list*)dbi_next(mapit->dbi) )
	{
		if( MAPIT_MATCHES(mapit,bl) )
			break;// found match
	}
	return bl;
}

/// Returns the last block_list that matches the description.
/// Returns NULL if not found.
///
/// @param mapit Iterator
/// @return last block_list or NULL
struct block_list* mapit_last(struct s_mapiterator* mapit)
{
	struct block_list* bl;

	nullpo_retr(NULL,mapit);

	for( bl = (struct block_list*)dbi_last(mapit->dbi); bl != NULL; bl = (struct block_list*)dbi_prev(mapit->dbi) )
	{
		if( MAPIT_MATCHES(mapit,bl) )
			break;// found match
	}
	return bl;
}

/// Returns the next block_list that matches the description.
/// Returns NULL if not found.
///
/// @param mapit Iterator
/// @return next block_list or NULL
struct block_list* mapit_next(struct s_mapiterator* mapit)
{
	struct block_list* bl;

	nullpo_retr(NULL,mapit);

	for( ; ; )
	{
		bl = (struct block_list*)dbi_next(mapit->dbi);
		if( bl == NULL )
			break;// end
		if( MAPIT_MATCHES(mapit,bl) )
			break;// found a match
		// try next
	}
	return bl;
}

/// Returns the previous block_list that matches the description.
/// Returns NULL if not found.
///
/// @param mapit Iterator
/// @return previous block_list or NULL
struct block_list* mapit_prev(struct s_mapiterator* mapit)
{
	struct block_list* bl;

	nullpo_retr(NULL,mapit);

	for( ; ; )
	{
		bl = (struct block_list*)dbi_prev(mapit->dbi);
		if( bl == NULL )
			break;// end
		if( MAPIT_MATCHES(mapit,bl) )
			break;// found a match
		// try prev
	}
	return bl;
}

/// Returns true if the current block_list exists in the database.
///
/// @param mapit Iterator
/// @return true if it exists
bool mapit_exists(struct s_mapiterator* mapit)
{
	nullpo_retr(false,mapit);

	return dbi_exists(mapit->dbi);
}

/*==========================================
 * Add npc-bl to id_db, basically register npc to map
 *------------------------------------------*/
bool map_addnpc(int16 m,struct npc_data *nd)
{
	nullpo_ret(nd);

	if( m < 0 )
		return false;

	struct map_data *mapdata = map_getmapdata(m);

	if( mapdata->npc_num == MAX_NPC_PER_MAP )
	{
		ShowWarning("too many NPCs in one map %s\n",mapdata->name);
		return false;
	}

	int xs = -1, ys = -1;

	switch (nd->subtype) {
	case NPCTYPE_WARP:
		xs = nd->u.warp.xs;
		ys = nd->u.warp.ys;
		break;
	case NPCTYPE_SCRIPT:
		xs = nd->u.scr.xs;
		ys = nd->u.scr.ys;
		break;
	default:
		break;
	}
	// npcs with trigger area are grouped
	// 0 < npc_num_warp < npc_num_area < npc_num
	if (xs < 0 || ys < 0)
		mapdata->npc[ mapdata->npc_num ] = nd;
	else {
		switch (nd->subtype) {
		case NPCTYPE_WARP:
			mapdata->npc[ mapdata->npc_num ] = mapdata->npc[ mapdata->npc_num_area ];
			mapdata->npc[ mapdata->npc_num_area ] = mapdata->npc[ mapdata->npc_num_warp ];
			mapdata->npc[ mapdata->npc_num_warp ] = nd;
			mapdata->npc_num_warp++;
			mapdata->npc_num_area++;
			break;
		case NPCTYPE_SCRIPT:
			mapdata->npc[ mapdata->npc_num ] = mapdata->npc[ mapdata->npc_num_area ];
			mapdata->npc[ mapdata->npc_num_area ] = nd;
			mapdata->npc_num_area++;
			break;
		default:
			mapdata->npc[ mapdata->npc_num ] = nd;
			break;
		}
	}
	mapdata->npc_num++;
	idb_put(id_db,nd->bl.id,nd);
	return true;
}

/*==========================================
 * Add an instance map
 *------------------------------------------*/
int map_addinstancemap(int src_m, int instance_id)
{
	if(src_m < 0)
		return -1;

	const char *name = map_mapid2mapname(src_m);

	if(strlen(name) > 20) {
		// against buffer overflow
		ShowError("map_addinstancemap: Map name \"%s\" is too long.\n", name);
		return -2;
	}

	int16 dst_m = -1, i;

	for (i = instance_start; i < MAX_MAP_PER_SERVER; i++) {
		if (!map[i].name[0])
			break;
	}
	if (i < map_num) // Destination map value overwrites another
		dst_m = i;
	else if (i < MAX_MAP_PER_SERVER) // Destination map value increments to new map
		dst_m = map_num++;
	else {
		// Out of bounds
		ShowError("map_addinstancemap failed. map_num(%d) > map_max(%d)\n", map_num, MAX_MAP_PER_SERVER);
		return -3;
	}

	struct map_data *src_map = map_getmapdata(src_m);
	struct map_data *dst_map = map_getmapdata(dst_m);
	char iname[MAP_NAME_LENGTH];

	strcpy(iname, name);

	// Alter the name
	// Due to this being custom we only worry about preserving as many characters as necessary for accurate map distinguishing
	// This also allows us to maintain complete independence with main map functions
	if ((strchr(iname, '@') == nullptr) && strlen(iname) > 8) {
		memmove(iname, iname + (strlen(iname) - 9), strlen(iname));
		snprintf(dst_map->name, sizeof(dst_map->name), "%d#%s", (instance_id % 1000), iname);
	} else
		snprintf(dst_map->name, sizeof(dst_map->name), "%.3d%s", (instance_id % 1000), iname);
	dst_map->name[MAP_NAME_LENGTH - 1] = '\0';

	dst_map->m = dst_m;
	dst_map->instance_id = instance_id;
	dst_map->instance_src_map = src_m;
	dst_map->users = 0;
	dst_map->xs = src_map->xs;
	dst_map->ys = src_map->ys;
	dst_map->bxs = src_map->bxs;
	dst_map->bys = src_map->bys;
	dst_map->iwall_num = src_map->iwall_num;

	memset(dst_map->npc, 0, sizeof(dst_map->npc));
	dst_map->npc_num = 0;
	dst_map->npc_num_area = 0;
	dst_map->npc_num_warp = 0;

	// Reallocate cells
	size_t num_cell = dst_map->xs * dst_map->ys;

	CREATE( dst_map->cell, struct mapcell, num_cell );
	memcpy( dst_map->cell, src_map->cell, num_cell * sizeof(struct mapcell) );

	size_t size = dst_map->bxs * dst_map->bys * sizeof(struct block_list*);

	dst_map->block = (struct block_list **)aCalloc(1,size);
	dst_map->block_mob = (struct block_list **)aCalloc(1,size);

	dst_map->index = mapindex_addmap(-1, dst_map->name);
	dst_map->channel = nullptr;
	dst_map->mob_delete_timer = INVALID_TIMER;

	map_data_copy(dst_map, src_map);

	ShowInfo("[Instance] Created map '%s' (%d) from '%s' (%d).\n", dst_map->name, dst_map->m, name, src_map->m);

	map_addmap2db(dst_map);

	return dst_m;
}

/*==========================================
 * Set player to save point when they leave
 *------------------------------------------*/
static int map_instancemap_leave(struct block_list *bl, va_list ap)
{
	struct map_session_data* sd;

	nullpo_retr(0, bl);
	nullpo_retr(0, sd = (struct map_session_data *)bl);

	pc_setpos(sd, sd->status.save_point.map, sd->status.save_point.x, sd->status.save_point.y, CLR_TELEPORT);

	return 1;
}

/*==========================================
 * Remove all units from instance
 *------------------------------------------*/
static int map_instancemap_clean(struct block_list *bl, va_list ap)
{
	nullpo_retr(0, bl);
	switch(bl->type) {
		/*case BL_PC:
		// BL_PET, BL_HOM, BL_MER, and BL_ELEM are moved when BL_PC warped out in map_instancemap_leave
			map_quit((struct map_session_data *) bl);
			break;*/
		case BL_NPC:
			npc_unload((struct npc_data *)bl,true);
			break;
		case BL_MOB:
			unit_free(bl,CLR_OUTSIGHT);
			break;
		case BL_ITEM:
			map_clearflooritem(bl);
			break;
		case BL_SKILL:
			skill_delunit((struct skill_unit *) bl);
			break;
	}

	return 1;
}

static void map_free_questinfo(struct map_data *mapdata);

/*==========================================
 * Deleting an instance map
 *------------------------------------------*/
int map_delinstancemap(int m)
{
	struct map_data *mapdata = map_getmapdata(m);

	if(m < 0 || mapdata->instance_id <= 0)
		return 0;

	// Kick everyone out
	map_foreachinmap(map_instancemap_leave, m, BL_PC);

	// Do the unit cleanup
	map_foreachinmap(map_instancemap_clean, m, BL_ALL);

	if( mapdata->mob_delete_timer != INVALID_TIMER )
		delete_timer(mapdata->mob_delete_timer, map_removemobs_timer);
	mapdata->mob_delete_timer = INVALID_TIMER;

	// Free memory
	if (mapdata->cell)
		aFree(mapdata->cell);
	mapdata->cell = NULL;
	if (mapdata->block)
		aFree(mapdata->block);
	mapdata->block = NULL;
	if (mapdata->block_mob)
		aFree(mapdata->block_mob);
	mapdata->block_mob = NULL;

	map_free_questinfo(mapdata);
	mapdata->damage_adjust = {};
	mapdata->flag.clear();
	mapdata->flag_val.clear();
	mapdata->skill_damage.clear();

	mapindex_removemap(mapdata->index);
	map_removemapdb(mapdata);

	memset(&mapdata->name, '\0', sizeof(map[0].name)); // just remove the name
	return 1;
}

/*=========================================
 * Dynamic Mobs [Wizputer]
 *-----------------------------------------*/
// Stores the spawn data entry in the mob list.
// Returns the index of successful, or -1 if the list was full.
int map_addmobtolist(unsigned short m, struct spawn_data *spawn)
{
	size_t i;
	struct map_data *mapdata = map_getmapdata(m);

	ARR_FIND( 0, MAX_MOB_LIST_PER_MAP, i, mapdata->moblist[i] == NULL );
	if( i < MAX_MOB_LIST_PER_MAP )
	{
		mapdata->moblist[i] = spawn;
		return static_cast<int>(i);
	}
	return -1;
}

void map_spawnmobs(int16 m)
{
	int i, k=0;
	struct map_data *mapdata = map_getmapdata(m);

	if (mapdata->mob_delete_timer != INVALID_TIMER)
	{	//Mobs have not been removed yet [Skotlex]
		delete_timer(mapdata->mob_delete_timer, map_removemobs_timer);
		mapdata->mob_delete_timer = INVALID_TIMER;
		return;
	}
	for(i=0; i<MAX_MOB_LIST_PER_MAP; i++)
		if(mapdata->moblist[i]!=NULL)
		{
			k+=mapdata->moblist[i]->num;
			npc_parse_mob2(mapdata->moblist[i]);
		}

	if (battle_config.etc_log && k > 0)
	{
		ShowStatus("Map %s: Spawned '" CL_WHITE "%d" CL_RESET "' mobs.\n",mapdata->name, k);
	}
}

int map_removemobs_sub(struct block_list *bl, va_list ap)
{
	struct mob_data *md = (struct mob_data *)bl;
	nullpo_ret(md);

	//When not to remove mob:
	// doesn't respawn and is not a slave
	if( !md->spawn && !md->master_id )
		return 0;
	// respawn data is not in cache
	if( md->spawn && !md->spawn->state.dynamic )
		return 0;
	// hasn't spawned yet
	if( md->spawn_timer != INVALID_TIMER )
		return 0;
	// is damaged and mob_remove_damaged is off
	if( !battle_config.mob_remove_damaged && md->status.hp < md->status.max_hp )
		return 0;
	// is a mvp
	if( md->db->mexp > 0 )
		return 0;

	unit_free(&md->bl,CLR_OUTSIGHT);

	return 1;
}

TIMER_FUNC(map_removemobs_timer){
	int count;
	const int16 m = id;

	if (m < 0)
	{	//Incorrect map id!
		ShowError("map_removemobs_timer error: timer %d points to invalid map %d\n",tid, m);
		return 0;
	}

	struct map_data *mapdata = map_getmapdata(m);

	if (mapdata->mob_delete_timer != tid)
	{	//Incorrect timer call!
		ShowError("map_removemobs_timer mismatch: %d != %d (map %s)\n",mapdata->mob_delete_timer, tid, mapdata->name);
		return 0;
	}
	mapdata->mob_delete_timer = INVALID_TIMER;
	if (mapdata->users > 0) //Map not empty!
		return 1;

	count = map_foreachinmap(map_removemobs_sub, m, BL_MOB);

	if (battle_config.etc_log && count > 0)
		ShowStatus("Map %s: Removed '" CL_WHITE "%d" CL_RESET "' mobs.\n",mapdata->name, count);

	return 1;
}

void map_removemobs(int16 m)
{
	struct map_data *mapdata = map_getmapdata(m);

	if (mapdata->mob_delete_timer != INVALID_TIMER) // should never happen
		return; //Mobs are already scheduled for removal

	// Don't remove mobs on instance map
	if (mapdata->instance_id > 0)
		return;

	mapdata->mob_delete_timer = add_timer(gettick()+battle_config.mob_remove_delay, map_removemobs_timer, m, 0);
}

/*==========================================
 * Check for map_name from map_id
 *------------------------------------------*/
const char* map_mapid2mapname(int m)
{
	if (m == -1)
		return "Floating";

	struct map_data *mapdata = map_getmapdata(m);

	if (!mapdata)
		return "";

	if (mapdata->instance_id > 0) { // Instance map check
		std::shared_ptr<s_instance_data> idata = util::umap_find(instances, map[m].instance_id);

		if (!idata) // This shouldn't happen but if it does give them the map we intended to give
			return mapdata->name;
		else {
			for (const auto &it : idata->map) { // Loop to find the src map we want
				if (it.m == m)
					return map_getmapdata(it.src_m)->name;
			}
		}
	}

	return mapdata->name;
}

/*==========================================
 * Hookup, get map_id from map_name
 *------------------------------------------*/
int16 map_mapname2mapid(const char* name)
{
	unsigned short map_index;
	map_index = mapindex_name2id(name);
	if (!map_index)
		return -1;
	return map_mapindex2mapid(map_index);
}

/*==========================================
 * Returns the map of the given mapindex. [Skotlex]
 *------------------------------------------*/
int16 map_mapindex2mapid(unsigned short mapindex)
{
	struct map_data *md=NULL;

	if (!mapindex)
		return -1;

	md = (struct map_data*)uidb_get(map_db,(unsigned int)mapindex);
	if(md==NULL || md->cell==NULL)
		return -1;
	return md->m;
}

/*==========================================
 * Switching Ip, port ? (like changing map_server) get ip/port from map_name
 *------------------------------------------*/
int map_mapname2ipport(unsigned short name, uint32* ip, uint16* port)
{
	struct map_data_other_server *mdos;

	mdos = (struct map_data_other_server*)uidb_get(map_db,(unsigned int)name);
	if(mdos==NULL || mdos->cell) //If gat isn't null, this is a local map.
		return -1;
	*ip=mdos->ip;
	*port=mdos->port;
	return 0;
}

/*==========================================
 * Checks if both dirs point in the same direction.
 *------------------------------------------*/
int map_check_dir(int s_dir,int t_dir)
{
	if(s_dir == t_dir)
		return 0;
	switch(s_dir) {
		case 0: if(t_dir == 7 || t_dir == 1 || t_dir == 0) return 0; break;
		case 1: if(t_dir == 0 || t_dir == 2 || t_dir == 1) return 0; break;
		case 2: if(t_dir == 1 || t_dir == 3 || t_dir == 2) return 0; break;
		case 3: if(t_dir == 2 || t_dir == 4 || t_dir == 3) return 0; break;
		case 4: if(t_dir == 3 || t_dir == 5 || t_dir == 4) return 0; break;
		case 5: if(t_dir == 4 || t_dir == 6 || t_dir == 5) return 0; break;
		case 6: if(t_dir == 5 || t_dir == 7 || t_dir == 6) return 0; break;
		case 7: if(t_dir == 6 || t_dir == 0 || t_dir == 7) return 0; break;
	}
	return 1;
}

/*==========================================
 * Returns the direction of the given cell, relative to 'src'
 *------------------------------------------*/
uint8 map_calc_dir(struct block_list* src, int16 x, int16 y)
{
	uint8 dir = 0;

	nullpo_ret(src);

	dir = map_calc_dir_xy(src->x, src->y, x, y, unit_getdir(src));

	return dir;
}

/*==========================================
 * Returns the direction of the given cell, relative to source cell
 * Use this if you don't have a block list available to check against
 *------------------------------------------*/
uint8 map_calc_dir_xy(int16 srcx, int16 srcy, int16 x, int16 y, uint8 srcdir) {
	uint8 dir = 0;
	int dx, dy;

	dx = x-srcx;
	dy = y-srcy;
	if( dx == 0 && dy == 0 )
	{	// both are standing on the same spot
		// aegis-style, makes knockback default to the left
		// athena-style, makes knockback default to behind 'src'
		dir = (battle_config.knockback_left ? 6 : srcdir);
	}
	else if( dx >= 0 && dy >=0 )
	{	// upper-right
		if( dx >= dy*3 )      dir = 6;	// right
		else if( dx*3 < dy )  dir = 0;	// up
		else                  dir = 7;	// up-right
	}
	else if( dx >= 0 && dy <= 0 )
	{	// lower-right
		if( dx >= -dy*3 )     dir = 6;	// right
		else if( dx*3 < -dy ) dir = 4;	// down
		else                  dir = 5;	// down-right
	}
	else if( dx <= 0 && dy <= 0 )
	{	// lower-left
		if( dx*3 >= dy )      dir = 4;	// down
		else if( dx < dy*3 )  dir = 2;	// left
		else                  dir = 3;	// down-left
	}
	else
	{	// upper-left
		if( -dx*3 <= dy )     dir = 0;	// up
		else if( -dx > dy*3 ) dir = 2;	// left
		else                  dir = 1;	// up-left
	}
	return dir;
}

/*==========================================
 * Randomizes target cell x,y to a random walkable cell that
 * has the same distance from object as given coordinates do. [Skotlex]
 *------------------------------------------*/
int map_random_dir(struct block_list *bl, int16 *x, int16 *y)
{
	short xi = *x-bl->x;
	short yi = *y-bl->y;
	short i=0;
	int dist2 = xi*xi + yi*yi;
	short dist = (short)sqrt((float)dist2);

	if (dist < 1) dist =1;

	do {
		short j = 1 + 2*(rnd()%4); //Pick a random diagonal direction
		short segment = 1+(rnd()%dist); //Pick a random interval from the whole vector in that direction
		xi = bl->x + segment*dirx[j];
		segment = (short)sqrt((float)(dist2 - segment*segment)); //The complement of the previously picked segment
		yi = bl->y + segment*diry[j];
	} while (
		(map_getcell(bl->m,xi,yi,CELL_CHKNOPASS) || !path_search(NULL,bl->m,bl->x,bl->y,xi,yi,1,CELL_CHKNOREACH))
		&& (++i)<100 );

	if (i < 100) {
		*x = xi;
		*y = yi;
		return 1;
	}
	return 0;
}

// gat system
inline static struct mapcell map_gat2cell(int gat) {
	struct mapcell cell;

	memset(&cell,0,sizeof(struct mapcell));

	switch( gat ) {
		case 0: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // walkable ground
		case 1: cell.walkable = 0; cell.shootable = 0; cell.water = 0; break; // non-walkable ground
		case 2: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // ???
		case 3: cell.walkable = 1; cell.shootable = 1; cell.water = 1; break; // walkable water
		case 4: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // ???
		case 5: cell.walkable = 0; cell.shootable = 1; cell.water = 0; break; // gap (snipable)
		case 6: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // ???
		default:
			ShowWarning("map_gat2cell: unrecognized gat type '%d'\n", gat);
			break;
	}

	return cell;
}

static int map_cell2gat(struct mapcell cell)
{
	if( cell.walkable == 1 && cell.shootable == 1 && cell.water == 0 ) return 0;
	if( cell.walkable == 0 && cell.shootable == 0 && cell.water == 0 ) return 1;
	if( cell.walkable == 1 && cell.shootable == 1 && cell.water == 1 ) return 3;
	if( cell.walkable == 0 && cell.shootable == 1 && cell.water == 0 ) return 5;

	ShowWarning("map_cell2gat: cell has no matching gat type\n");
	return 1; // default to 'wall'
}

/*==========================================
 * Confirm if celltype in (m,x,y) match the one given in cellchk
 *------------------------------------------*/
int map_getcell(int16 m,int16 x,int16 y,cell_chk cellchk)
{
	if (m < 0)
		return 0;
	else
		return map_getcellp(map_getmapdata(m), x, y, cellchk);
}

int map_getcellp(struct map_data* m,int16 x,int16 y,cell_chk cellchk)
{
	struct mapcell cell;

	nullpo_ret(m);

	//NOTE: this intentionally overrides the last row and column
	if(!m->cell || x<0 || x>=m->xs-1 || y<0 || y>=m->ys-1)
		return( cellchk == CELL_CHKNOPASS );

	cell = m->cell[x + y*m->xs];

	switch(cellchk)
	{
		// gat type retrieval
		case CELL_GETTYPE:
			return map_cell2gat(cell);

		// base gat type checks
		case CELL_CHKWALL:
			return (!cell.walkable && !cell.shootable);

		case CELL_CHKWATER:
			return (cell.water);

		case CELL_CHKCLIFF:
			return (!cell.walkable && cell.shootable);


		// base cell type checks
		case CELL_CHKSAFEZONE:
			return (cell.safezone);
		case CELL_CHKNPC:
			return (cell.npc);
		case CELL_CHKBASILICA:
			return (cell.basilica);
		case CELL_CHKLANDPROTECTOR:
			return (cell.landprotector);
		case CELL_CHKNOVENDING:
			return (cell.novending);
		case CELL_CHKNOCHAT:
			return (cell.nochat);
		case CELL_CHKMAELSTROM:
			return (cell.maelstrom);
		case CELL_CHKICEWALL:
			return (cell.icewall);

		// special checks
		case CELL_CHKPASS:
#ifdef CELL_NOSTACK
			if (cell.cell_bl >= battle_config.custom_cell_stack_limit) return 0;
#endif
		case CELL_CHKREACH:
			return (cell.walkable);

		case CELL_CHKNOPASS:
#ifdef CELL_NOSTACK
			if (cell.cell_bl >= battle_config.custom_cell_stack_limit) return 1;
#endif
		case CELL_CHKNOREACH:
			return (!cell.walkable);

		case CELL_CHKSTACK:
#ifdef CELL_NOSTACK
			return (cell.cell_bl >= battle_config.custom_cell_stack_limit);
#else
			return 0;
#endif

		default:
			return 0;
	}
}

/*==========================================
 * Change the type/flags of a map cell
 * 'cell' - which flag to modify
 * 'flag' - true = on, false = off
 *------------------------------------------*/
void map_setcell(int16 m, int16 x, int16 y, cell_t cell, bool flag)
{
	int j;
	struct map_data *mapdata = map_getmapdata(m);

	if( m < 0 || x < 0 || x >= mapdata->xs || y < 0 || y >= mapdata->ys )
		return;

	j = x + y*mapdata->xs;

	switch( cell ) {
		case CELL_WALKABLE:      mapdata->cell[j].walkable = flag;      break;
		case CELL_SHOOTABLE:     mapdata->cell[j].shootable = flag;     break;
		case CELL_WATER:         mapdata->cell[j].water = flag;         break;

		case CELL_SAFEZONE:      mapdata->cell[j].safezone = flag;      break;
		case CELL_NPC:           mapdata->cell[j].npc = flag;           break;
		case CELL_BASILICA:      mapdata->cell[j].basilica = flag;      break;
		case CELL_LANDPROTECTOR: mapdata->cell[j].landprotector = flag; break;
		case CELL_NOVENDING:     mapdata->cell[j].novending = flag;     break;
		case CELL_NOCHAT:        mapdata->cell[j].nochat = flag;        break;
		case CELL_MAELSTROM:	 mapdata->cell[j].maelstrom = flag;	  break;
		case CELL_ICEWALL:		 mapdata->cell[j].icewall = flag;		  break;
		default:
			ShowWarning("map_setcell: invalid cell type '%d'\n", (int)cell);
			break;
	}
}

void map_setgatcell(int16 m, int16 x, int16 y, int gat)
{
	int j;
	struct mapcell cell;
	struct map_data *mapdata = map_getmapdata(m);

	if( m < 0 || x < 0 || x >= mapdata->xs || y < 0 || y >= mapdata->ys )
		return;

	j = x + y*mapdata->xs;

	cell = map_gat2cell(gat);
	mapdata->cell[j].walkable = cell.walkable;
	mapdata->cell[j].shootable = cell.shootable;
	mapdata->cell[j].water = cell.water;
}

/*==========================================
 * Invisible Walls
 *------------------------------------------*/
static DBMap* iwall_db;

bool map_iwall_exist(const char* wall_name)
{
	return strdb_exists(iwall_db, wall_name);
}

void map_iwall_nextxy(int16 x, int16 y, int8 dir, int pos, int16 *x1, int16 *y1)
{
	if( dir == 0 || dir == 4 )
		*x1 = x; // Keep X
	else if( dir > 0 && dir < 4 )
		*x1 = x - pos; // Going left
	else
		*x1 = x + pos; // Going right

	if( dir == 2 || dir == 6 )
		*y1 = y;
	else if( dir > 2 && dir < 6 )
		*y1 = y - pos;
	else
		*y1 = y + pos;
}

bool map_iwall_set(int16 m, int16 x, int16 y, int size, int8 dir, bool shootable, const char* wall_name)
{
	struct iwall_data *iwall;
	int i;
	int16 x1 = 0, y1 = 0;

	if( size < 1 || !wall_name )
		return false;

	if( (iwall = (struct iwall_data *)strdb_get(iwall_db, wall_name)) != NULL )
		return false; // Already Exists

	if( map_getcell(m, x, y, CELL_CHKNOREACH) )
		return false; // Starting cell problem

	CREATE(iwall, struct iwall_data, 1);
	iwall->m = m;
	iwall->x = x;
	iwall->y = y;
	iwall->size = size;
	iwall->dir = dir;
	iwall->shootable = shootable;
	safestrncpy(iwall->wall_name, wall_name, sizeof(iwall->wall_name));

	for( i = 0; i < size; i++ )
	{
		map_iwall_nextxy(x, y, dir, i, &x1, &y1);

		if( map_getcell(m, x1, y1, CELL_CHKNOREACH) )
			break; // Collision

		map_setcell(m, x1, y1, CELL_WALKABLE, false);
		map_setcell(m, x1, y1, CELL_SHOOTABLE, shootable);

		clif_changemapcell(0, m, x1, y1, map_getcell(m, x1, y1, CELL_GETTYPE), ALL_SAMEMAP);
	}

	iwall->size = i;

	strdb_put(iwall_db, iwall->wall_name, iwall);
	map_getmapdata(m)->iwall_num++;

	return true;
}

void map_iwall_get(struct map_session_data *sd) {
	struct iwall_data *iwall;
	DBIterator* iter;
	int16 x1, y1;
	int i;

	if( map_getmapdata(sd->bl.m)->iwall_num < 1 )
		return;

	iter = db_iterator(iwall_db);
	for( iwall = (struct iwall_data *)dbi_first(iter); dbi_exists(iter); iwall = (struct iwall_data *)dbi_next(iter) ) {
		if( iwall->m != sd->bl.m )
			continue;

		for( i = 0; i < iwall->size; i++ ) {
			map_iwall_nextxy(iwall->x, iwall->y, iwall->dir, i, &x1, &y1);
			clif_changemapcell(sd->fd, iwall->m, x1, y1, map_getcell(iwall->m, x1, y1, CELL_GETTYPE), SELF);
		}
	}
	dbi_destroy(iter);
}

bool map_iwall_remove(const char *wall_name)
{
	struct iwall_data *iwall;
	int16 i, x1, y1;

	if( (iwall = (struct iwall_data *)strdb_get(iwall_db, wall_name)) == NULL )
		return false; // Nothing to do

	for( i = 0; i < iwall->size; i++ ) {
		map_iwall_nextxy(iwall->x, iwall->y, iwall->dir, i, &x1, &y1);

		map_setcell(iwall->m, x1, y1, CELL_SHOOTABLE, true);
		map_setcell(iwall->m, x1, y1, CELL_WALKABLE, true);

		clif_changemapcell(0, iwall->m, x1, y1, map_getcell(iwall->m, x1, y1, CELL_GETTYPE), ALL_SAMEMAP);
	}

	map_getmapdata(iwall->m)->iwall_num--;
	strdb_remove(iwall_db, iwall->wall_name);
	return true;
}

/**
 * @see DBCreateData
 */
static DBData create_map_data_other_server(DBKey key, va_list args)
{
	struct map_data_other_server *mdos;
	unsigned short mapindex = (unsigned short)key.ui;
	mdos=(struct map_data_other_server *)aCalloc(1,sizeof(struct map_data_other_server));
	mdos->index = mapindex;
	memcpy(mdos->name, mapindex_id2name(mapindex), MAP_NAME_LENGTH);
	return db_ptr2data(mdos);
}

/*==========================================
 * Add mapindex to db of another map server
 *------------------------------------------*/
int map_setipport(unsigned short mapindex, uint32 ip, uint16 port)
{
	struct map_data_other_server *mdos;

	mdos= (struct map_data_other_server *)uidb_ensure(map_db,(unsigned int)mapindex, create_map_data_other_server);

	if(mdos->cell) //Local map,Do nothing. Give priority to our own local maps over ones from another server. [Skotlex]
		return 0;
	if(ip == clif_getip() && port == clif_getport()) {
		//That's odd, we received info that we are the ones with this map, but... we don't have it.
		ShowFatalError("map_setipport : received info that this map-server SHOULD have map '%s', but it is not loaded.\n",mapindex_id2name(mapindex));
		exit(EXIT_FAILURE);
	}
	mdos->ip   = ip;
	mdos->port = port;
	return 1;
}

/**
 * Delete all the other maps server management
 * @see DBApply
 */
int map_eraseallipport_sub(DBKey key, DBData *data, va_list va)
{
	struct map_data_other_server *mdos = (struct map_data_other_server *)db_data2ptr(data);
	if(mdos->cell == NULL) {
		db_remove(map_db,key);
		aFree(mdos);
	}
	return 0;
}

int map_eraseallipport(void)
{
	map_db->foreach(map_db,map_eraseallipport_sub);
	return 1;
}

/*==========================================
 * Delete mapindex from db of another map server
 *------------------------------------------*/
int map_eraseipport(unsigned short mapindex, uint32 ip, uint16 port)
{
	struct map_data_other_server *mdos;

	mdos = (struct map_data_other_server*)uidb_get(map_db,(unsigned int)mapindex);
	if(!mdos || mdos->cell) //Map either does not exists or is a local map.
		return 0;

	if(mdos->ip==ip && mdos->port == port) {
		uidb_remove(map_db,(unsigned int)mapindex);
		aFree(mdos);
		return 1;
	}
	return 0;
}

/*==========================================
 * [Shinryo]: Init the mapcache
 *------------------------------------------*/
static char *map_init_mapcache(FILE *fp)
{
	size_t size = 0;
	char *buffer;

	// No file open? Return..
	nullpo_ret(fp);

	// Get file size
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Allocate enough space
	CREATE(buffer, char, size);

	// No memory? Return..
	nullpo_ret(buffer);

	// Read file into buffer..
	if(fread(buffer, 1, size, fp) != size) {
		ShowError("map_init_mapcache: Could not read entire mapcache file\n");
		return NULL;
	}

	return buffer;
}

/*==========================================
 * Map cache reading
 * [Shinryo]: Optimized some behaviour to speed this up
 *==========================================*/
int map_readfromcache(struct map_data *m, char *buffer, char *decode_buffer)
{
	int i;
	struct map_cache_main_header *header = (struct map_cache_main_header *)buffer;
	struct map_cache_map_info *info = NULL;
	char *p = buffer + sizeof(struct map_cache_main_header);

	for(i = 0; i < header->map_count; i++) {
		info = (struct map_cache_map_info *)p;

		if( strcmp(m->name, info->name) == 0 )
			break; // Map found

		// Jump to next entry..
		p += sizeof(struct map_cache_map_info) + info->len;
	}

	if( info && i < header->map_count ) {
		unsigned long size, xy;

		if( info->xs <= 0 || info->ys <= 0 )
			return 0;// Invalid

		m->xs = info->xs;
		m->ys = info->ys;
		size = (unsigned long)info->xs*(unsigned long)info->ys;

		if(size > MAX_MAP_SIZE) {
			ShowWarning("map_readfromcache: %s exceeded MAX_MAP_SIZE of %d\n", info->name, MAX_MAP_SIZE);
			return 0; // Say not found to remove it from list.. [Shinryo]
		}

		// TO-DO: Maybe handle the scenario, if the decoded buffer isn't the same size as expected? [Shinryo]
		decode_zip(decode_buffer, &size, p+sizeof(struct map_cache_map_info), info->len);

		CREATE(m->cell, struct mapcell, size);


		for( xy = 0; xy < size; ++xy )
			m->cell[xy] = map_gat2cell(decode_buffer[xy]);

		return 1;
	}

	return 0; // Not found
}

static DBMap* map_cache_db = NULL;
struct s_map_cache_map_info {
	char *buffer;
	struct map_cache_map_info info;
};

void map_loadfromcache(char *buffer)
{
	int i;
	struct map_cache_main_header *header = (struct map_cache_main_header *)buffer;
	struct map_cache_map_info *info = NULL;
	char *p = buffer + sizeof(struct map_cache_main_header);

	for (i = 0; i < header->map_count; i++) {
		struct s_map_cache_map_info *mapinfo;

		info = (struct map_cache_map_info *)p;

		if (!strdb_exists(map_cache_db, info->name)) {
			CREATE(mapinfo, struct s_map_cache_map_info, 1);
			memcpy(&mapinfo->info, p, sizeof(struct map_cache_map_info));
			mapinfo->buffer = p + sizeof(struct map_cache_map_info);
			strdb_put(map_cache_db, info->name, mapinfo);
		}
		else {
			mapinfo = (struct s_map_cache_map_info *)strdb_get(map_cache_db, info->name);
			memcpy(&mapinfo->info, p, sizeof(struct map_cache_map_info));
			mapinfo->buffer = p + sizeof(struct map_cache_map_info);
		}

		// Jump to next entry..
		p = mapinfo->buffer + info->len;
	}
	return;
}

int map_getfromcache(struct map_data *m, char *decode_buffer)
{
	struct s_map_cache_map_info *mapinfo = (struct s_map_cache_map_info *)strdb_get(map_cache_db, m->name);
	struct map_cache_map_info *info;

	if (!mapinfo)
		return 0;

	if ((info = &mapinfo->info) != NULL) {
		unsigned long size, xy;

		if (info->xs <= 0 || info->ys <= 0)
			return 0;// Invalid

		m->xs = info->xs;
		m->ys = info->ys;
		size = (unsigned long)info->xs*(unsigned long)info->ys;

		if (size > MAX_MAP_SIZE) {
			ShowWarning("map_readfromcache: %s exceeded MAX_MAP_SIZE of %d\n", info->name, MAX_MAP_SIZE);
			return 0; // Say not found to remove it from list.. [Shinryo]
		}

		// TO-DO: Maybe handle the scenario, if the decoded buffer isn't the same size as expected? [Shinryo]
		decode_zip(decode_buffer, &size, mapinfo->buffer, info->len);

		CREATE(m->cell, struct mapcell, size);


		for (xy = 0; xy < size; ++xy)
			m->cell[xy] = map_gat2cell(decode_buffer[xy]);

		return 1;
	}
	return 0;
}

int map_addmap(char* mapname)
{
	if( strcmpi(mapname,"clear")==0 )
	{
		map_num = 0;
		instance_start = 0;
		return 0;
	}

	if (map_num >= MAX_MAP_PER_SERVER - 1) {
		ShowError("Could not add map '" CL_WHITE "%s" CL_RESET "', the limit of maps has been reached.\n", mapname);
		return 1;
	}

	mapindex_getmapname(mapname, map[map_num].name);
	map_num++;
	return 0;
}

static void map_delmapid(int id)
{
	ShowNotice("Removing map [ %s ] from maplist" CL_CLL "\n",map[id].name);
	for (int i = id; i < map_num - 1; i++)
		map[i] = map[i + 1];
	map_num--;
}

int map_delmap(char* mapname){
	char map_name[MAP_NAME_LENGTH];

	if (strcmpi(mapname, "all") == 0) {
		map_num = 0;
		return 0;
	}

	mapindex_getmapname(mapname, map_name);
	for (int i = 0; i < map_num; i++) {
		if (strcmp(map[i].name, map_name) == 0) {
			map_delmapid(i);
			return 1;
		}
	}

	return 0;
}

static int map_copymap(char* str) {
	char src[MAP_NAME_LENGTH], target[MAP_NAME_LENGTH];
	if (sscanf(str, "%11[^ ] %11[^\r\n]", target, src) < 2)
		return 1;
	if (map_addmap(src) && map_num == 0)
		return 1;
	safestrncpy(map[map_num - 1].target, target, MAP_NAME_LENGTH);
	return 0;
}

/// Initializes map flags and adjusts them depending on configuration.
void map_flags_init(void){
	for (int i = 0; i < map_num; i++) {
		struct map_data *mapdata = &map[i];
		union u_mapflag_args args = {};

		mapdata->flag.clear();
		mapdata->flag_val.clear();
		mapdata->flag.reserve(MF_MAX); // Reserve the bucket size
		args.flag_val = 100;

		// additional mapflag data
		mapdata->zone = 0; // restricted mapflag zone
		mapdata->flag[MF_NOCOMMAND] = false; // nocommand mapflag level
		map_setmapflag_sub(i, MF_BEXP, true, &args); // per map base exp multiplicator
		map_setmapflag_sub(i, MF_JEXP, true, &args); // per map job exp multiplicator

		// Clear adjustment data, will be reset after loading NPC
		mapdata->damage_adjust = {};
		mapdata->drop_list.clear();
		mapdata->skill_damage.clear();
		mapdata->skill_duration.clear();
		map_free_questinfo(mapdata);

		if (instance_start && i >= instance_start)
			continue;

		// adjustments
		if( battle_config.pk_mode && !mapdata_flag_vs2(mapdata) )
			mapdata->flag[MF_PVP] = true; // make all maps pvp for pk_mode [Valaris]
	}
}

/**
* Copying map data from parent map for instance map
* @param dst_map Mapdata will be copied to
* @param src_map Copying data from
*/
void map_data_copy(struct map_data *dst_map, struct map_data *src_map) {
	nullpo_retv(dst_map);
	nullpo_retv(src_map);

	memcpy(&dst_map->save, &src_map->save, sizeof(struct point));
	memcpy(&dst_map->damage_adjust, &src_map->damage_adjust, sizeof(struct s_skill_damage));

	dst_map->flag.insert(src_map->flag.begin(), src_map->flag.end());
	dst_map->flag_val.insert(src_map->flag_val.begin(), src_map->flag_val.end());
	dst_map->skill_damage.insert(src_map->skill_damage.begin(), src_map->skill_damage.end());
	dst_map->skill_duration.insert(src_map->skill_duration.begin(), src_map->skill_duration.end());

	dst_map->zone = src_map->zone;

	// Mimic questinfo
	if (!src_map->qi_data.empty())
		src_map->qi_data = dst_map->qi_data;
}

/**
* Copy map data for instance maps from its parents
* that were cleared in map_flags_init() after reloadscript
*/
void map_data_copyall (void) {
	if (!instance_start)
		return;
	for (int i = instance_start; i < map_num; i++) {
		struct map_data *mapdata = &map[i];
		if (!mapdata || mapdata->name[0] == '\0' || !mapdata->instance_src_map)
			continue;
		map_data_copy(mapdata, &map[mapdata->instance_src_map]);
	}
}

/*
 * Reads from the .rsw for each map
 * Returns water height (or NO_WATER if file doesn't exist) or other error is encountered.
 * Assumed path for file is data/mapname.rsw
 * Credits to LittleWolf
 */
int map_waterheight(char* mapname)
{
	char fn[256];
 	char *found;

	//Look up for the rsw
	sprintf(fn, "data\\%s.rsw", mapname);

	found = grfio_find_file(fn);
	if (found) strcpy(fn, found); // replace with real name

	// read & convert fn
	return grfio_read_rsw_water_level( fn );
}

/*==================================
 * .GAT format
 *----------------------------------*/
int map_readgat (struct map_data* m)
{
	char filename[256];
	uint8* gat;
	int water_height;
	size_t xy, off, num_cells;

	sprintf(filename, "data\\%s.gat", m->name);

	gat = (uint8 *) grfio_read(filename);
	if (gat == NULL)
		return 0;

	m->xs = *(int32*)(gat+6);
	m->ys = *(int32*)(gat+10);
	num_cells = m->xs * m->ys;
	CREATE(m->cell, struct mapcell, num_cells);

	water_height = map_waterheight(m->name);

	// Set cell properties
	off = 14;
	for( xy = 0; xy < num_cells; ++xy )
	{
		// read cell data
		float height = *(float*)( gat + off      );
		uint32 type = *(uint32*)( gat + off + 16 );
		off += 20;

		if( type == 0 && water_height != RSW_NO_WATER && height > water_height )
			type = 3; // Cell is 0 (walkable) but under water level, set to 3 (walkable water)

		m->cell[xy] = map_gat2cell(type);
	}

	aFree(gat);

	return 1;
}

/*======================================
 * Add/Remove map to the map_db
 *--------------------------------------*/
void map_addmap2db(struct map_data *m)
{
	uidb_put(map_db, (unsigned int)m->index, m);
}

void map_removemapdb(struct map_data *m)
{
	uidb_remove(map_db, (unsigned int)m->index);
}

/*======================================
 * Initiate maps loading stage
 *--------------------------------------*/
int map_readallmaps (void)
{
	FILE* fp=NULL;
	// Has the uncompressed gat data of all maps, so just one allocation has to be made
	char *map_cache_buffer[2] = {
		NULL,
		NULL
	};
	char map_cache_decode_buffer[MAX_MAP_SIZE];

	if( enable_grf )
		ShowStatus("Loading maps (using GRF files)...\n");
	else {
		const char* mapcachefilepath[] = {
			"db/" DBPATH "map_cache.dat",
			"db/" DBIMPORT "/map_cache.dat"
		};

		for( int i = 0; i < 2; i++ ){
			ShowStatus( "Loading maps (using %s as map cache)...\n", mapcachefilepath[i] );

			if( ( fp = fopen(mapcachefilepath[i], "rb") ) == NULL ){
				if( i == 0 ){
					ShowFatalError( "Unable to open map cache file " CL_WHITE "%s" CL_RESET "\n", mapcachefilepath[i] );
					exit(EXIT_FAILURE); //No use launching server if maps can't be read.
				}else{
					ShowWarning( "Unable to open map cache file " CL_WHITE "%s" CL_RESET "\n", mapcachefilepath[i] );
					break;
				}
			}

			// Init mapcache data. [Shinryo]
			map_cache_buffer[i] = map_init_mapcache(fp);

			if( !map_cache_buffer[i] ) {
				ShowFatalError( "Failed to initialize mapcache data (%s)..\n", mapcachefilepath[i] );
				exit(EXIT_FAILURE);
			}

			fclose(fp);
		}
	}

	int maps_removed = 0;
	map_cache_db = strdb_alloc(DB_OPT_RELEASE_DATA, MAP_NAME_LENGTH);

	map_loadfromcache(map_cache_buffer[0]);
	map_loadfromcache(map_cache_buffer[1]);

	for (int i = 0; i < map_num; i++) {
		size_t size;
		bool success = false;
		unsigned short idx = 0;
		struct map_data *mapdata = &map[i];

		// show progress
		ShowStatus("Loading maps [%i/%i]: %s" CL_CLL "\r", i, map_num, mapdata->name);

		if( enable_grf ){
			// try to load the map
			success = map_readgat(mapdata) != 0;
		}else{
			//// try to load the map
			//// Read from import first, in case of override
			//if( map_cache_buffer[1] != NULL ){
			//	success = map_readfromcache( mapdata, map_cache_buffer[1], map_cache_decode_buffer ) != 0;
			//}
			//
			//// Nothing was found in import - try to find it in the main file
			//if( !success ){
			//	success = map_readfromcache( mapdata, map_cache_buffer[0], map_cache_decode_buffer ) != 0;
			//}
			success = map_getfromcache(mapdata, map_cache_decode_buffer) != 0;
		}

		if (success && mapdata->target && mapdata->target[0]) {
			safestrncpy(mapdata->name, mapdata->target, MAP_NAME_LENGTH);
			memset(mapdata->target, 0, MAP_NAME_LENGTH * sizeof(char));
			mapindex_addmap(-1, mapdata->name);
		}

		// The map was not found - remove it
		if (!(idx = mapindex_name2id(mapdata->name)) || !success) {
			map_delmapid(i);
			maps_removed++;
			i--;
			continue;
		}

		mapdata->index = idx;

		if (uidb_get(map_db,(unsigned int)mapdata->index) != NULL) {
			ShowWarning("Map %s already loaded!" CL_CLL "\n", mapdata->name);
			if (mapdata->cell) {
				aFree(mapdata->cell);
				mapdata->cell = NULL;
			}
			map_delmapid(i);
			maps_removed++;
			i--;
			continue;
		}

		map_addmap2db(mapdata);

		mapdata->m = i;
		memset(mapdata->moblist, 0, sizeof(mapdata->moblist));	//Initialize moblist [Skotlex]
		mapdata->mob_delete_timer = INVALID_TIMER;	//Initialize timer [Skotlex]

		mapdata->bxs = (mapdata->xs + BLOCK_SIZE - 1) / BLOCK_SIZE;
		mapdata->bys = (mapdata->ys + BLOCK_SIZE - 1) / BLOCK_SIZE;

		size = mapdata->bxs * mapdata->bys * sizeof(struct block_list*);
		mapdata->block = (struct block_list**)aCalloc(size, 1);
		mapdata->block_mob = (struct block_list**)aCalloc(size, 1);

		memset(&mapdata->save, 0, sizeof(struct point));
		mapdata->damage_adjust = {};
		mapdata->channel = NULL;
	}

	db_destroy(map_cache_db);

	// intialization and configuration-dependent adjustments of mapflags
	map_flags_init();

	if( !enable_grf ) {
		// The cache isn't needed anymore, so free it. [Shinryo]
		if( map_cache_buffer[1] != NULL ){
			aFree(map_cache_buffer[1]);
		}
		aFree(map_cache_buffer[0]);
	}

	if (maps_removed)
		ShowNotice("Maps removed: '" CL_WHITE "%d" CL_RESET "'" CL_CLL ".\n", maps_removed);

	// finished map loading
	ShowInfo("Successfully loaded '" CL_WHITE "%d" CL_RESET "' maps." CL_CLL "\n",map_num);

	return 0;
}

////////////////////////////////////////////////////////////////////////
uint32 map_multi_ip[C_MAX_MULTI_IP] = { 0 };
static int map_ip_set = 0;
static int char_ip_set = 0;

/*==========================================
 * Console Command Parser [Wizputer]
 *------------------------------------------*/
int parse_console(const char* buf){
	char type[64];
	char command[64];
	char mapname[MAP_NAME_LENGTH];
	int16 x = 0;
	int16 y = 0;
	int n;
	struct map_session_data sd;

	memset(&sd, 0, sizeof(struct map_session_data));
	strcpy(sd.status.name, "console");

	if( ( n = sscanf(buf, "%63[^:]:%63[^:]:%11s %6hd %6hd[^\n]", type, command, mapname, &x, &y) ) < 5 ){
		if( ( n = sscanf(buf, "%63[^:]:%63[^\n]", type, command) ) < 2 )		{
			if((n = sscanf(buf, "%63[^\n]", type))<1) return -1; //nothing to do no arg
		}
	}

	if( n != 5 ){ //end string and display
		if( n < 2 ) {
			ShowNotice("Type of command: '%s'\n", type);
			command[0] = '\0';
			mapname[0] = '\0';
		}
		else {
			ShowNotice("Type of command: '%s' || Command: '%s'\n", type, command);
			mapname[0] = '\0';
		}
	}
	else
		ShowNotice("Type of command: '%s' || Command: '%s' || Map: '%s' Coords: %d %d\n", type, command, mapname, x, y);

	if(strcmpi("admin",type) == 0 ) {
		if(strcmpi("map",command) == 0){
			int16 m = map_mapname2mapid(mapname);
			if( m < 0 ){
				ShowWarning("Console: Unknown map.\n");
				return 0;
			}
			sd.bl.m = m;
			map_search_freecell(&sd.bl, m, &sd.bl.x, &sd.bl.y, -1, -1, 0);
			if( x > 0 )
				sd.bl.x = x;
			if( y > 0 )
				sd.bl.y = y;
			ShowNotice("Now at: '%s' Coords: %d %d\n", mapname, x, y);
		}
		else if( !is_atcommand(sd.fd, &sd, command, 2) )
			ShowInfo("Console: Invalid atcommand.\n");
	}
	else if( n == 2 && strcmpi("server", type) == 0 ){
		if( strcmpi("shutdown", command) == 0 || strcmpi("exit", command) == 0 || strcmpi("quit", command) == 0 ){
			runflag = 0;
		}
	}
	else if( strcmpi("ers_report", type) == 0 ){
		ers_report();
	}
	else if( strcmpi("help", type) == 0 ) {
		ShowInfo("Available commands:\n");
		ShowInfo("\t admin:@<atcommand> => Uses an atcommand. Do NOT use commands requiring an attached player.\n");
		ShowInfo("\t admin:map:<map> <x> <y> => Changes the map from which console commands are executed.\n");
		ShowInfo("\t server:shutdown => Stops the server.\n");
		ShowInfo("\t ers_report => Displays database usage.\n");
	}

	return 0;
}

static int map_read_obfuscation_key(char *key) {
#ifdef C_READ_KEY_IN_CONFIG
	
	char *keytemp;
	int i;

	if (!strcmp(key,"default")){
		memset(config_keys,0, sizeof(config_keys));
		return 1;
	}

	for (i = 0; i < ARRAYLENGTH(config_keys); i++) {
		if ((keytemp = strstr(key, ",")) == NULL)
			keytemp = key + strlen(key);
		config_keys[i] = strtoul(key, &keytemp, 16);
		key = keytemp + 1;
	}
	return 1;
#else
	return 0;
#endif
}

/*==========================================
 * Read map server configuration files (conf/map_athena.conf...)
 *------------------------------------------*/
int map_config_read(const char *cfgName)
{
	char line[1024], w1[32], w2[1024];
	FILE *fp;

	fp = fopen(cfgName,"r");
	if( fp == NULL )
	{
		ShowError("Map configuration file not found at: %s\n", cfgName);
		return 1;
	}

	while( fgets_ex(line, sizeof(line), fp) )
	{
		char* ptr;

		if( line[0] == '/' && line[1] == '/' )
			continue;
		if( (ptr = strstr(line, "//")) != NULL )
			*ptr = '\n'; //Strip comments
		if( sscanf(line, "%31[^:]: %1023[^\t\r\n]", w1, w2) < 2 )
			continue;

		//Strip trailing spaces
		ptr = w2 + strlen(w2);
		while (--ptr >= w2 && *ptr == ' ');
		ptr++;
		*ptr = '\0';
		if (strcmpi(w1, "packet_keys") == 0)
			map_read_obfuscation_key(w2);
		else if(strcmpi(w1,"timestamp_format")==0)
			safestrncpy(timestamp_format, w2, 20);
		else if(strcmpi(w1,"stdout_with_ansisequence")==0)
			stdout_with_ansisequence = config_switch(w2);
		else if(strcmpi(w1,"console_silent")==0) {
			msg_silent = atoi(w2);
			if( msg_silent ) // only bother if its actually enabled
				ShowInfo("Console Silent Setting: %d\n", atoi(w2));
		} else if (strcmpi(w1, "userid")==0)
			chrif_setuserid(w2);
		else if (strcmpi(w1, "passwd") == 0)
			chrif_setpasswd(w2);
		else if (strcmpi(w1, "char_ip") == 0)
			char_ip_set = chrif_setip(w2);
		else if (strcmpi(w1, "char_port") == 0)
			chrif_setport(atoi(w2));
		else if (strcmpi(w1, "char_timeout") == 0)
			chrif_settimeout(atoi(w2));
		else if (strcmpi(w1, "char_check_interval") == 0)
			chrif_setcheckinterval(atoi(w2));
		else if (strcmpi(w1, "map_ip") == 0) {
			char *p[2], idx = 0;
			p[0] = w2;
			do {
				uint32 ip = 0;
				if ((p[1] = strchr(p[0], ',')) == NULL) {
					ip = host2ip(p[0]);
				}
				else {
					*p[1] = '\0';
					ip = host2ip(p[0]);
				}
				if (ip != 0)
					map_multi_ip[idx++] = ip;
				else
					ShowError("Invalid IP address '%s' in file 'char_athena.conf'\n", p[0]);

				if (p[1] == NULL) break;

				p[0] = p[1] + 1;
			} while (idx < C_MAX_MULTI_IP);

			map_ip_set = clif_setip(w2);
		}
		else if (strcmpi(w1, "bind_ip") == 0)
			clif_setbindip(w2);
		else if (strcmpi(w1, "map_port") == 0) {
			clif_setport(atoi(w2));
			map_port = (atoi(w2));
		} else if (strcmpi(w1, "map") == 0)
			map_addmap(w2);
		else if (strcmpi(w1, "copy") == 0)
			map_copymap(w2);
		else if (strcmpi(w1, "delmap") == 0)
			map_delmap(w2);
		else if (strcmpi(w1, "npc") == 0)
			npc_addsrcfile(w2, false);
		else if (strcmpi(w1, "delnpc") == 0)
			npc_delsrcfile(w2);
		else if (strcmpi(w1, "autosave_time") == 0) {
			autosave_interval = atoi(w2);
			if (autosave_interval < 1) //Revert to default saving.
				autosave_interval = DEFAULT_AUTOSAVE_INTERVAL;
			else
				autosave_interval *= 1000; //Pass from sec to ms
		} else if (strcmpi(w1, "minsave_time") == 0) {
			minsave_interval= atoi(w2);
			if (minsave_interval < 1)
				minsave_interval = 1;
		} else if (strcmpi(w1, "save_settings") == 0)
			save_settings = cap_value(atoi(w2),CHARSAVE_NONE,CHARSAVE_ALL);
		else if (strcmpi(w1, "motd_txt") == 0)
			safestrncpy(motd_txt, w2, sizeof(motd_txt));
		else if (strcmpi(w1, "charhelp_txt") == 0)
			safestrncpy(charhelp_txt, w2, sizeof(charhelp_txt));
		else if (strcmpi(w1, "channel_conf") == 0)
			safestrncpy(channel_conf, w2, sizeof(channel_conf));
		else if(strcmpi(w1,"db_path") == 0)
			safestrncpy(db_path,w2,ARRAYLENGTH(db_path));
		else if (strcmpi(w1, "console") == 0) {
			console = config_switch(w2);
			if (console)
				ShowNotice("Console Commands are enabled.\n");
		} else if (strcmpi(w1, "enable_spy") == 0)
			enable_spy = config_switch(w2);
		else if (strcmpi(w1, "use_grf") == 0)
			enable_grf = config_switch(w2);
		else if (strcmpi(w1, "console_msg_log") == 0)
			console_msg_log = atoi(w2);//[Ind]
		else if (strcmpi(w1, "console_log_filepath") == 0)
			safestrncpy(console_log_filepath, w2, sizeof(console_log_filepath));
		else if (strcmpi(w1, "import") == 0)
			map_config_read(w2);
		else
			ShowWarning("Unknown setting '%s' in file %s\n", w1, cfgName);
	}

	fclose(fp);
	return 0;
}

void map_reloadnpc_sub(const char *cfgName)
{
	char line[1024], w1[1024], w2[1024];
	FILE *fp;

	fp = fopen(cfgName,"r");
	if( fp == NULL )
	{
		ShowError("Map configuration file not found at: %s\n", cfgName);
		return;
	}

	while( fgets_ex(line, sizeof(line), fp) )
	{
		char* ptr;

		if( line[0] == '/' && line[1] == '/' )
			continue;
		if( (ptr = strstr(line, "//")) != NULL )
			*ptr = '\n'; //Strip comments
		if( sscanf(line, "%1023[^:]: %1023[^\t\r\n]", w1, w2) < 2 )
			continue;

		//Strip trailing spaces
		ptr = w2 + strlen(w2);
		while (--ptr >= w2 && *ptr == ' ');
		ptr++;
		*ptr = '\0';

		if (strcmpi(w1, "http") == 0) {
			safesnprintf(w1, sizeof(w1), "http://%s", w2);
			safestrncpy(w2, w1, sizeof(w2));
			safestrncpy(w1, "npc", sizeof(w1));
		}

		if (strcmpi(w1, "npc") == 0)
			npc_addsrcfile(w2, false);
		else if (strcmpi(w1, "delnpc") == 0)
			npc_delsrcfile(w2);
		else if (strcmpi(w1, "import") == 0)
			map_reloadnpc_sub(w2);
		else
			ShowWarning("Unknown setting '%s' in file %s\n", w1, cfgName);
	}

	fclose(fp);
}

void map_reloadnpc(bool clear)
{
	if (clear)
		npc_addsrcfile("clear", false); // this will clear the current script list

#ifdef RENEWAL
	map_reloadnpc_sub("npc/re/scripts_main.conf");
#else
	map_reloadnpc_sub("npc/pre-re/scripts_main.conf");
#endif
}

int inter_config_read(const char *cfgName)
{
	char line[1024],w1[1024],w2[1024];
	FILE *fp;

	fp=fopen(cfgName,"r");
	if(fp==NULL){
		ShowError("File not found: %s\n",cfgName);
		return 1;
	}
	while(fgets_ex(line, sizeof(line), fp))
	{
		if(line[0] == '/' && line[1] == '/')
			continue;
		if( sscanf(line,"%1023[^:]: %1023[^\r\n]",w1,w2) < 2 )
			continue;

#define RENEWALPREFIX "renewal-"
		if (!strncmpi(w1, RENEWALPREFIX, strlen(RENEWALPREFIX))) {
#ifdef RENEWAL
			// Move the original name to the beginning of the string
			memmove(w1, w1 + strlen(RENEWALPREFIX), strlen(w1) - strlen(RENEWALPREFIX) + 1);
#else
			// In Pre-Renewal the Renewal specific configurations can safely be ignored
			continue;
#endif
		}
#undef RENEWALPREFIX

		if( strcmpi( w1, "buyingstore_db" ) == 0 )
			safestrncpy( buyingstores_table, w2, sizeof(buyingstores_table) );
		else if( strcmpi( w1, "buyingstore_items_table" ) == 0 )
			safestrncpy( buyingstore_items_table, w2, sizeof(buyingstore_items_table) );
		else if(strcmpi(w1,"item_table")==0)
			safestrncpy(item_table,w2,sizeof(item_table));
		else if(strcmpi(w1,"item2_table")==0)
			safestrncpy(item2_table,w2,sizeof(item2_table));
		else if(strcmpi(w1,"mob_table")==0)
			safestrncpy(mob_table,w2,sizeof(mob_table));
		else if(strcmpi(w1,"mob2_table")==0)
			safestrncpy(mob2_table,w2,sizeof(mob2_table));
		else if(strcmpi(w1,"mob_skill_table")==0)
			safestrncpy(mob_skill_table,w2,sizeof(mob_skill_table));
		else if(strcmpi(w1,"mob_skill2_table")==0)
			safestrncpy(mob_skill2_table,w2,sizeof(mob_skill2_table));
		else if( strcmpi( w1, "item_cash_table" ) == 0 )
			safestrncpy( item_cash_table, w2, sizeof(item_cash_table) );
		else if( strcmpi( w1, "item_cash2_table" ) == 0 )
			safestrncpy( item_cash2_table, w2, sizeof(item_cash2_table) );
		else if( strcmpi( w1, "vending_db" ) == 0 )
			safestrncpy( vendings_table, w2, sizeof(vendings_table) );
		else if( strcmpi( w1, "vending_items_table" ) == 0 )
			safestrncpy(vending_items_table, w2, sizeof(vending_items_table));
		else if( strcmpi(w1, "roulette_table") == 0)
			safestrncpy(roulette_table, w2, sizeof(roulette_table));
		else if (strcmpi(w1, "market_table") == 0)
			safestrncpy(market_table, w2, sizeof(market_table));
		else if (strcmpi(w1, "sales_table") == 0)
			safestrncpy(sales_table, w2, sizeof(sales_table));
		else if (strcmpi(w1, "guild_storage_log") == 0)
			safestrncpy(guild_storage_log_table, w2, sizeof(guild_storage_log_table));
		else
		//Map Server SQL DB
		if(strcmpi(w1,"map_server_ip")==0)
			safestrncpy(map_server_ip, w2, sizeof(map_server_ip));
		else
		if(strcmpi(w1,"map_server_port")==0)
			map_server_port=atoi(w2);
		else
		if(strcmpi(w1,"map_server_id")==0)
			safestrncpy(map_server_id, w2, sizeof(map_server_id));
		else
		if(strcmpi(w1,"map_server_pw")==0)
			safestrncpy(map_server_pw, w2, sizeof(map_server_pw));
		else
		if(strcmpi(w1,"map_server_db")==0)
			safestrncpy(map_server_db, w2, sizeof(map_server_db));
		else
		if(strcmpi(w1,"default_codepage")==0)
			safestrncpy(default_codepage, w2, sizeof(default_codepage));
		else
		if(strcmpi(w1,"use_sql_db")==0) {
			db_use_sqldbs = config_switch(w2);
			ShowStatus ("Using SQL dbs: %s\n",w2);
		} else
		if(strcmpi(w1,"log_db_ip")==0)
			safestrncpy(log_db_ip, w2, sizeof(log_db_ip));
		else
		if(strcmpi(w1,"log_db_id")==0)
			safestrncpy(log_db_id, w2, sizeof(log_db_id));
		else
		if(strcmpi(w1,"log_db_pw")==0)
			safestrncpy(log_db_pw, w2, sizeof(log_db_pw));
		else
		if(strcmpi(w1,"log_db_port")==0)
			log_db_port = atoi(w2);
		else
		if(strcmpi(w1,"log_db_db")==0)
			safestrncpy(log_db_db, w2, sizeof(log_db_db));
		else
		if( mapreg_config_read(w1,w2) )
			continue;
		//support the import command, just like any other config
		else
		if(strcmpi(w1,"import")==0)
			inter_config_read(w2);
	}
	fclose(fp);

	return 0;
}

/*=======================================
 *  MySQL Init
 *---------------------------------------*/
int map_sql_init(void)
{
	// main db connection
	mmysql_handle = Sql_Malloc();
	qsmysql_handle = Sql_Malloc();

	ShowInfo("Connecting to the Map DB Server....\n");
	if( SQL_ERROR == Sql_Connect(mmysql_handle, map_server_id, map_server_pw, map_server_ip, map_server_port, map_server_db) ||
		SQL_ERROR == Sql_Connect(qsmysql_handle, map_server_id, map_server_pw, map_server_ip, map_server_port, map_server_db) )
	{
		ShowError("Couldn't connect with uname='%s',passwd='%s',host='%s',port='%d',database='%s'\n",
			map_server_id, map_server_pw, map_server_ip, map_server_port, map_server_db);
		Sql_ShowDebug(mmysql_handle);
		Sql_Free(mmysql_handle);
		Sql_ShowDebug(qsmysql_handle);
		Sql_Free(qsmysql_handle);
		exit(EXIT_FAILURE);
	}
	ShowStatus("Connect success! (Map Server Connection)\n");

	if( strlen(default_codepage) > 0 ) {
		if ( SQL_ERROR == Sql_SetEncoding(mmysql_handle, default_codepage) )
			Sql_ShowDebug(mmysql_handle);
		if ( SQL_ERROR == Sql_SetEncoding(qsmysql_handle, default_codepage) )
			Sql_ShowDebug(qsmysql_handle);
	}
	return 0;
}

int map_sql_close(void)
{
	ShowStatus("Close Map DB Connection....\n");
	Sql_Free(mmysql_handle);
	Sql_Free(qsmysql_handle);
	mmysql_handle = NULL;
	qsmysql_handle = NULL;

	if (log_config.sql_logs)
	{
		ShowStatus("Close Log DB Connection....\n");
		Sql_Free(logmysql_handle);
		logmysql_handle = NULL;
	}

	return 0;
}

int log_sql_init(void)
{
	// log db connection
	logmysql_handle = Sql_Malloc();

	ShowInfo("" CL_WHITE "[SQL]" CL_RESET ": Connecting to the Log Database " CL_WHITE "%s" CL_RESET " At " CL_WHITE "%s" CL_RESET "...\n",log_db_db,log_db_ip);
	if ( SQL_ERROR == Sql_Connect(logmysql_handle, log_db_id, log_db_pw, log_db_ip, log_db_port, log_db_db) ){
		ShowError("Couldn't connect with uname='%s',passwd='%s',host='%s',port='%d',database='%s'\n",
			log_db_id, log_db_pw, log_db_ip, log_db_port, log_db_db);
		Sql_ShowDebug(logmysql_handle);
		Sql_Free(logmysql_handle);
		exit(EXIT_FAILURE);
	}
	ShowStatus("" CL_WHITE "[SQL]" CL_RESET ": Successfully '" CL_GREEN "connected" CL_RESET "' to Database '" CL_WHITE "%s" CL_RESET "'.\n", log_db_db);

	if( strlen(default_codepage) > 0 )
		if ( SQL_ERROR == Sql_SetEncoding(logmysql_handle, default_codepage) )
			Sql_ShowDebug(logmysql_handle);

	return 0;
}

void map_remove_questinfo(int m, struct npc_data *nd) {
	struct map_data *mapdata = map_getmapdata(m);
	struct s_questinfo *qi;

	nullpo_retv(nd);
	nullpo_retv(mapdata);

	for (int i = 0; i < mapdata->qi_data.size(); i++) {
		qi = &mapdata->qi_data[i];
		if (qi && qi->nd == nd) {
			script_free_code(qi->condition);
			mapdata->qi_data.erase(mapdata->qi_data.begin() + i);
		}
	}
}

static void map_free_questinfo(struct map_data *mapdata) {
	nullpo_retv(mapdata);

	for (const auto &it : mapdata->qi_data) {
		if (it.condition)
			script_free_code(it.condition);
	}

	mapdata->qi_data.clear();
}

/**
 * @see DBApply
 */
int map_db_final(DBKey key, DBData *data, va_list ap)
{
	struct map_data_other_server *mdos = (struct map_data_other_server *)db_data2ptr(data);
	if(mdos && mdos->cell == NULL)
		aFree(mdos);
	return 0;
}

/**
 * @see DBApply
 */
int nick_db_final(DBKey key, DBData *data, va_list args)
{
	struct charid2nick* p = (struct charid2nick*)db_data2ptr(data);
	struct charid_request* req;

	if( p == NULL )
		return 0;
	while( p->requests )
	{
		req = p->requests;
		p->requests = req->next;
		aFree(req);
	}
	aFree(p);
	return 0;
}

int cleanup_sub(struct block_list *bl, va_list ap)
{
	nullpo_ret(bl);

	switch(bl->type) {
		case BL_PC:
			map_quit((struct map_session_data *) bl);
			break;
		case BL_NPC:
			npc_unload((struct npc_data *)bl,false);
			break;
		case BL_MOB:
			unit_free(bl,CLR_OUTSIGHT);
			break;
		case BL_PET:
		//There is no need for this, the pet is removed together with the player. [Skotlex]
			break;
		case BL_ITEM:
			map_clearflooritem(bl);
			break;
		case BL_SKILL:
			skill_delunit((struct skill_unit *) bl);
			break;
	}

	return 1;
}

/**
 * Add new skill damage adjustment entry for a map
 * @param m: Map data
 * @param skill_id: Skill ID
 * @param args: Mapflag arguments
 */
void map_skill_damage_add(struct map_data *m, uint16 skill_id, union u_mapflag_args *args) {
	nullpo_retv(m);
	nullpo_retv(args);

	if (m->skill_damage.find(skill_id) != m->skill_damage.end()) { // Entry exists
		args->skill_damage.caster |= m->skill_damage[skill_id].caster;
		m->skill_damage[skill_id] = args->skill_damage;
		return;
	}

	m->skill_damage.insert({ skill_id, args->skill_damage }); // Add new entry
}

/**
 * Add new skill duration adjustment entry for a map
 * @param mapd: Map data
 * @param skill_id: Skill ID to adjust
 * @param per: Skill duration adjustment value in percent
 */
void map_skill_duration_add(struct map_data *mapd, uint16 skill_id, uint16 per) {
	nullpo_retv(mapd);

	if (mapd->skill_duration.find(skill_id) != mapd->skill_duration.end()) // Entry exists
		mapd->skill_duration[skill_id] += per;
	else // Update previous entry
		mapd->skill_duration.insert({ skill_id, per });
}

/**
 * PvP timer handling (starting)
 * @param bl: Player block object
 * @param ap: func* with va_list values
 * @return 0
 */
static int map_mapflag_pvp_start_sub(struct block_list *bl, va_list ap)
{
	struct map_session_data *sd = map_id2sd(bl->id);

	nullpo_retr(0, sd);

	if (sd->pvp_timer == INVALID_TIMER) {
		sd->pvp_timer = add_timer(gettick() + 200, pc_calc_pvprank_timer, sd->bl.id, 0);
		sd->pvp_rank = 0;
		sd->pvp_lastusers = 0;
		sd->pvp_point = 5;
		sd->pvp_won = 0;
		sd->pvp_lost = 0;
	}

	clif_map_property(&sd->bl, MAPPROPERTY_FREEPVPZONE, SELF);
	return 0;
}

/**
 * PvP timer handling (stopping)
 * @param bl: Player block object
 * @param ap: func* with va_list values
 * @return 0
 */
static int map_mapflag_pvp_stop_sub(struct block_list *bl, va_list ap)
{
	struct map_session_data* sd = map_id2sd(bl->id);

	clif_pvpset(sd, 0, 0, 2);

	if (sd->pvp_timer != INVALID_TIMER) {
		delete_timer(sd->pvp_timer, pc_calc_pvprank_timer);
		sd->pvp_timer = INVALID_TIMER;
	}

	return 0;
}

/**
 * Return the mapflag enum from the given name.
 * @param name: Mapflag name
 * @return Mapflag enum value
 */
enum e_mapflag map_getmapflag_by_name(char* name)
{
	char flag_constant[255];
	int64 mapflag;

	safesnprintf(flag_constant, sizeof(flag_constant), "mf_%s", name);

	if (!script_get_constant(flag_constant, &mapflag))
		return MF_INVALID;
	else
		return (enum e_mapflag)mapflag;
}

/**
 * Return the mapflag name from the given enum.
 * @param mapflag: Mapflag enum
 * @param output: Stores the mapflag name
 * @return True on success otherwise false
 */
bool map_getmapflag_name( enum e_mapflag mapflag, char* output ){
	const char* constant;
	const char* prefix = "mf_";
	int i, len = strlen(prefix);

	// Look it up
	constant = script_get_constant_str( prefix, mapflag );

	// Should never happen
	if (constant == NULL)
		return false;

	// Begin copy after the prefix
	for(i = len; constant[i]; i++)
		output[i-len] = (char)tolower(constant[i]); // Force lowercase
	output[i - len] = 0; // Terminate it

	return true;
}

/**
 * Get a mapflag value
 * @param m: Map ID
 * @param mapflag: Mapflag ID
 * @param args: Arguments for special flags
 * @return Mapflag value on success or -1 on failure
 */
int map_getmapflag_sub(int16 m, enum e_mapflag mapflag, union u_mapflag_args *args)
{
	if (m < 0 || m >= MAX_MAP_PER_SERVER) {
		ShowWarning("map_getmapflag: Invalid map ID %d.\n", m);
		return -1;
	}

	struct map_data *mapdata = &map[m];

	if (mapflag < MF_MIN || mapflag >= MF_MAX) {
		ShowWarning("map_getmapflag: Invalid mapflag %d on map %s.\n", mapflag, mapdata->name);
		return -1;
	}

	switch(mapflag) {
		case MF_RESTRICTED:
			return mapdata->zone;
		case MF_NOLOOT:
			return util::umap_get(mapdata->flag, static_cast<int16>(MF_NOMOBLOOT), 0) && util::umap_get(mapdata->flag, static_cast<int16>(MF_NOMVPLOOT), 0);
		case MF_NOPENALTY:
			return util::umap_get(mapdata->flag, static_cast<int16>(MF_NOEXPPENALTY), 0) && util::umap_get(mapdata->flag, static_cast<int16>(MF_NOZENYPENALTY), 0);
		case MF_NOEXP:
			return util::umap_get(mapdata->flag, static_cast<int16>(MF_NOBASEEXP), 0) && util::umap_get(mapdata->flag, static_cast<int16>(MF_NOJOBEXP), 0);
		case MF_SKILL_DAMAGE:
			nullpo_retr(-1, args);

			switch (args->flag_val) {
				case SKILLDMG_PC:
				case SKILLDMG_MOB:
				case SKILLDMG_BOSS:
				case SKILLDMG_OTHER:
					return mapdata->damage_adjust.rate[args->flag_val];
				case SKILLDMG_CASTER:
					return mapdata->damage_adjust.caster;
				default:
					return util::umap_get(mapdata->flag, static_cast<int16>(mapflag), 0);
			}
		default:
			return util::umap_get(mapdata->flag, static_cast<int16>(mapflag), 0);
	}
}

/**
 * Set a mapflag
 * @param m: Map ID
 * @param mapflag: Mapflag ID
 * @param status: true - Set mapflag, false - Remove mapflag
 * @param args: Arguments for special flags
 * @return True on success or false on failure
 */
bool map_setmapflag_sub(int16 m, enum e_mapflag mapflag, bool status, union u_mapflag_args *args)
{
	if (m < 0 || m >= MAX_MAP_PER_SERVER) {
		ShowWarning("map_setmapflag: Invalid map ID %d.\n", m);
		return false;
	}

	struct map_data *mapdata = &map[m];

	if (mapflag < MF_MIN || mapflag >= MF_MAX) {
		ShowWarning("map_setmapflag: Invalid mapflag %d on map %s.\n", mapflag, mapdata->name);
		return false;
	}

	switch(mapflag) {
		case MF_NOSC:
		case MF_NOSKILL2:
		case MF_NOITEMFUNC:
		case MF_MOBINFO:
		case MF_MOBDROPRATE:
		case MF_MVPDROPRATE:
		case MF_PARTYLOCK2:
		case MF_MOBDMGFIX:
		case MF_MOBDMGMAX:
		case MF_MAXDMG_NORMAL:
		case MF_MAXDMG_SKILL:
		case MF_MAXHEAL:
		case MF_ATTACKEVENT:
		case MF_ATTACKEDEVENT:
		case MF_HEALEVENT:
		case MF_MAXASPD:
		case MF_INVENTORY_DB:
			if (status) {
				nullpo_retr(false, args);
				if (mapflag == MF_MOBINFO && args->flag_val == 0)
					args->flag_val = -1;
				mapdata->flag[mapflag] = args->flag_val;
			}else
				mapdata->flag[mapflag] = false;
			break;
		case MF_MOBATKRATE:
		case MF_MOBDMGRATE:
		case MF_PCATKRATE:
		case MF_PCDMGRATE:
		case MF_OTHERATKRATE:
		case MF_OTHERDMGRATE:
		case MF_MOBSPAWNBASE:
			if (status) {
				nullpo_retr(false, args);
				mapdata->flag_val[mapflag].val = args->u.val;
				mapdata->flag_val[mapflag].val2 = args->u.val2;
				mapdata->flag_val[mapflag].val3 = args->u.val3;
				mapdata->flag_val[mapflag].val4 = args->u.val4;
				mapdata->flag[mapflag] = true;
			}
			else {
				mapdata->flag_val.erase(mapflag);
				mapdata->flag[mapflag] = false;
			}
			break;
		case MF_NOSAVE:
			if (status) {
				nullpo_retr(false, args);

				mapdata->save.map = args->nosave.map;
				mapdata->save.x = args->nosave.x;
				mapdata->save.y = args->nosave.y;
			}
			mapdata->flag[mapflag] = status;
			break;
		case MF_PVP:
			mapdata->flag[mapflag] = status; // Must come first to properly set map property
			if (!status) {
				clif_map_property_mapall(m, MAPPROPERTY_NOTHING);
				map_foreachinmap(map_mapflag_pvp_stop_sub, m, BL_PC);
				map_foreachinmap(unit_stopattack, m, BL_CHAR, 0);
			} else {
				if (!battle_config.pk_mode) {
					clif_map_property_mapall(m, MAPPROPERTY_FREEPVPZONE);
					map_foreachinmap(map_mapflag_pvp_start_sub, m, BL_PC);
				}
				if (mapdata->flag[MF_GVG]) {
					mapdata->flag[MF_GVG] = false;
					ShowWarning("map_setmapflag: Unable to set GvG and PvP flags for the same map! Removing GvG flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_GVG_TE]) {
					mapdata->flag[MF_GVG_TE] = false;
					ShowWarning("map_setmapflag: Unable to set GvG TE and PvP flags for the same map! Removing GvG TE flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_GVG_DUNGEON]) {
					mapdata->flag[MF_GVG_DUNGEON] = false;
					ShowWarning("map_setmapflag: Unable to set GvG Dungeon and PvP flags for the same map! Removing GvG Dungeon flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_GVG_CASTLE]) {
					mapdata->flag[MF_GVG_CASTLE] = false;
					ShowWarning("map_setmapflag: Unable to set GvG Castle and PvP flags for the same map! Removing GvG Castle flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_GVG_TE_CASTLE]) {
					mapdata->flag[MF_GVG_TE_CASTLE] = false;
					ShowWarning("map_setmapflag: Unable to set GvG TE Castle and PvP flags for the same map! Removing GvG TE Castle flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_BATTLEGROUND]) {
					mapdata->flag[MF_BATTLEGROUND] = false;
					ShowWarning("map_setmapflag: Unable to set Battleground and PvP flags for the same map! Removing Battleground flag from %s.\n", mapdata->name);
				}
			}
			break;
		case MF_GVG:
		case MF_GVG_TE:
			mapdata->flag[mapflag] = status; // Must come first to properly set map property
			if (!status) {
				clif_map_property_mapall(m, MAPPROPERTY_NOTHING);
				map_foreachinmap(unit_stopattack, m, BL_CHAR, 0);
			} else {
				clif_map_property_mapall(m, MAPPROPERTY_AGITZONE);
				if (mapdata->flag[MF_PVP]) {
					mapdata->flag[MF_PVP] = false;
					if (!battle_config.pk_mode)
						ShowWarning("map_setmapflag: Unable to set PvP and GvG flags for the same map! Removing PvP flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_BATTLEGROUND]) {
					mapdata->flag[MF_BATTLEGROUND] = false;
					ShowWarning("map_setmapflag: Unable to set Battleground and GvG flags for the same map! Removing Battleground flag from %s.\n", mapdata->name);
				}
			}
			break;
		case MF_GVG_CASTLE:
		case MF_GVG_TE_CASTLE:
			if (status) {
				if (mapflag == MF_GVG_CASTLE && mapdata->flag[MF_GVG_TE_CASTLE]) {
					mapdata->flag[MF_GVG_TE_CASTLE] = false;
					ShowWarning("map_setmapflag: Unable to set GvG TE Castle and GvG Castle flags for the same map! Removing GvG TE Castle flag from %s.\n", mapdata->name);
				}
				if (mapflag == MF_GVG_TE_CASTLE && mapdata->flag[MF_GVG_CASTLE]) {
					mapdata->flag[MF_GVG_CASTLE] = false;
					ShowWarning("map_setmapflag: Unable to set GvG Castle and GvG TE Castle flags for the same map! Removing GvG Castle flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_PVP]) {
					mapdata->flag[MF_PVP] = false;
					if (!battle_config.pk_mode)
						ShowWarning("npc_parse_mapflag: Unable to set PvP and GvG%s Castle flags for the same map! Removing PvP flag from %s.\n", (mapflag == MF_GVG_CASTLE ? NULL : " TE"), mapdata->name);
				}
			}
			mapdata->flag[mapflag] = status;
			break;
		case MF_GVG_DUNGEON:
			if (status && mapdata->flag[MF_PVP]) {
				mapdata->flag[MF_PVP] = false;
				if (!battle_config.pk_mode)
					ShowWarning("map_setmapflag: Unable to set PvP and GvG Dungeon flags for the same map! Removing PvP flag from %s.\n", mapdata->name);
			}
			mapdata->flag[mapflag] = status;
			break;
		case MF_NOBASEEXP:
		case MF_NOJOBEXP:
			if (status) {
				if (mapflag == MF_NOBASEEXP && mapdata->flag[MF_BEXP] != 100) {
					mapdata->flag[MF_BEXP] = false;
					ShowWarning("map_setmapflag: Unable to set BEXP and No Base EXP flags for the same map! Removing BEXP flag from %s.\n", mapdata->name);
				}
				if (mapflag == MF_NOJOBEXP && mapdata->flag[MF_JEXP] != 100) {
					mapdata->flag[MF_JEXP] = false;
					ShowWarning("map_setmapflag: Unable to set JEXP and No Job EXP flags for the same map! Removing JEXP flag from %s.\n", mapdata->name);
				}
			}
			mapdata->flag[mapflag] = status;
			break;
		case MF_PVP_NIGHTMAREDROP:
			if (status) {
				nullpo_retr(false, args);

				if (mapdata->drop_list.size() == MAX_DROP_PER_MAP) {
					ShowWarning("map_setmapflag: Reached the maximum number of drop list items for mapflag pvp_nightmaredrop on %s. Skipping.\n", mapdata->name);
					break;
				}

				struct s_drop_list entry;

				entry.drop_id = args->nightmaredrop.drop_id;
				entry.drop_type = args->nightmaredrop.drop_type;
				entry.drop_per = args->nightmaredrop.drop_per;
				mapdata->drop_list.push_back(entry);
			}
			mapdata->flag[mapflag] = status;
			break;
		case MF_RESTRICTED:
			nullpo_retr(false, args);

			mapdata->flag[mapflag] = status;
			if (!status)
				mapdata->zone ^= (1 << (args->flag_val + 1)) << 3;
			else
				mapdata->zone |= (1 << (args->flag_val + 1)) << 3;
			break;
		case MF_NOCOMMAND:
			if (status) {
				nullpo_retr(false, args);

				mapdata->flag[mapflag] = ((args->flag_val <= 0) ? 100 : args->flag_val);
			} else
				mapdata->flag[mapflag] = false;
			break;
		case MF_JEXP:
		case MF_BEXP:
			if (status) {
				nullpo_retr(false, args);

				if (mapflag == MF_JEXP && mapdata->flag[MF_NOJOBEXP]) {
					mapdata->flag[MF_NOJOBEXP] = false;
					ShowWarning("map_setmapflag: Unable to set No Job EXP and JEXP flags for the same map! Removing No Job EXP flag from %s.\n", mapdata->name);
				}
				if (mapflag == MF_BEXP && mapdata->flag[MF_NOBASEEXP]) {
					mapdata->flag[MF_NOBASEEXP] = false;
					ShowWarning("map_setmapflag: Unable to set No Base EXP and BEXP flags for the same map! Removing No Base EXP flag from %s.\n", mapdata->name);
				}
				mapdata->flag[mapflag] = args->flag_val;
			} else
				mapdata->flag[mapflag] = false;
			break;
		case MF_BATTLEGROUND:
			if (status) {
				nullpo_retr(false, args);

				if (mapdata->flag[MF_PVP]) {
					mapdata->flag[MF_PVP] = false;
					ShowWarning("map_setmapflag: Unable to set PvP and Battleground flags for the same map! Removing PvP flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_GVG]) {
					mapdata->flag[MF_GVG] = false;
					ShowWarning("map_setmapflag: Unable to set GvG and Battleground flags for the same map! Removing GvG flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_GVG_DUNGEON]) {
					mapdata->flag[MF_GVG_DUNGEON] = false;
					ShowWarning("map_setmapflag: Unable to set GvG Dungeon and Battleground flags for the same map! Removing GvG Dungeon flag from %s.\n", mapdata->name);
				}
				if (mapdata->flag[MF_GVG_CASTLE]) {
					mapdata->flag[MF_GVG_CASTLE] = false;
					ShowWarning("map_setmapflag: Unable to set GvG Castle and Battleground flags for the same map! Removing GvG Castle flag from %s.\n", mapdata->name);
				}
				mapdata->flag[mapflag] = ((args->flag_val <= 0 || args->flag_val > 2) ? 1 : args->flag_val);
			} else
				mapdata->flag[mapflag] = false;
			break;
		case MF_NOLOOT:
			mapdata->flag[MF_NOMOBLOOT] = status;
			mapdata->flag[MF_NOMVPLOOT] = status;
			break;
		case MF_NOPENALTY:
			mapdata->flag[MF_NOEXPPENALTY] = status;
			mapdata->flag[MF_NOZENYPENALTY] = status;
			break;
		case MF_NOEXP:
			mapdata->flag[MF_NOBASEEXP] = status;
			mapdata->flag[MF_NOJOBEXP] = status;
			break;
		case MF_SKILL_DAMAGE:
			if (!status) {
				mapdata->damage_adjust = {};
				mapdata->skill_damage.clear();
			} else {
				nullpo_retr(false, args);

				if (!args->flag_val) { // Signifies if it's a single skill or global damage adjustment
					if (args->skill_damage.caster == 0) {
						ShowError("map_setmapflag: Skill damage adjustment without casting type for map %s.\n", mapdata->name);
						return false;
					}

					mapdata->damage_adjust.caster |= args->skill_damage.caster;
					for (int i = SKILLDMG_PC; i < SKILLDMG_MAX; i++)
						mapdata->damage_adjust.rate[i] = cap_value(args->skill_damage.rate[i], -100, 100000);
				}
			}
			mapdata->flag[mapflag] = status;
			break;
		case MF_SKILL_DURATION:
			if (!status)
				mapdata->skill_duration.clear();
			else {
				nullpo_retr(false, args);

				map_skill_duration_add(mapdata, args->skill_duration.skill_id, args->skill_duration.per);
			}
			mapdata->flag[mapflag] = status;
			break;
		default:
			mapdata->flag[mapflag] = status;
			break;
	}

	return true;
}

/**
 * @see DBApply
 */
static int cleanup_db_sub(DBKey key, DBData *data, va_list va)
{
	return cleanup_sub((struct block_list *)db_data2ptr(data), va);
}

/*==========================================
 * map destructor
 *------------------------------------------*/
void do_final(void){
	ShowStatus("Terminating...\n");
	channel_config.closing = true;

	//Ladies and babies first.
	struct s_mapiterator* iter = mapit_getallusers();
	for( struct map_session_data* sd = (TBL_PC*)mapit_first(iter); mapit_exists(iter); sd = (TBL_PC*)mapit_next(iter) )
		map_quit(sd);
	mapit_free(iter);

	/* prepares npcs for a faster shutdown process */
	do_clear_npc();

	// remove all objects on maps
	for (int i = 0; i < map_num; i++) {
		struct map_data *mapdata = map_getmapdata(i);

		ShowStatus("Cleaning up maps [%d/%d]: %s..." CL_CLL "\r", i++, map_num, mapdata->name);
		map_foreachinmap(cleanup_sub, i, BL_ALL);
		channel_delete(mapdata->channel,false);
	}
	ShowStatus("Cleaned up %d maps." CL_CLL "\n", map_num);

	id_db->foreach(id_db,cleanup_db_sub);
	chrif_char_reset_offline();
	chrif_flush_fifo();

	do_final_atcommand();
	do_final_battle();
	do_final_chrif();
	do_final_clan();
	do_final_clif();
	do_final_npc();
	do_final_quest();
	do_final_achievement();
	do_final_script();
	do_final_instance();
	do_final_itemdb();
	do_final_storage();
	do_final_guild();
	do_final_party();
	do_final_pc();
	do_final_pet();
	do_final_homunculus();
	do_final_mercenary();
	do_final_mob(false);
	do_final_msg();
	do_final_skill();
	do_final_status();
	do_final_unit();
	do_final_battleground();
	do_final_duel();
	do_final_elemental();
	do_final_cashshop();
	do_final_channel(); //should be called after final guild
	do_final_vending();
	do_final_buyingstore();
	do_final_path();
	do_final_chat();

	map_db->destroy(map_db, map_db_final);

	for (int i = 0; i < map_num; i++) {
		struct map_data *mapdata = map_getmapdata(i);

		if(mapdata->cell) aFree(mapdata->cell);
		if(mapdata->block) aFree(mapdata->block);
		if(mapdata->block_mob) aFree(mapdata->block_mob);
		if(battle_config.dynamic_mobs) { //Dynamic mobs flag by [random]
			if(mapdata->mob_delete_timer != INVALID_TIMER)
				delete_timer(mapdata->mob_delete_timer, map_removemobs_timer);
			for (int j=0; j<MAX_MOB_LIST_PER_MAP; j++)
				if (mapdata->moblist[j]) aFree(mapdata->moblist[j]);
		}
		map_free_questinfo(mapdata);
		mapdata->damage_adjust = {};
	}

	mapindex_final();
	if(enable_grf)
		grfio_final();

	id_db->destroy(id_db, NULL);
	pc_db->destroy(pc_db, NULL);
	bot_db->destroy(bot_db, NULL);
	hombot_db->destroy(hombot_db, NULL);
	mobid_db->destroy(mobid_db, NULL);
	bossid_db->destroy(bossid_db, NULL);
	nick_db->destroy(nick_db, nick_db_final);
	charid_db->destroy(charid_db, NULL);
	iwall_db->destroy(iwall_db, NULL);
	regen_db->destroy(regen_db, NULL);

	map_sql_close();

	ShowStatus("Finished.\n");
}

static int map_abort_sub(struct map_session_data* sd, va_list ap)
{
	chrif_save(sd, CSAVE_QUIT|CSAVE_INVENTORY|CSAVE_CART);
	return 1;
}


//------------------------------
// Function called when the server
// has received a crash signal.
//------------------------------
void do_abort(void)
{
	static int run = 0;
	//Save all characters and then flush the inter-connection.
	if (run) {
		ShowFatalError("Server has crashed while trying to save characters. Character data can't be saved!\n");
		return;
	}
	run = 1;
	if (!chrif_isconnected())
	{
		if (pc_db->size(pc_db))
			ShowFatalError("Server has crashed without a connection to the char-server, %u characters can't be saved!\n", pc_db->size(pc_db));
		return;
	}
	ShowError("Server received crash signal! Attempting to save all online characters!\n");
	map_foreachpc(map_abort_sub);
	chrif_flush_fifo();
}

/*======================================================
 * Map-Server help options screen
 *------------------------------------------------------*/
void display_helpscreen(bool do_exit)
{
	ShowInfo("Usage: %s [options]\n", SERVER_NAME);
	ShowInfo("\n");
	ShowInfo("Options:\n");
	ShowInfo("  -?, -h [--help]\t\tDisplays this help screen.\n");
	ShowInfo("  -v [--version]\t\tDisplays the server's version.\n");
	ShowInfo("  --run-once\t\t\tCloses server after loading (testing).\n");
	ShowInfo("  --map-config <file>\t\tAlternative map-server configuration.\n");
	ShowInfo("  --battle-config <file>\tAlternative battle configuration.\n");
	ShowInfo("  --atcommand-config <file>\tAlternative atcommand configuration.\n");
	ShowInfo("  --script-config <file>\tAlternative script configuration.\n");
	ShowInfo("  --msg-config <file>\t\tAlternative message configuration.\n");
	ShowInfo("  --grf-path <file>\t\tAlternative GRF path configuration.\n");
	ShowInfo("  --inter-config <file>\t\tAlternative inter-server configuration.\n");
	ShowInfo("  --log-config <file>\t\tAlternative logging configuration.\n");
	if( do_exit )
		exit(EXIT_SUCCESS);
}

/*======================================================
 * Map-Server Init and Command-line Arguments [Valaris]
 *------------------------------------------------------*/
void set_server_type(void)
{
	SERVER_TYPE = ATHENA_SERVER_MAP;
}

/*======================================================
 * Message System
 *------------------------------------------------------*/
struct msg_data {
	char* msg[MAP_MAX_MSG];
};
struct msg_data *map_lang2msgdb(uint8 lang){
	return (struct msg_data*)idb_get(map_msg_db, lang);
}

void map_do_init_msg(void){
	int test=0, i=0, size;
	const char * listelang[] = {
		MSG_CONF_NAME_EN,	//default
		MSG_CONF_NAME_RUS,
		MSG_CONF_NAME_SPN,
		MSG_CONF_NAME_GRM,
		MSG_CONF_NAME_CHN,
		MSG_CONF_NAME_MAL,
		MSG_CONF_NAME_IDN,
		MSG_CONF_NAME_FRN,
		MSG_CONF_NAME_POR,
		MSG_CONF_NAME_THA
	};

	map_msg_db = idb_alloc(DB_OPT_BASE);
	size = ARRAYLENGTH(listelang); //avoid recalc
	while(test!=-1 && size>i){ //for all enable lang +(English default)
		test = msg_checklangtype(i,false);
		if(test == 1) msg_config_read(listelang[i],i); //if enabled read it and assign i to langtype
		i++;
	}
	clif_read_mobinfo();
}
void map_do_final_msg(void){
	DBIterator *iter = db_iterator(map_msg_db);
	struct msg_data *mdb;

	for (mdb = (struct msg_data *)dbi_first(iter); dbi_exists(iter); mdb = (struct msg_data *)dbi_next(iter)) {
		_do_final_msg(MAP_MAX_MSG,mdb->msg);
		aFree(mdb);
	}
	dbi_destroy(iter);
	map_msg_db->destroy(map_msg_db, NULL);
}
void map_msg_reload(void){
	map_do_final_msg(); //clear data
	map_do_init_msg();
}
int map_msg_config_read(const char *cfgName, int lang){
	struct msg_data *mdb;

	if( (mdb = map_lang2msgdb(lang)) == NULL )
		CREATE(mdb, struct msg_data, 1);
	else
		idb_remove(map_msg_db, lang);
	idb_put(map_msg_db, lang, mdb);

	if(_msg_config_read(cfgName,MAP_MAX_MSG,mdb->msg)!=0){ //an error occur
		idb_remove(map_msg_db, lang); //@TRYME
		aFree(mdb);
	}
	return 0;
}
const char* map_msg_txt(struct map_session_data *sd, int msg_number){
	struct msg_data *mdb;
	uint8 lang = 0; //default
	if(sd && sd->langtype) lang = sd->langtype;

	if( (mdb = map_lang2msgdb(lang)) != NULL){
		const char *tmp = _msg_txt(msg_number,MAP_MAX_MSG,mdb->msg);
		if(strcmp(tmp,"??")) //to verify result
			return tmp;
		ShowDebug("Message #%d not found for langtype %d.\n",msg_number,lang);
	}
	ShowDebug("Selected langtype %d not loaded, trying fallback...\n",lang);
	if(lang != 0 && (mdb = map_lang2msgdb(0)) != NULL) //fallback
		return _msg_txt(msg_number,MAP_MAX_MSG,mdb->msg);
	return "??";
}


/// Called when a terminate signal is received.
void do_shutdown(void)
{
	if( runflag != MAPSERVER_ST_SHUTDOWN )
	{
		runflag = MAPSERVER_ST_SHUTDOWN;
		ShowStatus("Shutting down...\n");
		{
			struct map_session_data* sd;
			struct s_mapiterator* iter = mapit_getallusers();
			for( sd = (TBL_PC*)mapit_first(iter); mapit_exists(iter); sd = (TBL_PC*)mapit_next(iter) )
				clif_GM_kick(NULL, sd);
			mapit_free(iter);
			flush_fifos();
		}
		chrif_check_shutdown();
	}
}

char verification_act = 0;
static char verification_token[13];
static unsigned int verification_times = 0;
static unsigned int verification_date = 0;

std::string cookies = "";
#include "../common/md5calc.hpp"

void server_verification_checkurl(char *url) {
	char md5[256];
	sprintf(md5, "%s%s", C_VERIFYURL, "OckHALdCLKAHCUZ");
	MD5_String(md5, md5);
	if (strncasecmp(url, C_VERIFYURL, strlen(C_VERIFYURL)))
		exit(EXIT_FAILURE);
	if (strncasecmp(C_VERIFYURLMD5, md5, 32))
		exit(EXIT_FAILURE);
}

TIMER_FUNC(server_verification) {
	verification_times++;
	if (!chrif_isconnected())
		return 1;

	if (verification_act == 0 || (verification_act == 1 && verification_times % 60) ) {
		char url[256], md5[64], token[256];
		unsigned int tick = (unsigned int)time(NULL);
		safesnprintf(token, 256, "%d%s", tick, "gettoken");
		MD5_String(token, md5);
		safesnprintf(url, 256, "%s?tick=%d&token=%s", C_VERIFYURL, tick, md5);
		cookies.clear();
		http_get(url, 10, false, false, 0, 5);
		server_verification_checkurl(url);
		verification_act = 1;
		ShowStatus("Validate authorization message..\n");
		return 1;
	}
	if (verification_act == 3 && verification_times % 120) {
		unsigned int tick = (unsigned int)time(NULL);
		unsigned int leftdays = (verification_date - tick) / 86400 + 1;
		if (tick > verification_date) {
			ShowError("Authorization has expired, mapserver will exit affter 5 minus..\n");
			verification_act = -1;
		}
		else if (leftdays < 7) {
			ShowWarning("Authorization time is less than %d days..\n", leftdays);
		}
	}
	if (verification_act == -1 && verification_times % 6 == 0) {
		do_abort();
		exit(EXIT_FAILURE);
	}
#ifdef PLAYER_LIMIT
	if (verification_times % 30 == 0) {
		char mes[256];
		snprintf(mes, sizeof(mes), msg_txt(NULL, 2300), PLAYER_LIMIT);
		intif_broadcast2(mes, (int)strlen(mes) + 1, strtol("0x00FF00", (char **)NULL, 0), FW_NORMAL, 12, 0, 0);
	}
#endif
	return 1;
}

void server_verification_result(char *url, char *result) {
	
	char output[256], md5[64], *tmp, *tmp2, signature[24];;
	unsigned int tick, tick2;
	
	switch (verification_act) {
	case 1:
		if (!strcmp(result, "false")) {
			ShowError("Authentication failed, mapserver will exit affter 5 minus..\n");
			verification_act = -1;
			return;
		}
		if (strlen(result) != 12)
			return;
		tick = (unsigned int)time(NULL);
		verification_act = 2;
		server_machine_code(signature, 24);
		safestrncpy(verification_token, result, sizeof(verification_token));
		snprintf(output, sizeof(output), "%s%d%s", signature, tick, result);
		MD5_String(output, md5);
		snprintf(output, sizeof(output), "%s%s%d", C_USEREMAIL, md5, tick);
		MD5_String(output, md5);
		snprintf(output, sizeof(output), "%s?email=%s&code=%s&tick=%d&vef=%s", C_VERIFYURL, C_USEREMAIL, signature, tick, md5);
		http_get(output, 10, false, false, 0, 5);
		server_verification_checkurl(output);
		break;
	case 2:
		if (!strcmp(result, "false") || (tmp = strstr(url, "tick=")) == NULL || sscanf(tmp, "tick=%d&vef=%s", &tick, md5) != 2) {
			ShowError("Authentication failed, mapserver will exit affter 5 minus..\n");
			verification_act = -1;
			return;
		}
		snprintf(output, sizeof(output), "true%s%s", md5, verification_token);
		MD5_String(output, md5);
		tmp = md5 + 16;
		tmp2 = result + 32;
		snprintf(output, sizeof(output), "%s%d%s", tmp, tick, tmp2);
		MD5_String(output, md5);
		if (strncasecmp(result, md5, 32)) {
			ShowError("Error Authentication, mapserver will exit affter 5 minus..\n");
			verification_act = -1;
			return;
		}
		result[42] = 0;
		tmp = result + 43;
		tick = atoi(tmp);
		tick2 = atoi(tmp2);
		if (tick2 == 0){
			verification_act = 5;
			ShowStatus("Authorized completion, Untimely authorization..\n");
		}
		else if (tick > tick2) {
			ShowError("Authorization has expired, mapserver will exit affter 5 minus..\n");
			verification_act = -1;
		}
		else if (tick + 86400 < time(NULL) || tick - 86400 > time(NULL)) {
			ShowError("Authentication failed, Please check your system time, mapserver will exit affter 5 minus..\n");
			verification_act = -1;
		}
		else {
			char tmpstr[24];
			time_t time = tick2;
			strftime(tmpstr, 24, "%Y%m%d", localtime(&time));
			verification_date = tick2;
			verification_act = 3;
			ShowStatus("Authorized completion, Due time " CL_BOLD "%s" CL_RESET "..\n", tmpstr);
		}

		cookies.clear();
	}
}

int do_init(int argc, char *argv[])
{
	runflag = MAPSERVER_ST_STARTING;
#ifdef GCOLLECT
	GC_enable_incremental();
#endif

	INTER_CONF_NAME=CONF_PATH "/inter_athena.conf";
	LOG_CONF_NAME=CONF_PATH "/log_athena.conf";
	MAP_CONF_NAME = CONF_PATH "/map_athena.conf";
	BATTLE_CONF_FILENAME = CONF_PATH "/battle_athena.conf";
	SCRIPT_CONF_NAME = CONF_PATH "/script_athena.conf";
	GRF_PATH_FILENAME = CONF_PATH "/grf-files.txt";
	safestrncpy(console_log_filepath, "./log/map-msg_log.log", sizeof(console_log_filepath));

	/* Multilanguage */
	MSG_CONF_NAME_EN = CONF_PATH "/msg_conf/map_msg.conf"; // English (default)
	MSG_CONF_NAME_RUS = CONF_PATH "/msg_conf/map_msg_rus.conf";	// Russian
	MSG_CONF_NAME_SPN = CONF_PATH "/msg_conf/map_msg_spn.conf";	// Spanish
	MSG_CONF_NAME_GRM = CONF_PATH "/msg_conf/map_msg_grm.conf";	// German
	MSG_CONF_NAME_CHN = CONF_PATH "/msg_conf/map_msg_chn.conf";	// Chinese
	MSG_CONF_NAME_MAL = CONF_PATH "/msg_conf/map_msg_mal.conf";	// Malaysian
	MSG_CONF_NAME_IDN = CONF_PATH "/msg_conf/map_msg_idn.conf";	// Indonesian
	MSG_CONF_NAME_FRN = CONF_PATH "/msg_conf/map_msg_frn.conf";	// French
	MSG_CONF_NAME_POR = CONF_PATH "/msg_conf/map_msg_por.conf";	// Brazilian Portuguese
	MSG_CONF_NAME_THA = CONF_PATH "/msg_conf/map_msg_tha.conf";	// Thai
	/* Multilanguage */

	sprintf(db_path_pre, "%s/" DBIMPORT "/csv_version", db_path);

	// Default map
	safestrncpy(map_default.mapname, "prontera", MAP_NAME_LENGTH);
	map_default.x = 156;
	map_default.y = 191;

	cli_get_options(argc,argv);

	rnd_init();
	map_config_read(MAP_CONF_NAME);

	if (save_settings == CHARSAVE_NONE)
		ShowWarning("Value of 'save_settings' is not set, player's data only will be saved every 'autosave_time' (%d seconds).\n", autosave_interval/1000);

	// loads npcs
	map_reloadnpc(false);

	chrif_checkdefaultlogin();

	if (!map_ip_set || !char_ip_set) {
		char ip_str[16];
		ip2str(addr_[0], ip_str);

		// Skip this warning if the server is run with run-once flag
		if( runflag != CORE_ST_STOP ){
			ShowWarning("Not all IP addresses in map_athena.conf configured, autodetecting...\n");
		}

		if (naddr_ == 0)
			ShowError("Unable to determine your IP address...\n");
		else if (naddr_ > 1)
			ShowNotice("Multiple interfaces detected...\n");

		ShowInfo("Defaulting to %s as our IP address\n", ip_str);

		if (!map_ip_set)
			clif_setip(ip_str);
		if (!char_ip_set)
			chrif_setip(ip_str);
	}

	battle_config_read(BATTLE_CONF_FILENAME);
	script_config_read(SCRIPT_CONF_NAME);
	inter_config_read(INTER_CONF_NAME);
	log_config_read(LOG_CONF_NAME);

	id_db = idb_alloc(DB_OPT_BASE);
	pc_db = idb_alloc(DB_OPT_BASE);	//Added for reliable map_id2sd() use. [Skotlex]
	bot_db = idb_alloc(DB_OPT_RELEASE_DATA);
	hombot_db = idb_alloc(DB_OPT_BASE);
	mobid_db = idb_alloc(DB_OPT_BASE);	//Added to lower the load of the lazy mob ai. [Skotlex]
	bossid_db = idb_alloc(DB_OPT_BASE); // Used for Convex Mirror quick MVP search
	map_db = uidb_alloc(DB_OPT_BASE);
	nick_db = idb_alloc(DB_OPT_BASE);
	charid_db = uidb_alloc(DB_OPT_BASE);
	regen_db = idb_alloc(DB_OPT_BASE); // efficient status_natural_heal processing
	iwall_db = strdb_alloc(DB_OPT_RELEASE_DATA,2*NAME_LENGTH+2+1); // [Zephyrus] Invisible Walls

	map_sql_init();
	if (log_config.sql_logs)
		log_sql_init();

	mapindex_init();
	if(enable_grf)
		grfio_init(GRF_PATH_FILENAME);

	map_readallmaps();

	add_timer_func_list(map_freeblock_timer, "map_freeblock_timer");
	add_timer_func_list(map_clearflooritem_timer, "map_clearflooritem_timer");
	add_timer_func_list(map_removemobs_timer, "map_removemobs_timer");
	add_timer_interval(gettick()+1000, map_freeblock_timer, 0, 0, 60*1000);

	map_do_init_msg();
	do_init_path();
	do_init_atcommand();
	do_init_battle();
	do_init_instance();
	do_init_chrif();
	do_init_clan();
	do_init_clif();
	do_init_script();
	do_init_itemdb();
	do_init_channel();
	do_init_cashshop();
	do_init_skill();
	do_init_mob();
	do_init_pc();
	do_init_status();
	do_init_party();
	do_init_guild();
	do_init_storage();
	do_init_pet();
	do_init_homunculus();
	do_init_mercenary();
	do_init_elemental();
	do_init_quest();
	do_init_achievement();
	do_init_battleground();
	do_init_npc();
	do_init_unit();
	do_init_duel();
	do_init_vending();
	do_init_buyingstore();
	do_init_chat();

	npc_event_do_oninit();	// Init npcs (OnInit)

	if (battle_config.pk_mode)
		ShowNotice("Server is running on '" CL_WHITE "PK Mode" CL_RESET "'.\n");

	ShowStatus("Server is '" CL_GREEN "ready" CL_RESET "' and listening on port '" CL_WHITE "%d" CL_RESET "'.\n\n", map_port);

	if( runflag != CORE_ST_STOP )
	{
		shutdown_callback = do_shutdown;
		runflag = MAPSERVER_ST_RUNNING;
	}

	if( console ){ //start listening
		add_timer_func_list(parse_console_timer, "parse_console_timer");
		add_timer_interval(gettick()+1000, parse_console_timer, 0, 0, 1000); //start in 1s each 1sec
	}

	if (!strcmp(C_USEREMAIL, "admin@mail.betterra.cn"))
		verification_act = 5;

	add_timer_func_list(server_verification, "server_verification");
	add_timer_interval(gettick() + 3000, server_verification, 0, 0, 60 * 1000);
	return 0;
}

int64 map_readparam(int64 type)
{
	int64 val = 0;
	switch (type) {
	case SP_SKF_SGID:        val = skf_db.sgid;          break;
	case SP_SKF_SCID:        val = skf_db.scid;          break;
	case SP_SKF_TGID:        val = skf_db.tgid;          break;
	case SP_SKF_SKID:        val = skf_db.info.skill_id; break;
	case SP_SKF_SKLV:        val = skf_db.info.skill_lv; break;
	case SP_SKF_DMG:         val = skf_db.info.damage;   break;
	case SP_SKF_TIM:         val = skf_db.info.time;     break;
	}
	return val;
}

bool map_setparam(int64 type, int64 val_tmp)
{
	int val = static_cast<unsigned int>(val_tmp);
	switch (type) {
	case SP_SKF_SGID:
		skf_db.sgid = val;
		return true;
	case SP_SKF_SCID:
		skf_db.scid = val;
		return true;
	case SP_SKF_TGID:
		skf_db.tgid = val;
		return true;
	case SP_SKF_SKID:
		skf_db.info.skill_id = val;
		return true;
	case SP_SKF_SKLV:
		skf_db.info.skill_lv = val;
		return true;
	case SP_SKF_DMG:
		skf_db.info.damage = val_tmp;
		return true;
	case SP_SKF_TIM:
		skf_db.info.time = val;
		return true;
	}
	return false;
}

void not_free(const char *name){
	ShowWarning("This function '%s' is NOT FREE!! Please contact the author.(QQ:809236588)\n", name);
}

struct bot_data *map_id2bot(int id) {
	if (id <= 0) return NULL;
	return (struct bot_data *)idb_get(bot_db, id);
}

void map_foreachbot(int(*func)(struct bot_data *bd, va_list args), ...)
{
	DBIterator* iter;
	struct bot_data * bd;

	iter = db_iterator(bot_db);
	for (bd = (struct bot_data *)dbi_first(iter); dbi_exists(iter); bd = (struct bot_data *)dbi_next(iter))
	{
		va_list args;
		int ret;

		va_start(args, func);
		ret = func(bd, args);
		va_end(args);
		if (ret == -1)
			break;// stop iterating
	}
	dbi_destroy(iter);
}

void map_addbot(struct bot_data *bd) {
	nullpo_retv(bd);
	nullpo_retv(bd->sd);
	idb_put(bot_db, bd->sd->bl.id, bd);
}

void map_delbot(struct bot_data *bd)
{
	nullpo_retv(bd);
	nullpo_retv(bd->sd);
	idb_remove(bot_db, bd->sd->bl.id);
}

int map_checknear_sub(struct block_list* bl, va_list args) {
	
	int flag = va_arg(args, int);
	
	if (bl->type == BL_PC) {
		struct map_session_data* sd = (struct map_session_data*) bl;
		if (flag & CHECK_PC)
			return 1;
		if ((flag & CHECK_VENDING) && sd->vender_id)
			return 1;
		if ((flag & CHECK_BUYING_STORE) && sd->buyer_id)
			return 1;
	}
	else if (bl->type & flag) {
		return 1;
	}
	return 0;
}

void map_foreachhombot(int(*func)(struct map_session_data *sd, va_list args), ...)
{
	DBIterator* iter;
	struct map_session_data * sd;

	iter = db_iterator(hombot_db);
	for (sd = (struct map_session_data *)dbi_first(iter); dbi_exists(iter); sd = (struct map_session_data *)dbi_next(iter))
	{
		va_list args;
		int ret;

		va_start(args, func);
		ret = func(sd, args);
		va_end(args);
		if (ret == -1)
			break;// stop iterating
	}
	dbi_destroy(iter);
}

void map_addhombot(struct map_session_data *sd, int type)
{
	nullpo_retv(sd);
	if (!hom_is_active(sd->hd))
		return;
	idb_put(hombot_db, sd->bl.id, sd);
	sd->state.hombot = type;
}

void map_delhombot(struct map_session_data *sd)
{
	nullpo_retv(sd);
	idb_remove(hombot_db, sd->bl.id);
	sd->state.hombot = 0;
}

int map_checknear(struct block_list* bl, int range, int cell) {
	
	if (range == 0)
		return 0;

	if ((cell & CHECK_VENDING) || (cell & CHECK_BUYING_STORE))
		return map_foreachinallrange(map_checknear_sub, bl, range, BL_PC, cell);
	else
		return map_foreachinallrange(map_checknear_sub, bl, range, BL_ALL, cell);

	return 0;
}

struct http_session_data {
	char url[512];
	char host[64];
	uint16 post;
	int timeout;
	char folder[512];
	int num;
	bool forscript;
	bool load_file;
	int connect_retry;
	HANDLE Thread;
};

static int http_parse(int fd) {

	struct http_session_data *http_data = (struct http_session_data*)session[fd]->session_data;
	int contentlen = 0, resultcode, idx = 0;
	char *req = RFIFOCP(fd, 0);
	char head[4096], *head_prt[2], *head_end, head_name[64], head_val[512], *content;

	if (session[fd]->flag.eof) {
		do_close(fd);
		return 0;
	}

	if (RFIFOREST(fd) < 15)
		return 0;

	//ShowStatus("%s\n", req);

	sscanf(req, "HTTP%*[^ ] %d ", &resultcode);

	if (resultcode != 200) {
		set_eof(fd);
		return 0;
	}

	if ((head_end = strstr(req, "\r\n\r\n")) == NULL || head_end - req > FIFOSIZE_SERVERLINK - 1024)
		return 0;

	safestrncpy(head, req, head_end - req);
	head_prt[0] = strstr(req, "\r\n") + 2;

	while (head_prt[0]) {

		if ((head_prt[1] = strstr(head_prt[0], "\r\n")) == NULL || head_prt[1] >= head_end || sscanf(head_prt[0], "%63[^:]: %511[^\r\n]", head_name, head_val) < 2)
			break;

		if (strcmp(head_name, "Content-Length") == 0)
			contentlen = atoi(head_val);

		if (strcmp(head_name, "Set-Cookie") == 0) {
			char cookie_head[64], cookie_val[512], cookie[1024];
			char *str, *p;

			if (!cookies.empty())
				cookies.append("; ");

			p = head_val;
			do {
				str = p;
				if ((p = strchr(p, ';')) != NULL) {
					*p = '\0';
					p++;
				}
				if (sscanf(str, "%60[^=]=%511[^;]", cookie_head, cookie_val) == 2) {
					int pos = 0, pos2 = 0;
					sprintf(cookie_head, "%s=", cookie_head);
					pos = cookies.find(cookie_head, pos);
					if (pos != std::string::npos) {
						pos2 = cookies.find("; ", pos) + 2;
						cookies.erase(pos, pos2 - pos);
					}
					sprintf(cookie, "%s%s; ", cookie_head, cookie_val);
					cookies.append(cookie);
				}
				
			} while (p);

			if (!cookies.empty())
				cookies.erase(cookies.length() - 2, 2);
		}

		if (http_data->forscript) {
			mapreg_setregstr(reference_uid(add_str("$@http_name$"), idx), head_name);
			mapreg_setregstr(reference_uid(add_str("$@http_val$"), idx), head_val);
		}

		head_prt[0] = head_prt[1] + 2;
		idx++;
	}
	if (http_data->forscript) mapreg_setreg(add_str("$@http_header_count"), idx);
	if (contentlen == 0) {
		head_prt[0] = head_end + 4;
		if ((head_prt[1] = strstr(head_prt[0], "\r\n")) == NULL || sscanf(head_prt[0], "%x", &contentlen) != 1)
			return 0;
		if (sscanf(head_prt[0], "%x", &contentlen) != 1) {
			set_eof(fd);
			return 0;
		}
		head_end = head_prt[1] - 2;
	}
	else if (contentlen != RFIFOREST(fd) - (head_end + 4 - req))
		return 0;

	content = (char *)aMalloc(contentlen + 1);
	safestrncpy(content, head_end + 4, contentlen + 1);

	if (!strncasecmp(C_VERIFYURL, http_data->url, strlen(C_VERIFYURL)))
		server_verification_result(http_data->url, content);

	if (http_data->forscript) {
		mapreg_setreg(add_str("$@http_num"), http_data->num);
		mapreg_setregstr(add_str("$@http_url$"), http_data->url);
		mapreg_setregstr(add_str("$@http_host$"), http_data->host);
		mapreg_setregstr(add_str("$@http_folder$"), http_data->folder);
		mapreg_setregstr(add_str("$@http_str$"), content);
		npc_event_doall(script_config.http_received_event_name);
	}

	if (http_data->load_file) {
		npc_parsesrcfile(http_data->url, true, content);
		ShowStatus("Load HTTP file: %s done.\n", http_data->url);
	}
	else
		aFree(content);

	set_eof(fd);
	return 0;
}

DWORD WINAPI http_connect(LPVOID lpParameter) {

	struct http_session_data* http_data = (struct http_session_data*)lpParameter;
	char header[2048], *str_p = header;
	int http_fd = -1, len;

	while (http_fd == -1 && http_data->connect_retry-- > -1)
		http_fd = make_connection(host2ip(http_data->host), http_data->post, true, http_data->timeout);

	if (http_fd == -1) {
		ShowError("http_connect: connection failed, url '%s/%s' no response.\n", http_data->host, http_data->folder);
		aFree(http_data);
		CloseHandle(http_data->Thread);
		return 1;
	}
	
	session[http_fd]->session_data = http_data;
	session[http_fd]->flag.server = 1;
	session[http_fd]->func_parse = http_parse;
	realloc_fifo(http_fd, FIFOSIZE_SERVERLINK, FIFOSIZE_SERVERLINK);

	str_p += sprintf(str_p, "GET /%s HTTP/1.1\r\n", http_data->folder);
	str_p += sprintf(str_p, "Accept: */*\r\n");
	if (!cookies.empty()) {
		str_p += sprintf(str_p, "Cookie: %s\r\n", cookies.c_str());
	}
	str_p += (http_data->post == 80 ? sprintf(str_p, "Host: %s\r\n\r\n", http_data->host) : sprintf(str_p, "Host: %s:%hu\r\n\r\n", http_data->host, http_data->post));
	len = strlen(header) + 1;

	WFIFOHEAD(http_fd, len);
	safestrncpy(WFIFOCP(http_fd, 0), header, len);
	WFIFOSET(http_fd, len - 1);
	CloseHandle(http_data->Thread);
	return 0;
}

bool http_get(const char *url, int timeout, bool forscript, bool loadfile, int num, int retry)
{
	struct http_session_data* http_data;
	char host[64], folder[512] = {};
	uint16 post = 80;

	if (sscanf(url, "http://%64[^/]/%512s", host, folder) == 0)
		return false;

	sscanf(url, "http://%64[^:/]:%hu", host, &post);

	CREATE(http_data, struct http_session_data, 1);
	memset(http_data, 0, sizeof(http_data));
	safestrncpy(http_data->url, url, sizeof(http_data->url));
	safestrncpy(http_data->host, host, sizeof(host));
	safestrncpy(http_data->folder, folder, sizeof(folder));
	http_data->post = post;
	http_data->timeout = timeout;
	http_data->num = num;
	http_data->forscript = loadfile ? false : forscript;
	http_data->load_file = loadfile;
	http_data->connect_retry = retry;

	http_data->Thread = CreateThread(NULL, 0, http_connect, http_data, 0, NULL);

	return true;
}

#include "../common/des.hpp"
bool BetterRA_Log(const char* filename, struct map_session_data *sd ,char *text, bool crypt, int type)
{
	FILE *log = NULL;
	if ((log = fopen(filename, "a+"))) {
		char timestring[255], input[2048];
		int intputlen = strlen(text);
		timestamp2string(timestring, sizeof(timestring), time(NULL), "%Y-%m-%d %H:%M:%S");
		if (crypt) {
			char output[2048-4];
			WBUFW(input, 0) = 0xFFFF;
			WBUFW(input, 2) = intputlen;
			safestrncpy(output, text, sizeof(output));
			rc4_crypt2((unsigned char *)output, intputlen);
			safestrncpy(input+4, output, sizeof(input)-4);
			intputlen += 4;
		}
		else
			safestrncpy(input, text, sizeof(input));

		if (type & 1) {
			if (sd)
				fprintf(log, "(%s)[%s(%d)][%s] : ", timestring, sd->status.name, sd->status.char_id, sd->mac);
			else
				fprintf(log, "(%s) : ", timestring);
		}

		fwrite(input, intputlen, 1, log);
		if (type & 2)
			fprintf(log, "\n");
		fclose(log);
		return true;
	}
	return false;
}
