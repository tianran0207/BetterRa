// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//
#include <stdlib.h>

#include "../common/cbasetypes.hpp"
#include "../common/core.hpp"
#include "../common/db.hpp"
#include "../common/malloc.hpp"
#include "../common/random.hpp"
#include "../common/showmsg.hpp"
#include "../common/socket.hpp"
#include "../common/strlib.hpp"
#include "../common/timer.hpp"
#include "../common/sql.hpp"
#include "../common/cli.hpp"

#include "chat.hpp"

struct ChatServ_Config chatserv_config;
DBMap* online_chat_db;
Sql* sql_handle = NULL;

int chat_server_port = 3306;
char chat_server_ip[32] = "127.0.0.1";
char chat_server_id[32] = "ragnarok";
char chat_server_pw[32] = ""; // Allow user to send empty password (bugreport:7787)
char chat_server_db[32] = "ragnarok";
char default_codepage[32] = ""; //Feature by irmin.

bool chat_config_read(const char* cfgName, bool issql) {
	char line[1024], w1[1024], w2[1024];
	FILE* fp = fopen(cfgName, "r");

	if (fp == NULL) {
		ShowError("Configuration file not found: %s.\n", cfgName);
		return false;
	}

	while (fgets(line, sizeof(line), fp)) {
		if (line[0] == '/' && line[1] == '/')
			continue;

		if (sscanf(line, "%1023[^:]: %1023[^\r\n]", w1, w2) != 2)
			continue;

		remove_control_chars(w1);
		remove_control_chars(w2);
		
		if (issql) {
			if (strcmpi(w1, "map_server_ip") == 0)
				strcpy(chat_server_ip, w2);
			else if (strcmpi(w1, "map_server_port") == 0)
				chat_server_port = atoi(w2);
			else if (strcmpi(w1, "map_server_id") == 0)
				strcpy(chat_server_id, w2);
			else if (strcmpi(w1, "map_server_pw") == 0)
				strcpy(chat_server_pw, w2);
			else if (strcmpi(w1, "map_server_db") == 0)
				strcpy(chat_server_db, w2);
			else if (strcmpi(w1, "default_codepage") == 0)
				strcpy(default_codepage, w2);
		}else if (strcmpi(w1, "map_ip") == 0) {
			chatserv_config.map_ip = host2ip(w2);
			if (chatserv_config.map_ip) {
				char ip_str[16];
				safestrncpy(chatserv_config.map_ip_str, w2, sizeof(chatserv_config.map_ip_str));
				ShowStatus("Login server IP address : %s -> %s\n", w2, ip2str(chatserv_config.map_ip, ip_str));
			}
		}else if (strcmpi(w1, "map_port") == 0) {
			chatserv_config.map_port = atoi(w2);
		}else if (strcmpi(w1, "chat_ip") == 0) {
			chatserv_config.chat_ip = host2ip(w2);
			if (chatserv_config.chat_ip) {
				char ip_str[16];
				safestrncpy(chatserv_config.chat_ip_str, w2, sizeof(chatserv_config.chat_ip_str));
				ShowStatus("Character server IP address : %s -> %s\n", w2, ip2str(chatserv_config.chat_ip, ip_str));
			}
		}else if (strcmpi(w1, "bind_ip") == 0) {
			chatserv_config.bind_ip = host2ip(w2);
			if (chatserv_config.bind_ip) {
				char ip_str[16];
				safestrncpy(chatserv_config.bind_ip_str, w2, sizeof(chatserv_config.bind_ip_str));
				ShowStatus("Character server binding IP address : %s -> %s\n", w2, ip2str(chatserv_config.bind_ip, ip_str));
			}
		}else if (strcmpi(w1, "chat_port") == 0) {
			chatserv_config.chat_port = atoi(w2);
		}else if (strcmpi(w1, "global_deley") == 0) {
			chatserv_config.global_deley = atoi(w2);
		}else if (strcmpi(w1, "public_deley") == 0) {
			chatserv_config.public_deley = atoi(w2);
		}else if (strcmpi(w1, "display_type") == 0) {
			chatserv_config.display_type = atoi(w2);
		}else if (strcmpi(w1, "userid") == 0) {
			safestrncpy(chatserv_config.userid, w2, sizeof(chatserv_config.userid));
		}else if (strcmpi(w1, "passwd") == 0) {
			safestrncpy(chatserv_config.passwd, w2, sizeof(chatserv_config.passwd));
		}
	}

	fclose(fp);

	ShowInfo("Done reading %s.\n", cfgName);
	return true;
}

static char* gethttpheadline(char** buf) {
	char *pos, *line;
	pos = strchr(*buf, '\r\n');
	if (pos == NULL)
		return NULL;
	*pos = '\0'; *(pos + 1) = '\0';
	line = *buf; *buf = pos + 2;
	return line;
}

static uint64 recv_decode(int fd, unsigned char *buf, char *decoded) {
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
	if (packetlen > (uint64)session[fd]->max_rdata) {
		struct chat_session_data* sd = (struct chat_session_data*)session[fd]->session_data;
		sd->skiplen = packetlen - session[fd]->rdata_size;
		RFIFOSKIP(fd, session[fd]->rdata_size);
		return 0;
	}

	for (i = 0; i < len; i++)
		decoded[i] = data[i] ^ masks[i % 4];
	decoded[i] = '\0';

	return packetlen;
}

static int send_message(int fd, char *buf) {
	unsigned char sendbuf[2048],temp[2048];
	int lenght;
	gbk2utf8(buf, (char *)sendbuf, 2048);

	lenght = strlen((char *)sendbuf);
	temp[0] = 0x81;
	if (lenght < 125) {
		temp[1] = lenght;
		memcpy(temp + 2, sendbuf, lenght);
		lenght += 2;
	}
	else {
		temp[1] = 126;
		temp[2] = lenght >> 8;
		temp[3] = lenght & 255;
		memcpy(temp + 4, sendbuf, lenght);
		lenght += 4;
	}
	WFIFOHEAD(fd, lenght);
	memcpy(WFIFOCP(fd, 0), temp, lenght);
	WFIFOSET(fd, lenght);
	return 1;
}

static bool chat_auth_ok(int fd, const char *nickname, const char *password) {
	char ip[16];
	char* data;
	struct chat_session_data *sd;
	struct online_chat_data *olsd;
	int char_id, account_id, party_id, guild_id;
	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `char_id`,`account_id`,`party_id`,`guild_id` FROM `char` WHERE `name` = '%s' LIMIT 1", nickname) || SQL_SUCCESS != Sql_NextRow(sql_handle)) {
		ShowInfo("chat_login: Nickname '%s' does not exist.\n", nickname);
		Sql_FreeResult(sql_handle);
		return false;
	}
	else {
		Sql_GetData(sql_handle, 0, &data, NULL); char_id = strtoul(data, NULL, 10);
		Sql_GetData(sql_handle, 1, &data, NULL); account_id = strtoul(data, NULL, 10);
		Sql_GetData(sql_handle, 2, &data, NULL); party_id = strtoul(data, NULL, 10);
		Sql_GetData(sql_handle, 3, &data, NULL); guild_id = strtoul(data, NULL, 10);
		Sql_FreeResult(sql_handle);
	}

	if (uidb_exists(online_chat_db, account_id))
		return false;

	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `unban_time`,`group_id` FROM `login` WHERE `account_id` = %d AND `user_pass` = '%s' LIMIT 1", account_id, password) || SQL_SUCCESS != Sql_NextRow(sql_handle)) {
		ShowInfo("chat_login: The password for the nickname(%s) is incorrect.\n", nickname);
		return false;
	}

	CREATE(session[fd]->session_data, struct chat_session_data, 1);
	sd = (struct chat_session_data*)session[fd]->session_data;
	sd->auth = true;
	sd->account_id = account_id;
	sd->char_id = char_id;
	sd->party_id = party_id;
	sd->guild_id = guild_id;
	sd->beattick = gettick();
	sd->fd = fd;
	safestrncpy(sd->nickname, nickname, NAME_LENGTH);

	Sql_GetData(sql_handle, 0, &data, NULL); sd->unban_time = strtoul(data, NULL, 10);
	Sql_GetData(sql_handle, 0, &data, NULL); sd->unban_time = strtoul(data, NULL, 10);
	Sql_FreeResult(sql_handle);

	CREATE(olsd, struct online_chat_data, 1);
	olsd->account_id = account_id;
	olsd->char_id = char_id;
	olsd->party_id = party_id;
	olsd->guild_id = guild_id;
	safestrncpy(olsd->nickname, nickname, NAME_LENGTH);
	olsd->fd = fd;

	uidb_put(online_chat_db, account_id, olsd);
	ip2str(session[fd]->client_addr, ip);
	ShowStatus("Authentication accepted (account: %s, id: %d, ip: %s)\n", nickname , account_id, ip);
	return true;
}

const char handshake[] = "HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Accept: %s\r\n\r\n";
static int inclif_parse_websocket(int fd) {
	int result = 0, len;
	char *buf, *line;
	char *seckey = NULL, *name;
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

	while (*line != '\0') {
		line = gethttpheadline(&buf);

		if (buf == NULL || line == NULL)
			break;

		if (!strnicmp(line, "GET", 3)) {
			char temp[64];
			char *password, *tempp;

			line = line + 5; 
			name = temp;
			if ((tempp = strchr(line, ' ')) == NULL) {
				set_eof(fd); 
				return 0; 
			}
			*tempp = '\0';
			base64_decode(line, strlen(line), (unsigned char *)temp);
			if ((tempp = strchr(temp, '|')) == NULL) {
				set_eof(fd); 
				return 0; 
			}
			*tempp = '\0'; 
			password = tempp + 1;

			if(!chat_auth_ok(fd, name, password)){
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

enum chat_type {
	PKL_PUBLIC = 1,
	PKL_GLOBAL,
	PKL_PARTY,
	PKL_GUILD,
	PKL_WHISPER,
};

static void PrintMessage(char *msg, int type, bool send) {
	char channel[64];
	switch (type) {
	case PKL_PUBLIC:  safestrncpy(channel, "[喊话]", sizeof(channel)); break;
	case PKL_GLOBAL:  safestrncpy(channel, "[公开]", sizeof(channel)); break;
	case PKL_PARTY:   safestrncpy(channel, "[队伍]", sizeof(channel)); break;
	case PKL_GUILD:   safestrncpy(channel, "[公会]", sizeof(channel)); break;
	case PKL_WHISPER: safestrncpy(channel, "[密语]", sizeof(channel)); break;
	}
	if (send)
		ShowInfo("WEB接收->%s %s\n", channel, msg + 3);
	else
		ShowInfo("WEB发送->%s %s\n", channel, msg + 3);
}


static bool chat_mapif_isconnected();
static int inclif_parse_receive(int fd, struct chat_session_data *sd) {
	int packetlen;
	char decoded[2048], gbkcode[2048];
	unsigned char *buf = (unsigned char *)RFIFOCP(fd, 0);
	short type;
	
	if(!chat_mapif_isconnected())
		return 0;

	if ((packetlen = (int)recv_decode(fd, buf, decoded)) == 0)
		return 1;
	else
		RFIFOSKIP(fd, packetlen);
	if (!strcmp(decoded,"HBT")) { //心跳包
		sd->beattick = gettick();
		return 1;
	}else if (!strnicmp(decoded, "GL:", 3)) { //全服
		type = PKL_GLOBAL;
	}else if (!strnicmp(decoded, "PR:", 3)) { //队伍
		type = PKL_PARTY;
	}else if (!strnicmp(decoded, "GU:", 3)) { //公会
		type = PKL_GUILD;
	}else if (!strnicmp(decoded, "WS:", 3)) { //私聊
		type = PKL_WHISPER;
	}
	else
		return 1;

	utf82gbk(decoded + 3, gbkcode, 2048);
	
	switch (type)
	{
	case PKL_GLOBAL:
		sprintf(decoded, "%s : %s", sd->nickname, gbkcode);
		packetlen = strlen(decoded) + 6 + 10;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOW(map_fd, 4) = 2;
		strncpy(WFIFOCP(map_fd, 6), "0xFF0000", 9);
		strncpy(WFIFOCP(map_fd, 15), decoded, strlen(decoded) + 1);
		WFIFOSET(map_fd, packetlen);
		sprintf(gbkcode, "GL:%s", decoded);
		break;
	case PKL_PARTY:
		if (sd->party_id <= 0) {
			send_message(fd, "PR:WR:提示: 你没有队伍。。");
			return 1;
		}
		sprintf(decoded, "%s : %s", sd->nickname, gbkcode);
		packetlen = strlen(decoded) + 15;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOW(map_fd, 4) = 3;
		WFIFOL(map_fd, 6) = sd->account_id;
		WFIFOL(map_fd, 10) = sd->party_id;
		strncpy(WFIFOCP(map_fd, 14), decoded, strlen(decoded) + 1);
		WFIFOSET(map_fd, packetlen);
		sprintf(gbkcode, "PR:%s", decoded);
		break;
	case PKL_GUILD:
		if (sd->guild_id <= 0) {
			send_message(fd, "GU:WR:提示 : 你没有公会。。");
			return 1;
		}
		sprintf(decoded, "%s : %s", sd->nickname, gbkcode);
		packetlen = strlen(decoded) + 15;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOW(map_fd, 4) = 4;
		WFIFOL(map_fd, 6) = sd->account_id;
		WFIFOL(map_fd, 10) = sd->guild_id;
		strncpy(WFIFOCP(map_fd, 14), decoded, strlen(decoded) + 1);
		WFIFOSET(map_fd, packetlen);
		sprintf(gbkcode, "GU:%s", decoded);
		break;
	case PKL_WHISPER:
		buf = (unsigned char *)strstr(gbkcode, "|#|");
		*buf = '\0'; buf = buf + 3;
		//gbkcode 为 nickname
		//buf 为 mes
		strncpy(decoded, (char *)buf, strlen((char *)buf) + 1);
		packetlen = strlen(decoded) + 10 + NAME_LENGTH * 2 + 1;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOW(map_fd, 4) = 5;
		WFIFOL(map_fd, 6) = sd->account_id;
		strncpy(WFIFOCP(map_fd, 10), gbkcode, strlen(gbkcode) + 1);
		strncpy(WFIFOCP(map_fd, 10 + NAME_LENGTH), sd->nickname, NAME_LENGTH);
		strncpy(WFIFOCP(map_fd, 10 + NAME_LENGTH * 2), decoded, strlen(decoded) + 1);
		sprintf(gbkcode, "WS:%s : %s", sd->nickname, decoded);
		WFIFOSET(map_fd, packetlen);
		break;
	}
	//send_message(fd, gbkcode);
	PrintMessage(gbkcode, type, true);
	return 1;
}

static int inclif_parse_close(int fd, struct chat_session_data *sd) {
	set_eof(fd);
	return 1;
}

int chatclif_parse(int fd) {
	struct chat_session_data* sd = (struct chat_session_data*)session[fd]->session_data;
	uint32 ipl = session[fd]->client_addr;

	// disconnect any player if no login-server.
	//if (map_fd < 0)
	//	set_eof(fd);

	if (session[fd]->flag.eof) {
		if (sd != NULL) {
			ShowStatus("Character '" CL_WHITE "%s" CL_RESET "' logged off.\n", sd->nickname);
			uidb_remove(online_chat_db, sd->account_id);
		}
		do_close(fd);
		return 0;
	}
	if (sd && sd->skiplen > 0) {
		if (sd->skiplen > session[fd]->rdata_size) {
			RFIFOSKIP(fd, session[fd]->rdata_size);
			sd->skiplen -= session[fd]->rdata_size;
		}
		else {
			RFIFOSKIP(fd, (int)sd->skiplen);
			sd->skiplen = 0;
		}
	}
	while (RFIFOREST(fd) >= 2) {
		int next = 1;
		unsigned short cmd;

		cmd = RFIFOB(fd, 0);
		switch (cmd) {
		
		case 'g': case 'G': next = inclif_parse_websocket(fd); break;
		case 129: next = inclif_parse_receive(fd, sd); break;
		case 136: next = inclif_parse_close(fd, sd); break;
		
		// unknown packet received
		default:
			ShowError("parse_char: Received unknown packet " CL_WHITE "0x%x" CL_RESET " from ip '" CL_WHITE "%s" CL_RESET "'! Disconnecting!\n", RFIFOW(fd, 0), ip2str(ipl, NULL));
			set_eof(fd);
			return 0;
		}
		if (next == 0) {
			return 0;
		}
	}

	RFIFOFLUSH(fd);
	return 0;
}
//type 0: chat server login
//type 1: player login
//type 2: player logout
//type 3: 世界发言包
//type 4: 队伍发言包
//type 5: 公会发言包
//type 6: 私聊发言包


static int chat_mapif_parse(int fd);
static int chat_check_connect_mapserver(int tid, t_tick tick, int id, intptr_t data) {

	if (map_fd > 0 && session[map_fd] != NULL) {
		WFIFOHEAD(map_fd, 6);// sends a ping packet to login server (will receive pong 0x2718)
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = 6;
		WFIFOW(map_fd, 4) = 1;
		WFIFOSET(map_fd, 6);
		return 0;
	}

	ShowInfo("Attempt to connect to map-server...\n");
	
	if ((naddr_ != 0) && (!chatserv_config.map_ip)) {
		char ip_str[16];
		ip2str(addr_[0], ip_str);
		safestrncpy(chatserv_config.map_ip_str, ip_str, sizeof(chatserv_config.map_ip_str));
		chatserv_config.map_ip = str2ip(chatserv_config.map_ip_str);
	}
	
	map_fd = make_connection(chatserv_config.map_ip, chatserv_config.map_port, false, 10);
	if (map_fd == -1)
	{	//Try again later. [Skotlex]
		map_fd = 0;
		return 0;
	}
	session[map_fd]->func_parse = chat_mapif_parse;
	session[map_fd]->flag.server = 1;
	realloc_fifo(map_fd, FIFOSIZE_SERVERLINK, FIFOSIZE_SERVERLINK);

	WFIFOHEAD(map_fd, 54);
	WFIFOW(map_fd, 0) = 0x9F0F;
	WFIFOW(map_fd, 2) = 54;
	WFIFOW(map_fd, 4) = 0;
	memcpy(WFIFOP(map_fd, 6), chatserv_config.userid, 24);
	memcpy(WFIFOP(map_fd, 30), chatserv_config.passwd, 24);
	WFIFOSET(map_fd, 54);

	return 1;
}

static bool chat_mapif_isconnected() {
	return (map_fd > 0 && session[map_fd] && !session[map_fd]->flag.eof);
}

static int chat_mapif_keepalive(int fd) {
	RFIFOSKIP(fd, 2);
	session[fd]->flag.ping = 0;
	return 1;
}
static int chat_mapif_result(int fd) {
	int type = RFIFOW(fd, 2);
	switch (type) {
	case 0:
		ShowStatus("Chat-server connection refused.\n");
		RFIFOSKIP(fd, 4);
		exit(EXIT_FAILURE);
		break;
	case 1:
		ShowStatus("Chat-server Connected to Map-Server.\n\n");
		break;
	}
	RFIFOSKIP(fd, 4);
	
	return 1;
}
//----------------------------------------------------------

static int chat_mapif_GlobalMessage(int fd) {
	DBIterator* iter;
	struct online_chat_data *od;
	char message[1024];
	sprintf(message, "GL:%s", RFIFOCP(fd, 4));
	iter = db_iterator(online_chat_db);
	for (od = (struct online_chat_data *)dbi_first(iter); dbi_exists(iter); od = (struct online_chat_data *)dbi_next(iter)) {
		send_message(od->fd, message);
	}
	dbi_destroy(iter);
	RFIFOSKIP(fd, RFIFOW(fd, 2));
	PrintMessage(message, PKL_PUBLIC, false);
	return 1;
}

static int chat_mapif_PartyMessage(int fd) {
	DBIterator* iter;
	struct online_chat_data *od;
	char message[1024];
	int party_id;
	party_id = RFIFOL(fd, 4);
	sprintf(message, "PR:%s", RFIFOCP(fd, 8));
	iter = db_iterator(online_chat_db);
	for (od = (struct online_chat_data *)dbi_first(iter); dbi_exists(iter); od = (struct online_chat_data *)dbi_next(iter)) {
		if (od->party_id == party_id)
			send_message(od->fd, message);
	}
	dbi_destroy(iter);
	RFIFOSKIP(fd, RFIFOW(fd, 2));
	PrintMessage(message, PKL_PARTY, false);
	return 1;
}

static int chat_mapif_GuildMessage(int fd) {
	DBIterator* iter;
	struct online_chat_data *od;
	char message[1024];
	int guild_id;
	guild_id = RFIFOL(fd, 4);
	sprintf(message, "GU:%s", RFIFOCP(fd, 8));
	iter = db_iterator(online_chat_db);
	for (od = (struct online_chat_data *)dbi_first(iter); dbi_exists(iter); od = (struct online_chat_data *)dbi_next(iter)) {
		if (od->guild_id == guild_id)
			send_message(od->fd, message);
	}
	dbi_destroy(iter);
	RFIFOSKIP(fd, RFIFOW(fd, 2));
	PrintMessage(message, PKL_GUILD, false);
	return 1;
}

static int chat_mapif_WisMessage(int fd) {
	DBIterator* iter;
	struct online_chat_data *od;
	char message[1024], *name;
	memcpy(message, RFIFOCP(fd, 0), RFIFOW(fd, 2));
	name = RFIFOCP(fd, 4);
	sprintf(message, "WS:%s", RFIFOCP(fd, 4 + NAME_LENGTH));
	iter = db_iterator(online_chat_db);
	for (od = (struct online_chat_data *)dbi_first(iter); dbi_exists(iter); od = (struct online_chat_data *)dbi_next(iter)) {
		if (!strcmp(name, od->nickname))
			send_message(od->fd, message);
	}
	dbi_destroy(iter);
	RFIFOSKIP(fd, RFIFOW(fd, 2));
	PrintMessage(message, PKL_WHISPER, false);
	return 1;
}

static int chat_mapif_error(int fd, uint16 cmd) {
	struct online_chat_data *od = (struct online_chat_data *)uidb_get(online_chat_db, RFIFOL(fd, 2));

	if (od) {
		switch (cmd) {
		case 0xFC33:
			send_message(od->fd, "PR:WR:提示: 你没有队伍或队伍中没有玩家。");
			break;
		case 0xFC34:
			send_message(od->fd, "GU:WR:提示: 你没有公会或公会中没有玩家。");
			break;
		case 0xFC35:
			send_message(od->fd, "WS:WR:提示: 对方不在线。");
			break;
		}
	}
	RFIFOSKIP(fd, 6);
	return 1;
}

static int chat_mapif_parse(int fd) {
	
	if (fd != map_fd) {
		ShowDebug("parse_fromlogin: Disconnecting invalid session #%d (is not the login-server)\n", fd);
		do_close(fd);
		return 0;
	}

	if (session[fd]->flag.eof) {
		do_close(fd);
		map_fd = -1;
		ShowWarning("Connection to Map Server lost.\n\n");
		return 0;
	}	else if (session[fd]->flag.ping) {/* we've reached stall time */
		if (DIFF_TICK(last_tick, session[fd]->rdata_tick) > (stall_time * 5)) {/* we can't wait any longer */
			set_eof(fd);
			return 0;
		}
		else if (session[fd]->flag.ping != 2) { /* we haven't sent ping out yet */
			WFIFOHEAD(fd, 6);// sends a ping packet to login server (will receive pong 0x2718)
			WFIFOW(fd, 0) = 0x9F0F;
			WFIFOW(fd, 2) = 6;
			WFIFOW(fd, 4) = 1;
			WFIFOSET(fd, 6);
			session[fd]->flag.ping = 2;
		}
	}
	uint16 cjie = RFIFOW(fd, 0);
	while (RFIFOREST(fd) >= 2) {
		// -1: Login server is not connected
		//  0: Avoid processing followup packets (prev was probably incomplete) packet
		//  1: Continue parsing
		int next = 1;
		uint16 command = RFIFOW(fd, 0);
		switch (command) {
		//case 0x2711: next = chlogif_parse_ackconnect(fd, sd); break;
		case 0xFC30:  next = chat_mapif_result(fd); break;
		case 0xFC31:  next = chat_mapif_keepalive(fd); break;
		case 0xFC41:  next = chat_mapif_GlobalMessage(fd); break;
		case 0xFC42:  next = chat_mapif_PartyMessage(fd); break;
		case 0xFC43:  next = chat_mapif_GuildMessage(fd); break;
		case 0xFC44:  next = chat_mapif_WisMessage(fd); break;
		case 0xFC33:  next = chat_mapif_error(fd, command); break;
		case 0xFC34:  next = chat_mapif_error(fd, command); break;
		case 0xFC35:  next = chat_mapif_error(fd, command); break;
		default:
			RFIFOSKIP(fd, session[fd]->rdata_size);
			next = 1;
			//ShowError("Unknown packet 0x%04x received from login-server, disconnecting.\n", command);
			//set_eof(fd);
			//return 0;
		}
		if (next == 0)
			return 0;
	}

	RFIFOFLUSH(fd);
	return 0;
}

int do_init(int argc, char** argv)
{

	const char* CHAT_CONF_NAME = "conf/chat_athena.conf";
	const char* SQL_CONF_NAME = "conf/inter_athena.conf";

	chatserv_config.map_ip = 0;
	chatserv_config.map_port = 5121;
	chatserv_config.chat_ip = 0;
	chatserv_config.bind_ip = INADDR_ANY;
	chatserv_config.chat_port = 8080;
	safestrncpy(chatserv_config.userid, "s1", sizeof(chatserv_config.userid));
	safestrncpy(chatserv_config.passwd, "p1", sizeof(chatserv_config.passwd));

	chat_config_read(CHAT_CONF_NAME, false);
	chat_config_read(SQL_CONF_NAME, true);
	online_chat_db = uidb_alloc(DB_OPT_RELEASE_DATA);

	sql_handle = Sql_Malloc();
	if (SQL_ERROR == Sql_Connect(sql_handle, chat_server_id, chat_server_pw, chat_server_ip, (uint16)chat_server_port, chat_server_db))
	{
		ShowError("Couldn't connect with username = '%s', password = '%s', host = '%s', port = '%d', database = '%s'\n",
			chat_server_id, chat_server_pw, chat_server_ip, chat_server_port, chat_server_db);
		Sql_ShowDebug(sql_handle);
		Sql_Free(sql_handle);
		exit(EXIT_FAILURE);
	}

	if (*default_codepage) {
		if (SQL_ERROR == Sql_SetEncoding(sql_handle, default_codepage))
			Sql_ShowDebug(sql_handle);
	}

	if ((chat_fd = make_listen_bind(chatserv_config.bind_ip, chatserv_config.chat_port)) == -1) {
		ShowFatalError("Failed to bind to port '" CL_WHITE "%d" CL_RESET "'\n", chatserv_config.chat_port);
		exit(EXIT_FAILURE);
	}
	set_defaultparse(chatclif_parse);

	add_timer_func_list(chat_check_connect_mapserver, "chat_check_connect_mapserver");
	add_timer_interval(gettick() + 15000, chat_check_connect_mapserver, 0, 0, 20 * 1000);

	ShowStatus("Connect map-server after " CL_GREEN " 15 seconds" CL_RESET ".\n", chatserv_config.chat_port);
	ShowStatus("The chat-server is " CL_GREEN "ready" CL_RESET " (Server is listening on the port %d).\n", chatserv_config.chat_port);
	
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
	do_close(map_fd);
	do_close(chat_fd);
}
