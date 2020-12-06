#pragma once

#ifndef _CHAT_HPP_
#define _CHAT_HPP_

#include "websocket.hpp"

struct ChatServ_Config {
	bool enable;
	char userid[NAME_LENGTH];
	char passwd[NAME_LENGTH];
	char map_ip_str[128];
	uint32 map_ip;
	uint16 map_port;
	char bind_ip_str[128];
	uint32 bind_ip;
	uint16 chat_port;
	uint32 listen_message;
	uint32 listen_console;
	int console_atcommand;
	uint32 global_min_group;
	uint32 global_min_reg_time;
	char global_world_clan[NAME_LENGTH];
	char web_client_prefix[NAME_LENGTH];
	char global_color[12];
	uint16 global_deley;
	int display_type;
};
extern struct ChatServ_Config chatserv_config;

struct chat_session_data {
	bool auth; // whether the session is authed or not
	uint32 account_id;
	uint32 char_id;
	uint32 party_id;
	uint32 guild_id;
	uint32 group_id;
	char nickname[NAME_LENGTH];

	int web_count;
	t_tick beattick;
	t_tick global_tick;
	unsigned int state;
	time_t unban_time;
	time_t expiration_time;

	int fd;
};

int chat_split_auth(int fd, const char *line);

void do_init_chat(void);
void do_final_chat(void);

#endif //_CHAT_H_
