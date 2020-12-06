// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//
#include <stdlib.h>

#include "../common/core.hpp"
#include "../common/malloc.hpp"
#include "../common/showmsg.hpp"
#include "../common/sql.hpp"

#include "websocket.hpp"
#include "chat.hpp"

struct ChatServ_Config chatserv_config;

#define CHAT_SIZE_MAX 255
int map_fd = -1; //login file descriptor
int chat_fd = -1; //char file descriptor

DBMap* online_chat_db;
Sql* sql_handle = NULL;

int chat_server_port = 3306;
char chat_server_ip[32] = "127.0.0.1";
char chat_server_id[32] = "ragnarok";
char chat_server_pw[32] = ""; // Allow user to send empty password (bugreport:7787)
char chat_server_db[32] = "ragnarok";
char default_codepage[32] = ""; //Feature by irmin.
char channel[32] = "world";

bool chat_config_read(const char* cfgName, bool issql) {
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
	}

	fclose(fp);

	ShowInfo("Done reading %s.\n", cfgName);
	return true;
}

static bool chat_auth_ok(int fd, const char *nickname, const char *password) {
	char ip[16];
	char* data;
	struct chat_session_data *sd;
	uint32 char_id, account_id, party_id, guild_id;
	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `char_id`,`account_id`,`party_id`,`guild_id` FROM `char` WHERE `name` = '%s' LIMIT 1", nickname) || SQL_SUCCESS != Sql_NextRow(sql_handle)) {
		ShowInfo("chat_auth_ok: Nickname '%s' does not exist.\n", nickname);
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

	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `group_id`,`state`,`unban_time`,`expiration_time` FROM `login` WHERE `account_id` = %d AND `user_pass` = '%s' LIMIT 1", account_id, password) || SQL_SUCCESS != Sql_NextRow(sql_handle)) {
		ShowInfo("chat_auth_ok: The password for the nickname(%s) is incorrect.\n", nickname);
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
	uidb_put(online_chat_db, account_id, sd);
	
	Sql_GetData(sql_handle, 0, &data, NULL); sd->group_id = (unsigned int)atoi(data);
	Sql_GetData(sql_handle, 1, &data, NULL); sd->state = (unsigned int)strtoul(data, NULL, 10);
	Sql_GetData(sql_handle, 2, &data, NULL); sd->unban_time = atol(data);
	Sql_GetData(sql_handle, 3, &data, NULL); sd->expiration_time = atol(data);
	Sql_FreeResult(sql_handle);

	if (sd->expiration_time != 0 && sd->expiration_time < time(NULL) ||
		sd->unban_time != 0 && sd->unban_time > time(NULL) ||
		sd->state != 0) {
		aFree(session[fd]->session_data);
		session[fd]->session_data = NULL;
		return false;
	}

	ip2str(session[fd]->client_addr, ip);
	ShowStatus("Authentication accepted (account: %s, id: %d, ip: %s)\n", nickname , account_id, ip);

	WFIFOHEAD(map_fd, 15 + NAME_LENGTH);
	WFIFOW(map_fd, 0) = 0x9F0F;
	WFIFOW(map_fd, 2) = 15 + NAME_LENGTH;
	WFIFOB(map_fd, 4) = PKL_LOGIN;
	WFIFOL(map_fd, 5) = sd->account_id;
	WFIFOL(map_fd, 9) = sd->char_id;
	WFIFOW(map_fd, 13) = chatserv_config.console_atcommand ? sd->group_id : 0;
	memcpy(WFIFOCP(map_fd, 15), sd->nickname, NAME_LENGTH);
	WFIFOSET(map_fd, 15 + NAME_LENGTH);

	return true;
}

static void inclif_PrintMessage(enum e_chat_type type, char *msg)
{
	switch (type) {
	case PKL_GLOBAL:
		ShowInfo("%s\n", msg + 3);
		break;
	case PKL_CONSOLE:
		break;
	default:
		ShowInfo("%s %s\n", websocket_msg_txt(type - PKL_GLOBAL), msg + 3);
		break;
	}
}

static int inclif_SendMessage(enum e_chat_type type, int group_id, char *to_name, char *out_message)
{
	DBIterator* iter;
	struct chat_session_data *tsd;
	int result = 0;

	iter = db_iterator(online_chat_db);
	for (tsd = (struct chat_session_data *)dbi_first(iter); dbi_exists(iter); tsd = (struct chat_session_data *)dbi_next(iter))
	{
		if (tsd->group_id >= chatserv_config.listen_message) {
			websocket_send_str(tsd->fd, out_message);
			continue;
		}
		switch (type) {
		case PKL_GLOBAL:
			result += websocket_send_str(tsd->fd, out_message);
			break;
		case PKL_PARTY:
			if (tsd->party_id == group_id)
				result += websocket_send_str(tsd->fd, out_message);
			break;
		case PKL_GUILD:
			if (tsd->guild_id == group_id)
				result += websocket_send_str(tsd->fd, out_message);
			break;
		case PKL_WHISPER:
			if (!strcmp(tsd->nickname, to_name))
				result += websocket_send_str(tsd->fd, out_message);
			break;
		case PKL_CONSOLE:
			if (tsd->group_id >= chatserv_config.listen_console)
				result += websocket_send_str(tsd->fd, out_message);
			break;
		}
	}
	dbi_destroy(iter);
	inclif_PrintMessage(type, out_message);
	return result;
}

int chat_split_auth (int fd, const char *line)
{
	char temp[1024], temp_gbk[1024], name[128], password[128];
	base64_decode(line, strlen(line), (unsigned char *)temp);
	utf82gbk(temp, temp_gbk, 1024);
	if (sscanf(temp, "%127[^|]|%127[^\r\n]", name, password) != 2)
		return 0;
	if (!chat_auth_ok(fd, name, password))
		return 0;

	return 1;
}

static bool chat_mapif_isconnected();
static int inclif_parse_receive(int fd, struct chat_session_data *sd)
{
	int packetlen, group_id = -1;
	char decoded[2048], gbkcode[2048], nickname[NAME_LENGTH] = {};
	unsigned char *buf = (unsigned char *)RFIFOCP(fd, 0);
	enum e_chat_type type;

	if (sd == NULL)
		return 0;

	if ((packetlen = websocket_decode(fd, buf, decoded, 2048)) == 0)
		return 1;
	else
		RFIFOSKIP(fd, packetlen);

	if (!strcmp(decoded,"HBT")) { //心跳包
		t_tick tick = gettick();
		if (sd->beattick + 10000 > tick) {
			safesnprintf(gbkcode, CHAT_SIZE_MAX, "WR:%s", websocket_msg_txt(10));
			websocket_send_str(fd, gbkcode);
			set_eof(fd);
			return 1;
		}
		sd->beattick = gettick();
		return 1;
	}else if (!strnicmp(decoded, "GL:", 3)) {
		type = PKL_GLOBAL;
	}else if (!strnicmp(decoded, "PR:", 3)) {
		type = PKL_PARTY;
		group_id = sd->party_id;
	}else if (!strnicmp(decoded, "GU:", 3)) {
		type = PKL_GUILD;
		group_id = sd->guild_id;
	}else if (!strnicmp(decoded, "WS:", 3)) {
		type = PKL_WHISPER;
	}else
		return 1;

	if (!utf82gbk(decoded + 3, gbkcode, CHAT_SIZE_MAX)) {
		char message[CHAT_SIZE_MAX];
		sprintf(message, websocket_msg_txt(21), CHAT_SIZE_MAX);
		websocket_send_str(fd, message);
		return 1;
	}

	char message[CHAT_SIZE_MAX];
	switch (type){
	case PKL_GLOBAL:
		if (sd->group_id < chatserv_config.global_min_group) {
			sprintf(message, websocket_msg_txt(22), chatserv_config.global_min_group);
			websocket_send_str(fd, message);
			return 1;
		}
		if (sd->global_tick + chatserv_config.global_deley > gettick()) {
			sprintf(message, websocket_msg_txt(20), chatserv_config.global_deley / 1000);
			websocket_send_str(fd, message);
			return 1;
		}
		sd->global_tick = gettick();
		safesnprintf(decoded, 2048, "%s %s : %s %s", channel, sd->nickname, chatserv_config.web_client_prefix, gbkcode);
		packetlen = 14 + strlen(decoded) + 1;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOB(map_fd, 4) = PKL_GLOBAL;
		safestrncpy(WFIFOCP(map_fd, 5), chatserv_config.global_color, 9);
		safestrncpy(WFIFOCP(map_fd, 14), decoded, strlen(decoded) + 1);
		WFIFOSET(map_fd, packetlen);
		safesnprintf(gbkcode, 2048, "GL:%s", decoded);
		break;
	case PKL_PARTY:
		if (sd->party_id <= 0) {
			sprintf(gbkcode, "PR:WR:%s", websocket_msg_txt(11));
			websocket_send_str(fd, gbkcode);
			return 1;
		}
		sprintf(decoded, "%s : %s %s", sd->nickname, chatserv_config.web_client_prefix, gbkcode);
		packetlen = 13 + strlen(decoded) + 1;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOB(map_fd, 4) = PKL_PARTY;
		WFIFOL(map_fd, 5) = sd->account_id;
		WFIFOL(map_fd, 9) = sd->party_id;
		safestrncpy(WFIFOCP(map_fd, 13), decoded, strlen(decoded) + 1);
		WFIFOSET(map_fd, packetlen);
		group_id = sd->party_id;
		sprintf(gbkcode, "PR:%s", decoded);
		break;
	case PKL_GUILD:
		if (sd->guild_id <= 0) {
			sprintf(gbkcode, "GU:WR:%s", websocket_msg_txt(12));
			websocket_send_str(fd, gbkcode);
			return 1;
		}
		sprintf(decoded, "%s : %s %s", sd->nickname, chatserv_config.web_client_prefix, gbkcode);
		packetlen = 13 + strlen(decoded) + 1;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOB(map_fd, 4) = PKL_GUILD;
		WFIFOL(map_fd, 5) = sd->account_id;
		WFIFOL(map_fd, 9) = sd->guild_id;
		safestrncpy(WFIFOCP(map_fd, 13), decoded, strlen(decoded) + 1);
		WFIFOSET(map_fd, packetlen);
		sprintf(gbkcode, "GU:%s", decoded);
		break;
	case PKL_WHISPER:
		buf = (unsigned char *)strstr(gbkcode, "|#|");
		*buf = '\0'; buf = buf + 3;
		//gbkcode - nickname
		//buf - mes
		safestrncpy(nickname, gbkcode, NAME_LENGTH);
		safesnprintf(decoded, CHAT_MAX_MSG,"%s %s", chatserv_config.web_client_prefix, (char *)buf);
		packetlen = 9 + NAME_LENGTH * 2 + strlen(decoded) + 1;
		WFIFOHEAD(map_fd, packetlen);
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = packetlen;
		WFIFOB(map_fd, 4) = PKL_WHISPER;
		WFIFOL(map_fd, 5) = sd->account_id;
		safestrncpy(WFIFOCP(map_fd, 9), nickname, NAME_LENGTH);
		safestrncpy(WFIFOCP(map_fd, 9 + NAME_LENGTH), sd->nickname, NAME_LENGTH);
		safestrncpy(WFIFOCP(map_fd, 9 + NAME_LENGTH * 2), decoded, strlen(decoded) + 1);
		WFIFOSET(map_fd, packetlen);
		sprintf(decoded, "WS:%s -> %s : %s %s", sd->nickname, nickname, chatserv_config.web_client_prefix, (char *)buf);
		safestrncpy(gbkcode, decoded, strlen(decoded) + 1);
		websocket_send_str(fd, gbkcode);
		break;
	}
	sd->web_count = inclif_SendMessage(type, group_id, nickname, gbkcode);
	return 1;
}

static int inclif_parse_close(int fd, struct chat_session_data *sd) {
	set_eof(fd);
	return 1;
}

int chatclif_parse(int fd) {
	struct chat_session_data* sd = (struct chat_session_data*)session[fd]->session_data;
	uint32 ipl = session[fd]->client_addr;

	if (session[fd]->flag.eof) {
		if (sd != NULL) {
			if (chat_mapif_isconnected()) {
				WFIFOHEAD(map_fd, 9 + NAME_LENGTH);
				WFIFOW(map_fd, 0) = 0x9F0F;
				WFIFOW(map_fd, 2) = 9 + NAME_LENGTH;
				WFIFOB(map_fd, 4) = PKL_LOGOUT;
				WFIFOL(map_fd, 5) = sd->account_id;
				WFIFOSET(map_fd, 9 + NAME_LENGTH);
			}
			ShowStatus("Character '" CL_WHITE "%s" CL_RESET "' logged off.\n", sd->nickname);
			uidb_remove(online_chat_db, sd->account_id);
		}
		do_close(fd);
		return 0;
	}
	if (!chat_mapif_isconnected()) {
		set_eof(fd);
		return 0;
	}
	if (RFIFOREST(fd) > 2048) {
		ShowWarning("chat_mapif_parse: Received packet too long packet_len (%d), disconnecting session #%d.\n", RFIFOREST(fd), fd);
		set_eof(fd);
		return 0;
	}
	while (RFIFOREST(fd) >= 2) {
		int next = 1;
		unsigned short cmd;

		cmd = RFIFOB(fd, 0);
		switch (cmd)
		{
		case 'G': next = websocket_websocket_check(fd, chat_split_auth); break;
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

static int chat_mapif_parse(int fd);
static int chat_check_connect_mapserver(int tid, t_tick tick, int id, intptr_t data) {

	if (map_fd > 0 && session[map_fd] != NULL) {
		WFIFOHEAD(map_fd, 5);// sends a ping packet to login server (will receive pong 0x2718)
		WFIFOW(map_fd, 0) = 0x9F0F;
		WFIFOW(map_fd, 2) = 5;
		WFIFOB(map_fd, 4) = PKL_PING;
		WFIFOSET(map_fd, 5);
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

	WFIFOHEAD(map_fd, 77);
	WFIFOW(map_fd, 0) = 0x9F0F;
	WFIFOW(map_fd, 2) = 77;
	WFIFOB(map_fd, 4) = PKL_NONE;
	memcpy(WFIFOP(map_fd, 5), chatserv_config.userid, NAME_LENGTH);
	memcpy(WFIFOP(map_fd, 29), chatserv_config.passwd, NAME_LENGTH);
	memcpy(WFIFOP(map_fd, 53), chatserv_config.global_world_clan, NAME_LENGTH);
	WFIFOSET(map_fd, 77);

	return 1;
}

static bool chat_mapif_isconnected() {
	return (map_fd > 0 && session[map_fd] && !session[map_fd]->flag.eof);
}

static int chat_mapif_keepalive(int fd) {
	session[fd]->flag.ping = 0;
	return 1;
}
static int chat_mapif_result(int fd)
{
	int type = RFIFOW(fd, 4);
	switch (type) {
	case 0:
		memset(channel, 0, NAME_LENGTH);
		ShowStatus("Chat-server Connected to Map-Server. without channel.\n");
		break;
	case 1:
		memcpy(channel, RFIFOP(fd, 6), NAME_LENGTH);
		ShowStatus("Chat-server Connected to Map-Server. World channel alias is '%s'.\n", channel);
		break;
	}
	return 1;
}
//----------------------------------------------------------

static int chat_mapif_message(int fd) {

	char message[CHAT_SIZE_MAX+NAME_LENGTH];
	char *in_name  = RFIFOCP(fd, 9);
	char *out_name = RFIFOCP(fd, 9 + NAME_LENGTH);
	char *out_message = RFIFOCP(fd, 9 + NAME_LENGTH*2);
	int group_id = RFIFOL(fd, 5);
	enum e_chat_type type = (enum e_chat_type)RFIFOB(fd, 4);

	switch (type) {
	case PKL_GLOBAL:
		safesnprintf(message, CHAT_SIZE_MAX+NAME_LENGTH, "PU:%s : %s", out_name, out_message);
		break;
	case PKL_PARTY:
		safesnprintf(message, CHAT_SIZE_MAX+NAME_LENGTH, "PR:%s : %s", out_name, out_message);
		break;
	case PKL_GUILD:
		safesnprintf(message, CHAT_SIZE_MAX+NAME_LENGTH, "GU:%s : %s", out_name, out_message);
		break;
	case PKL_WHISPER:
		if (strcmp(chatserv_config.global_world_clan, out_name))
			safesnprintf(message, CHAT_SIZE_MAX+NAME_LENGTH, "WS:%s -> %s : %s", in_name, out_name, out_message);
		else {
			type = PKL_GLOBAL;
			safesnprintf(message, CHAT_SIZE_MAX+NAME_LENGTH, "GL:%s %s : %s", channel, in_name, out_message);
		}
		break;
	case PKL_CONSOLE: {

	}
		break;
	default:
		safestrncpy(message, out_message, CHAT_SIZE_MAX+NAME_LENGTH);
		break;
	}

	inclif_SendMessage(type, group_id, out_name, message);
	return 1;
}

static int chat_mapif_info(int fd) {
	struct chat_session_data *sd = (struct chat_session_data *)uidb_get(online_chat_db, RFIFOL(fd, 4));
	enum e_chat_type type = (enum e_chat_type)RFIFOB(fd, 8);
	int count = RFIFOL(fd, 9);
	char message[1024];

	switch (type) {
	case PKL_NONE:
		ShowWarning("Connection failure. interface disable, please check 'conf/battle/BattleRa.conf -> chat_server_interface'..\n");
		return 1;
	case PKL_GLOBAL:
		return 1;
	case PKL_PARTY:
		sprintf(message, "PR:WR:%s", websocket_msg_txt(15));
		break;
	case PKL_GUILD:
		sprintf(message, "GU:WR:%s", websocket_msg_txt(16));
		break;
	case PKL_WHISPER:
		sprintf(message, "WS:WR:%s", websocket_msg_txt(17));
		break;
	}

	if (sd != NULL) {
		if (!sd->web_count && !count)
			websocket_send_str(sd->fd, message);
		sd->web_count = 0;
	}
	return 1;
}

static int chat_mapif_parse(int fd) {
	
	int packet_len;

	if (fd != map_fd) {
		ShowDebug("chat_mapif_parse: Disconnecting invalid session #%d (is not the login-server)\n", fd);
		do_close(fd);
		return 0;
	}

	if (session[fd]->flag.eof) {
		do_close(fd);
		map_fd = -1;
		ShowWarning("Connection to Map Server lost.\n\n");
		return 0;
	}
	else if (session[fd]->flag.ping) {/* we've reached stall time */
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

	if (RFIFOREST(fd) < 4)
		return 0;

	uint16 cmd = RFIFOW(fd, 0);
	packet_len = RFIFOW(fd, 2);

	if (packet_len < 4 || packet_len > 32768) {
		ShowWarning("chat_mapif_parse: Received packet 0x%04x specifies invalid packet_len (%d), disconnecting session #%d.\n", RFIFOW(fd, 0), packet_len, fd);
		set_eof(fd);
		return 0;
	}

	while ((int)RFIFOREST(fd) >= packet_len) {
		int next = 1;
		switch (cmd) {
		case 0xFC30:  next = chat_mapif_result(fd); break;
		case 0xFC31:  next = chat_mapif_keepalive(fd); break;
		case 0xFC32:  next = chat_mapif_message(fd); break;
		case 0xFC33:  next = chat_mapif_info(fd); break;
		default:
			ShowError("chat_mapif_parse: Unknown packet 0x%04x received from login-server, skiping packet.\n", cmd);
			set_eof(fd);
			return 0;
		}
		if (next == 0)
			return 0;

		RFIFOSKIP(fd, packet_len);
		return 0;
	}

	return 0;
}

void do_init_chat(void)
{
	const char* SQL_CONF_NAME = CONF_PATH "/inter_athena.conf";

	if (!chatserv_config.enable)
		return;

	chat_config_read(SQL_CONF_NAME, true);
	online_chat_db = uidb_alloc(DB_OPT_BASE);

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
	add_timer_interval(gettick() + 5000, chat_check_connect_mapserver, 0, 0, 5000);

	ShowStatus("Connect map-server after " CL_GREEN " 15 seconds" CL_RESET ".\n", chatserv_config.chat_port);
	ShowStatus("The chat-server is " CL_GREEN "ready" CL_RESET " (Server is listening on the port %d).\n", chatserv_config.chat_port);
}

void do_final_chat(void)
{
	do_close(map_fd);
	do_close(chat_fd);
}
