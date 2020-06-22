#pragma once

#ifndef _WEBSOCKET_HPP_
#define _WEBSOCKET_HPP_

#include "../common/cbasetypes.hpp"
#include "../common/strlib.hpp"
#include "../common/socket.hpp"
#include "../common/mmo.hpp"
#include "support/base64.h"
#include "support/sha1.h"
#include "support/transcoding.h"

enum WSFrameType {
	ERROR_FRAME = 0xFF00,
	INCOMPLETE_FRAME = 0xFE00,

	OPENING_FRAME = 0x3300,
	CLOSING_FRAME = 0x3400,

	INCOMPLETE_TEXT_FRAME = 0x01,
	INCOMPLETE_BINARY_FRAME = 0x02,

	TEXT_FRAME = 0x81,
	BINARY_FRAME = 0x82,

	PING_FRAME = 0x19,
	PONG_FRAME = 0x1A
};

#define CHAT_MAX_MSG 300	//max number of msg_conf
const char* websocket_msg_txt(int msg_number);

int websocket_decode(int fd, unsigned char *buf, char *decoded, int maxlen);
int websocket_send(int fd, char *buf, int len, enum WSFrameType type);
int websocket_send_str(int fd, char *buf);
int websocket_websocket_check(int fd, int(*func)(int, const char *));

#endif //_WEBSOCKET_HPP_
