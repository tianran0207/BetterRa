#pragma once

#ifndef _ROBROSER_HPP_
#define _ROBROSER_HPP_

#include "websocket.hpp"

struct RoBrowser_Config {
	bool enable;
	char bind_ip_str[128];
	uint32 bind_ip;
	uint16 robrowser_port;
};
extern struct RoBrowser_Config robrowser_config;

struct robrowser_session_data {
	int websocket_fd;
	int map_fd;
};

void do_init_robrowser(void);
void do_final_robrowser(void);

#endif //_ROBROSER_HPP_
