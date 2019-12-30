#pragma once

#ifndef _CHAT_HPP_
#define _CHAT_HPP_

#include "../common/cbasetypes.hpp"
#include "../common/mmo.hpp"
#include "support/base64.h"
#include "support/sha1.h"
#include "support/transcoding.h"

int map_fd = -1; //login file descriptor
int chat_fd = -1; //char file descriptor

struct ChatServ_Config {
	char userid[NAME_LENGTH];
	char passwd[NAME_LENGTH];
	char map_ip_str[128];
	uint32 map_ip;
	uint16 map_port;
	char chat_ip_str[128];
	uint32 chat_ip;
	char bind_ip_str[128];
	uint32 bind_ip;
	uint16 chat_port;
	uint32 listen_group;
	uint32 global_min_group;
	uint32 global_min_reg_time;
	char global_world_clan[NAME_LENGTH];
	char global_prefix[NAME_LENGTH];
	char global_color[12];
	uint16 global_deley;
	int display_type;
};
extern struct CharServ_Config charserv_config;

struct chat_session_data {
	bool auth; // whether the session is authed or not
	uint32 account_id;
	uint32 char_id;
	uint32 party_id;
	uint32 guild_id;
	uint32 group_id;
	char nickname[NAME_LENGTH];

	t_tick beattick;
	t_tick global_tick;
	unsigned int state;
	time_t unban_time;
	time_t expiration_time;

	int fd;
	uint64 skiplen;
};

struct online_chat_data {
	uint32 account_id;
	uint32 char_id;
	uint32 party_id;
	uint32 guild_id;
	char nickname[NAME_LENGTH];
	int fd;
};

#endif //_CHAT_H_
