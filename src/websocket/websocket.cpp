// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//
#include <stdlib.h>

#include "support/base64.h"
#include "support/sha1.h"
#include "support/transcoding.h"
#include "../common/showmsg.hpp"
#include "../common/msg_conf.hpp"

#include "chat.hpp"
#include "robrowser.hpp"

static char* msg_table[CHAT_MAX_MSG]; // Login Server messages_conf

int websocket_msg_read(const char *cfgName) {
	return _msg_config_read(cfgName, CHAT_MAX_MSG, msg_table);
}
const char* websocket_msg_txt(int msg_number) {
	return _msg_txt(msg_number, CHAT_MAX_MSG, msg_table);
}
void websocket_msg_final(void) {
	_do_final_msg(CHAT_MAX_MSG, msg_table);
}

static bool websocket_config_read(const char* cfgName) {
	char line[1024], w1[1024], w2[1024];
	FILE* fp = fopen(cfgName, "r");

	if (fp == NULL) {
		ShowError("Configuration file not found: %s.\n", cfgName);
		return false;
	}

	while (fgets_ex(line, sizeof(line), fp)) {
		if (line[0] == '/' && line[1] == '/')
			continue;

		if (sscanf(line, "%1023[^:]: %1023[^\r\n]", w1, w2) != 2)
			continue;

		remove_control_chars(w1);
		remove_control_chars(w2);

		if (strcmpi(w1, "bind_ip") == 0) {
			chatserv_config.bind_ip = host2ip(w2);
			robrowser_config.bind_ip = host2ip(w2);
			if (chatserv_config.bind_ip) {
				char ip_str[16];
				safestrncpy(chatserv_config.bind_ip_str, w2, sizeof(chatserv_config.bind_ip_str));
				safestrncpy(robrowser_config.bind_ip_str, w2, sizeof(robrowser_config.bind_ip_str));
				ShowStatus("WebSocket server binding IP address : %s -> %s\n", w2, ip2str(chatserv_config.bind_ip, ip_str));
			}
		}
		else if (strcmpi(w1, "robrowser_agent_enable") == 0) {
			robrowser_config.enable = config_switch(w2);
		}
		else if (strcmpi(w1, "robrowser_port") == 0) {
			robrowser_config.robrowser_port = atoi(w2);
		}
		else if (strcmpi(w1, "chat_server_enable") == 0) {
			chatserv_config.enable = config_switch(w2);
		}
		else if (strcmpi(w1, "chat_port") == 0) {
			chatserv_config.chat_port = atoi(w2);
		}
		else if (strcmpi(w1, "map_ip") == 0) {
			chatserv_config.map_ip = host2ip(w2);
			if (chatserv_config.map_ip) {
				char ip_str[16];
				safestrncpy(chatserv_config.map_ip_str, w2, sizeof(chatserv_config.map_ip_str));
				ShowStatus("Map server IP address : %s -> %s\n", w2, ip2str(chatserv_config.map_ip, ip_str));
			}
		}
		else if (strcmpi(w1, "map_port") == 0) {
			chatserv_config.map_port = atoi(w2);
		}
		else if (strcmpi(w1, "listen_group") == 0) {
			chatserv_config.listen_group = atoi(w2);
		}
		else if (strcmpi(w1, "global_min_group") == 0) {
			chatserv_config.global_min_group = atoi(w2);
		}
		else if (strcmpi(w1, "global_min_reg_time") == 0) {
			chatserv_config.global_min_reg_time = atoi(w2);
		}
		else if (strcmpi(w1, "global_world_clan") == 0) {
			safestrncpy(chatserv_config.global_world_clan, w2, sizeof(chatserv_config.global_world_clan));
		}
		else if (strcmpi(w1, "web_client_prefix") == 0) {
			safestrncpy(chatserv_config.web_client_prefix, w2, sizeof(chatserv_config.web_client_prefix));
		}
		else if (strcmpi(w1, "global_color") == 0) {
			safestrncpy(chatserv_config.global_color, w2, sizeof(chatserv_config.global_color));
		}
		else if (strcmpi(w1, "global_deley") == 0) {
			chatserv_config.global_deley = atoi(w2);
		}
		else if (strcmpi(w1, "display_type") == 0) {
			chatserv_config.display_type = atoi(w2);
		}
		else if (strcmpi(w1, "userid") == 0) {
			safestrncpy(chatserv_config.userid, w2, sizeof(chatserv_config.userid));
		}
		else if (strcmpi(w1, "passwd") == 0) {
			safestrncpy(chatserv_config.passwd, w2, sizeof(chatserv_config.passwd));
		}
	}

	fclose(fp);

	ShowInfo("Done reading %s.\n", cfgName);
	return true;
}

int websocket_decode(int fd, unsigned char *buf, char *decoded, int maxlen)
{
	unsigned char Uint64byte[8], masks[4];
	uint64 len, packetlen;
	int i;
	unsigned char *data;

	len = buf[1] & 127;
	switch (len) {
	case 126:
		len = (uint16)(buf[2] << 8 | buf[3]);
		packetlen = len + 8;
		memcpy(masks, buf + 4, 4);
		data = buf + 8;
		break;
	case 127:
		for (i = 0; i < 8; i++)
			Uint64byte[i] = buf[9 - i];
		len = *(uint64 *)Uint64byte;
		packetlen = len + 14;
		memcpy(masks, buf + 10, 4);
		data = buf + 14;
		break;
	default:
		memcpy(masks, buf + 2, 4);
		packetlen = len + 6;
		data = buf + 6;
		break;
	}

	for (i = 0; i < len && i < maxlen; i++)
		decoded[i] = data[i] ^ masks[i % 4];

	decoded[i] = '\0';

	return (int)packetlen;
}

int websocket_send(int fd, char *buf, int len, enum WSFrameType type)
{
	static unsigned char temp[20480];
	temp[0] = (char)type;
	if (len <= 125) {
		temp[1] = (char)len;
		memcpy(temp + 2, buf, len);
		len += 2;
	}
	else if (len <= 65535) {
		temp[1] = (char)126;
		temp[2] = (char)((len >> 8) & 0xFF);
		temp[3] = (char)(len & 0xFF);
		memcpy(temp + 4, buf, len);
		len += 4;
	}
	else {
		temp[1] = (char)127;
		for (int i = 2; i < 5; i++)
			temp[i] = (char)0;
		for (int i = 5; i < 8; i++)
			temp[i] = ((char)((len >> 8 * i) & 0xFF));
		memcpy(temp + 8, buf, len);
		len += 8;
	}
	if (!session[fd] || !session[fd]->session_data)
		return 1;
	WFIFOHEAD(fd, len);
	memcpy(WFIFOCP(fd, 0), temp, len);
	WFIFOSET(fd, len);
	return 1;
}

int websocket_send_str(int fd, char *buf)
{
	char sendbuf[2048];
	if (!gbk2utf8(buf, (char *)sendbuf, 2048))
		return 1;

	return websocket_send(fd, sendbuf, strlen(sendbuf), TEXT_FRAME);
}

static char* websocket_head_line(char** buf) {
	char *pos, *line;
	pos = strchr(*buf, '\r\n');
	if (pos == NULL)
		return NULL;
	*pos = '\0'; *(pos + 1) = '\0';
	line = *buf; *buf = pos + 2;
	return line;
}

const char handshake[] = "HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Accept: %s\r\n\r\n";
int websocket_websocket_check(int fd, int(*func)(int, const char *))
{
	int result = 0, len;
	char *buf, *line;
	char *seckey = NULL;
	char accept[128];
	SHA1_CTX sha;
	uint8_t sha1code[20];
	char reply[1024];

	if (RFIFOREST(fd) < 3 || RFIFOREST(fd) > 2048)
		return 1;

	buf = RFIFOCP(fd, 0);
	if (strstr(buf, "GET") != buf || (line = strstr(buf, "\r\n\r\n")) == NULL)
		return 0;

	RFIFOSKIP(fd, line - buf + 4);

	while (*line != '\0')
	{
		line = websocket_head_line(&buf);

		if (buf == NULL || line == NULL)
			break;

		if (!strnicmp(line, "GET", 3)) {
			char temp[1024], *tempp;
			line = line + 5;
			tempp = temp;
			if ((tempp = strchr(line, ' ')) == NULL) {
				set_eof(fd);
				return 0;
			}
			*tempp = '\0';
			if (!func(fd, line)) {
				set_eof(fd);
				return 0;
			}
			result++;
		}
		else if (!strnicmp(line, "Connection", 10)) {
			line = strchr(line, ':') + 2;
			if (line && !strcmp(line, "Upgrade"))
				result++;
		}
		else if (!strnicmp(line, "Upgrade", 7)) {
			line = strchr(line, ':') + 2;
			if (line && !strcmp(line, "websocket"))
				result++;
		}
		else if (!strnicmp(line, "Sec-WebSocket-Version", 21)) {
			line = strchr(line, ':') + 2;
			if (line && !strcmp(line, "13"))
				result++;
		}
		else if (!strnicmp(line, "Sec-WebSocket-Key", 17)) {
			seckey = strchr(line, ':') + 2;
		}
	}

	if (result != 4 || seckey == NULL || seckey[0] == '\0')
		return 0;

	sprintf(seckey, "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", seckey);

	len = strlen(seckey);
	SHA1Init(&sha);
	SHA1Update(&sha, (uint8_t *)seckey, len);
	SHA1Final(sha1code, &sha);
	base64_encode(sha1code, sizeof(sha1code), accept);
	sprintf(reply, handshake, accept);
	len = strlen(reply);
	WFIFOHEAD(fd, len);
	memcpy(WFIFOP(fd, 0), reply, len);
	WFIFOSET(fd, len);
	return 1;
}

int do_init(int argc, char** argv)
{
	const char* MSG_CONF_NAME_EN = CONF_PATH "/msg_conf/BetterRa_websocket_msg.conf";
	const char* CHAT_CONF_NAME = CONF_PATH "/websocket_athena.conf";
	memset(&chatserv_config, 0, sizeof(chatserv_config));

	chatserv_config.enable = false;
	chatserv_config.map_ip = 0;
	chatserv_config.map_port = 5121;
	chatserv_config.bind_ip = INADDR_ANY;
	chatserv_config.chat_port = 8080;
	safestrncpy(chatserv_config.userid, "s1", sizeof(chatserv_config.userid));
	safestrncpy(chatserv_config.passwd, "p1", sizeof(chatserv_config.passwd));
	safestrncpy(chatserv_config.global_color, "0xFFFFFF", sizeof(chatserv_config.global_color));
	websocket_msg_read(MSG_CONF_NAME_EN);
	websocket_config_read(CHAT_CONF_NAME);
	do_init_chat();
	do_init_robrowser();
	
	return 0;
}

void do_abort(void)
{
}

void set_server_type(void)
{
}

void do_final(void)
{
	do_final_chat();
	do_final_robrowser();
	websocket_msg_final();
}
