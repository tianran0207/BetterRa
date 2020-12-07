// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//
#include <stdlib.h>

#include "../common/core.hpp"
#include "../common/malloc.hpp"
#include "../common/showmsg.hpp"
#include "../common/sql.hpp"

#include "websocket.hpp"
#include "robrowser.hpp"

struct RoBrowser_Config robrowser_config;

int robrowser_fd = -1;

int robrowser_agent(int from_fd, int to_fd, char *buf, size_t len)
{
	if (len == 0)
		return 0;

	if (buf != NULL) {
		WFIFOHEAD(to_fd, len);// sends a ping packet to login server (will receive pong 0x2718)
		memcpy(WFIFOCP(to_fd, 0), buf, len);
		WFIFOSET(to_fd, len);
	}
	else {
		char *sendbuf = (char *)aMalloc(sizeof(char) * (len + 1));
		memcpy(sendbuf, RFIFOCP(from_fd, 0), len);
		websocket_send(to_fd, sendbuf, len, BINARY_FRAME);
		aFree(sendbuf);
	}
		

	return 1;
}

int robrowser_mapif_parse(int fd)
{
	struct robrowser_session_data* sd = (struct robrowser_session_data*)session[fd]->session_data;
	if (session[fd]->flag.eof) {
		if (sd)
			set_eof(sd->websocket_fd);
		do_close(fd);
		return 0;
	}
	while (RFIFOREST(fd) >= 2) {
		robrowser_agent(fd, sd->websocket_fd, NULL, RFIFOREST(fd));
		RFIFOSKIP(fd, RFIFOREST(fd));
	}

	RFIFOFLUSH(fd);
	return 0;
}

static int robrowser_parse_connection(int fd, const char *line)
{
	struct robrowser_session_data* sd;
	char ip_str[128];
	int port;
	if (sscanf(line, "%127[^:]:%d", ip_str, &port) != 2)
		return 0;
	int map_fd = make_connection(host2ip(ip_str), port, false, 1);
	if (map_fd == -1)
		return 0;
	session[map_fd]->func_parse = robrowser_mapif_parse;
	realloc_fifo(map_fd, FIFOSIZE_SERVERLINK, FIFOSIZE_SERVERLINK);
	CREATE(session[map_fd]->session_data, struct robrowser_session_data, 1);
	sd = (struct robrowser_session_data*)session[map_fd]->session_data;
	sd->websocket_fd = fd;
	sd->map_fd = map_fd;
	CREATE(session[fd]->session_data, struct robrowser_session_data, 1);
	sd = (struct robrowser_session_data*)session[fd]->session_data;
	sd->websocket_fd = fd;
	sd->map_fd = map_fd;
	return 1;
}

static int robrowser_parse_receive(int fd)
{
	struct robrowser_session_data* sd = (struct robrowser_session_data*)session[fd]->session_data;
	int packetlen;
	static char decoded[2048];
	unsigned char *buf = (unsigned char *)RFIFOCP(fd, 0);
	memcpy(decoded, buf, RFIFOREST(fd));

	if (sd == NULL) {
		set_eof(fd);
		return 0;
	}

	if ((packetlen = (int)websocket_decode(fd, buf, decoded, 2048)) == 0)
		return 1;
	else
		RFIFOSKIP(fd, packetlen);

	robrowser_agent(fd, sd->map_fd, decoded, packetlen - 6);

	return 1;
}

int robrowserclif_parse(int fd) {
	struct robrowser_session_data* sd = (struct robrowser_session_data*)session[fd]->session_data;
	uint32 ipl = session[fd]->client_addr;
	size_t packet_len = 0;

	if (session[fd]->flag.eof) {
		if (sd)
			session[sd->map_fd]->flag.eof;
		do_close(fd);
		return 0;
	}
	if (RFIFOREST(fd) > 2048) {
		ShowWarning("chat_mapif_parse: Received packet too long packet_len (%d), disconnecting session #%d.\n", RFIFOREST(fd), fd);
		set_eof(fd);
		return 0;
	}
	while (RFIFOREST(fd) >= 2) {
		int next = 1;
		switch (RFIFOB(fd, 0)) {
		case 'G': next = websocket_websocket_check(fd, robrowser_parse_connection); break;
		default:  next = robrowser_parse_receive(fd); break;
		}
		if (next == 0)
			return 0;
	}

	RFIFOFLUSH(fd);
	return 0;
}

void do_init_robrowser(void)
{
	if (!robrowser_config.enable)
		return;

	set_defaultparse(robrowserclif_parse);

	if ((robrowser_fd = make_listen_bind(robrowser_config.bind_ip, robrowser_config.robrowser_port)) == -1) {
		ShowFatalError("Failed to bind to port '" CL_WHITE "%d" CL_RESET "'\n", robrowser_config.robrowser_port);
		exit(EXIT_FAILURE);
	}

	ShowStatus("The chat-server is " CL_GREEN "ready" CL_RESET " (Server is listening on the port %d).\n", robrowser_config.robrowser_port);
}

void do_final_robrowser(void)
{
	if (robrowser_fd != -1) {
		do_close(robrowser_fd);
		robrowser_fd = -1;
	}
}
