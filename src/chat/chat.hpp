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
	char userid[24];
	char passwd[24];
	char map_ip_str[128];
	uint32 map_ip;
	uint16 map_port;
	char chat_ip_str[128];
	uint32 chat_ip;
	char bind_ip_str[128];
	uint32 bind_ip;
	uint16 chat_port;
	uint16 global_deley;
	uint16 public_deley;
	int display_type;
};
extern struct CharServ_Config charserv_config;

struct chat_session_data {
	bool auth; // whether the session is authed or not
	uint32 account_id;
	uint32 char_id;
	uint32 party_id;
	uint32 guild_id;
	char nickname[NAME_LENGTH];

	t_tick beattick;
	t_tick global_tick;
	t_tick public_tick;
	t_tick unban_time;
	t_tick ban_tick;

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
