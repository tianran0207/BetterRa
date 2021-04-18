// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "core.hpp"

#include "../config/core.hpp"

#ifndef MINICORE
#include "ers.hpp"
#include "socket.hpp"
#include "timer.hpp"
#include "sql.hpp"
#endif
#include <stdlib.h>
#include <signal.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include "winapi.hpp" // Console close event handling
#include <direct.h> // _chdir
#endif

#include "cbasetypes.hpp"
#include "malloc.hpp"
#include "mmo.hpp"
#include "showmsg.hpp"
#include "strlib.hpp"
#include "des.hpp"

#ifndef DEPRECATED_COMPILER_SUPPORT
	#if defined( _MSC_VER ) && _MSC_VER < 1900
		#error "Visual Studio versions older than Visual Studio 2015 are not officially supported anymore"
	#elif defined( __clang__ ) && __clang_major__ < 4 && !( __clang_major__ == 3 && __clang_minor__ >= 7 )
		#error "clang versions older than clang 3.7 are not officially supported anymore"
	#elif !defined( __clang__ ) && defined( __GNUC__ ) && __GNUC__ < 5
		#error "GCC versions older than GCC 5 are not officially supported anymore"
	#endif
#endif

/// Called when a terminate signal is received.
void (*shutdown_callback)(void) = NULL;

#if defined(BUILDBOT)
	int buildbotflag = 0;
#endif

int runflag = CORE_ST_RUN;
char db_path[12] = "db"; /// relative path for db from server
char db_path_pre[64] = "db/import/csv_version";
char conf_path[12] = "conf"; /// relative path for conf from server

char *SERVER_NAME = NULL;
char SERVER_TYPE = ATHENA_SERVER_NONE;

#ifndef MINICORE	// minimalist Core
// Added by Gabuzomeu
//
// This is an implementation of signal() using sigaction() for portability.
// (sigaction() is POSIX; signal() is not.)  Taken from Stevens' _Advanced
// Programming in the UNIX Environment_.
//
#ifdef WIN32	// windows don't have SIGPIPE
#define SIGPIPE SIGINT
#endif

#ifndef POSIX
#define compat_signal(signo, func) signal(signo, func)
#else
sigfunc *compat_signal(int signo, sigfunc *func) {
	struct sigaction sact, oact;

	sact.sa_handler = func;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
#ifdef SA_INTERRUPT
	sact.sa_flags |= SA_INTERRUPT;	/* SunOS */
#endif

	if (sigaction(signo, &sact, &oact) < 0)
		return (SIG_ERR);

	return (oact.sa_handler);
}
#endif

/*======================================
 *	CORE : Console events for Windows
 *--------------------------------------*/
#ifdef _WIN32
static BOOL WINAPI console_handler(DWORD c_event) {
    switch(c_event) {
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
		if( shutdown_callback != NULL )
			shutdown_callback();
		else
			runflag = CORE_ST_STOP;// auto-shutdown
        break;
	default:
		return FALSE;
    }
    return TRUE;
}

static void cevents_init() {
	if (SetConsoleCtrlHandler(console_handler,TRUE)==FALSE)
		ShowWarning ("Unable to install the console handler!\n");
}
#endif

/*======================================
 *	CORE : Signal Sub Function
 *--------------------------------------*/
static void sig_proc(int sn) {
	static int is_called = 0;

	switch (sn) {
	case SIGINT:
	case SIGTERM:
		if (++is_called > 3)
			exit(EXIT_SUCCESS);
		if( shutdown_callback != NULL )
			shutdown_callback();
		else
			runflag = CORE_ST_STOP;// auto-shutdown
		break;
	case SIGSEGV:
	case SIGFPE:
		do_abort();
		// Pass the signal to the system's default handler
		compat_signal(sn, SIG_DFL);
		raise(sn);
		break;
#ifndef _WIN32
	case SIGXFSZ:
		// ignore and allow it to set errno to EFBIG
		ShowWarning ("Max file size reached!\n");
		//run_flag = 0;	// should we quit?
		break;
	case SIGPIPE:
		//ShowInfo ("Broken pipe found... closing socket\n");	// set to eof in socket.cpp
		break;	// does nothing here
#endif
	}
}

void signals_init (void) {
	compat_signal(SIGTERM, sig_proc);
	compat_signal(SIGINT, sig_proc);
#ifndef _DEBUG // need unhandled exceptions to debug on Windows
	//compat_signal(SIGSEGV, sig_proc);
	//compat_signal(SIGFPE, sig_proc);
#endif
#ifndef _WIN32
	compat_signal(SIGILL, SIG_DFL);
	compat_signal(SIGXFSZ, sig_proc);
	compat_signal(SIGPIPE, sig_proc);
	compat_signal(SIGBUS, SIG_DFL);
	compat_signal(SIGTRAP, SIG_DFL);
#endif
}
#endif

const char* get_svn_revision(void) {
#ifdef SVNVERSION
	return EXPAND_AND_QUOTE(SVNVERSION);
#else// not SVNVERSION
	static char svn_version_buffer[16] = "";
	FILE *fp;

	if( svn_version_buffer[0] != '\0' )
		return svn_version_buffer;

	// subversion 1.7 uses a sqlite3 database
	// FIXME this is hackish at best...
	// - ignores database file structure
	// - assumes the data in NODES.dav_cache column ends with "!svn/ver/<revision>/<path>)"
	// - since it's a cache column, the data might not even exist
	if( (fp = fopen(".svn" PATHSEP_STR "wc.db", "rb")) != NULL || (fp = fopen(".." PATHSEP_STR ".svn" PATHSEP_STR "wc.db", "rb")) != NULL )
	{
	#ifndef SVNNODEPATH
		//not sure how to handle branches, so i'll leave this overridable define until a better solution comes up
		#define SVNNODEPATH trunk
	#endif
		const char* prefix = "!svn/ver/";
		const char* postfix = "/" EXPAND_AND_QUOTE(SVNNODEPATH) ")"; // there should exist only 1 entry like this
		size_t prefix_len = strlen(prefix);
		size_t postfix_len = strlen(postfix);
		size_t i,j,len;
		char* buffer;

		// read file to buffer
		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		buffer = (char*)aMalloc(len + 1);
		fseek(fp, 0, SEEK_SET);
		len = fread(buffer, 1, len, fp);
		buffer[len] = '\0';
		fclose(fp);

		// parse buffer
		for( i = prefix_len + 1; i + postfix_len <= len; ++i ) {
			if( buffer[i] != postfix[0] || memcmp(buffer + i, postfix, postfix_len) != 0 )
				continue; // postfix missmatch
			for( j = i; j > 0; --j ) {// skip digits
				if( !ISDIGIT(buffer[j - 1]) )
					break;
			}
			if( memcmp(buffer + j - prefix_len, prefix, prefix_len) != 0 )
				continue; // prefix missmatch
			// done
			snprintf(svn_version_buffer, sizeof(svn_version_buffer), "%d", atoi(buffer + j));
			break;
		}
		aFree(buffer);

		if( svn_version_buffer[0] != '\0' )
			return svn_version_buffer;
	}

	// subversion 1.6 and older?
	if ((fp = fopen(".svn/entries", "r")) != NULL)
	{
		char line[1024];
		int rev;
		// Check the version
		if (fgets(line, sizeof(line), fp))
		{
			if(!ISDIGIT(line[0]))
			{
				// XML File format
				while (fgets(line,sizeof(line),fp))
					if (strstr(line,"revision=")) break;
				if (sscanf(line," %*[^\"]\"%11d%*[^\n]", &rev) == 1) {
					snprintf(svn_version_buffer, sizeof(svn_version_buffer), "%d", rev);
				}
			}
			else
			{
				// Bin File format
				if ( fgets(line, sizeof(line), fp) == NULL ) { printf("Can't get bin name\n"); } // Get the name
				if ( fgets(line, sizeof(line), fp) == NULL ) { printf("Can't get entries kind\n"); } // Get the entries kind
				if(fgets(line, sizeof(line), fp)) // Get the rev numver
				{
					snprintf(svn_version_buffer, sizeof(svn_version_buffer), "%d", atoi(line));
				}
			}
		}
		fclose(fp);

		if( svn_version_buffer[0] != '\0' )
			return svn_version_buffer;
	}

	// fallback
	svn_version_buffer[0] = UNKNOWN_VERSION;
	return svn_version_buffer;
#endif
}

// Grabs the hash from the last time the user updated their working copy (last pull)
const char *get_git_hash (void) {
	static char GitHash[41] = ""; //Sha(40) + 1
	FILE *fp;

	if( GitHash[0] != '\0' )
		return GitHash;

	if( (fp = fopen(".git/refs/remotes/origin/master", "r")) != NULL || // Already pulled once
		(fp = fopen(".git/refs/heads/master", "r")) != NULL ) { // Cloned only
		char line[64];
		char *rev = (char*)malloc(sizeof(char) * 50);

		if( fgets(line, sizeof(line), fp) && sscanf(line, "%40s", rev) )
			snprintf(GitHash, sizeof(GitHash), "%s", rev);

		free(rev);
		fclose(fp);
	} else {
		GitHash[0] = UNKNOWN_VERSION;
	}

	if ( !(*GitHash) ) {
		GitHash[0] = UNKNOWN_VERSION;
	}

	return GitHash;
}

/*======================================
 *	CORE : Display title
 *  ASCII By CalciumKid 1/12/2011
 *--------------------------------------*/
static void display_title(void) {
	const char* svn = get_svn_revision();
	const char* git = get_git_hash();

	ShowMessage("\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "                                                                 " CL_PASS"" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "       " CL_BT_WHITE "            rAthena Development Team presents                  " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "                 ___   __  __                                    " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "           _____/   | / /_/ /_  ___  ____  ____ _                " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "          / ___/ /| |/ __/ __ \\/ _ \\/ __ \\/ __ `/                " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "         / /  / ___ / /_/ / / /  __/ / / / /_/ /                 " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "        /_/  /_/  |_\\__/_/ /_/\\___/_/ /_/\\__,_/                  " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "                                                                 " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "       " CL_GREEN "              http://rathena.org/board/                        " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	//ShowMessage("" CL_PASS "     " CL_BOLD "                                                                 " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "                                                                　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "                 BetterRA Development Team presents             　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "              ____       __  __            ____  ___            　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "             / __ )___  / /_/ /____  _____/ __ \\/   |          　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "            / __  / _ \\/ __/ __/ _ \\/ ___/ /_/ / /| |         　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "           / /_/ /  __/ /_/ /_/  __/ /  / _, _/ ___ |           　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "          /_____/\\___/\\__/\\__/\\___/_/  /_/ |_/_/  |_|       　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "                                                                　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "          Do not use(it) for commercial/business purpose        　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "                                                                　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "            http://betterra.cn     QQ group: 419290524          　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("" CL_BG_CYAN CL_BT_WHITE "                                                                　" CL_BG_CYAN CL_CLL CL_NORMAL "\n");
	ShowMessage("\n");

#ifdef C_GITHASH
	ShowInfo("Git Hash: '" CL_WHITE "%s" CL_RESET "'\n", C_GITHASH);
	return;
#endif

	if( svn[0] != UNKNOWN_VERSION )
		ShowInfo("SVN Revision: '" CL_WHITE "%s" CL_RESET "'\n", svn);
	else if( git[0] != UNKNOWN_VERSION )
		ShowInfo("Git Hash: '" CL_WHITE "%s" CL_RESET "'\n", git);
}

// Warning if executed as superuser (root)
void usercheck(void)
{
#if !defined(BUILDBOT)
#ifdef _WIN32
	if (IsCurrentUserLocalAdministrator()) {
		ShowInfo("You are running rAthena with admin privileges, it is not necessary.\n");
	}
#else
	if (geteuid() == 0) {
		ShowInfo("You are running rAthena with root privileges, it is not necessary.\n");
	}
#endif
#endif
}

#ifdef _WIN32
#include <DbgHelp.h>
int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers, PWCHAR pwAppName)
{
	BOOL bOwnDumpFile = FALSE;
	HANDLE hDumpFile = hFile;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);

	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
	if (hDbgHelp && (pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump")))	{
		if (hDumpFile == NULL || hDumpFile == INVALID_HANDLE_VALUE)	{
			TCHAR szPath[MAX_PATH] = { 0 };
			TCHAR szFileName[MAX_PATH] = { 0 };
			SYSTEMTIME stLocalTime;
			GetLocalTime(&stLocalTime);
			GetCurrentDirectory(MAX_PATH, szPath);
			sprintf(szFileName, "%s//%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", szPath,
				stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
				stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
				GetCurrentProcessId(), GetCurrentThreadId());
			hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
			bOwnDumpFile = TRUE;
			OutputDebugString(szFileName);
		}
		if (hDumpFile != INVALID_HANDLE_VALUE) {
			ExpParam.ThreadId = GetCurrentThreadId();
			ExpParam.ExceptionPointers = pExceptionPointers;
			ExpParam.ClientPointers = FALSE;
			pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
				hDumpFile, MiniDumpNormal, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);
			if (bOwnDumpFile)
				CloseHandle(hDumpFile);
		}
	}
	if (hDbgHelp != NULL)
		FreeLibrary(hDbgHelp);
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())
		return EXCEPTION_CONTINUE_SEARCH;
	#ifndef MINICORE
		do_abort();
	#endif
	return GenerateMiniDump(NULL, lpExceptionInfo, NULL);
}
#endif

#ifdef _WIN32
#include <limits.h>
#include <intrin.h>
typedef unsigned __int32  uint32_t;
#else
#include <stdint.h>
#endif
class CPUID {
	uint32_t regs[4];
public:
	explicit CPUID(unsigned i) {
#ifdef _WIN32
		__cpuid((int *)regs, (int)i);
#else
		asm volatile
			("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
				: "a" (i), "c" (0));
		// ECX is set to zero for CPUID function 4
#endif
	}
	const uint32_t &EAX() const { return regs[0]; }
	const uint32_t &EBX() const { return regs[1]; }
	const uint32_t &ECX() const { return regs[2]; }
	const uint32_t &EDX() const { return regs[3]; }
};

#include "md5calc.hpp"
void server_machine_code(char *code, int len) {
	CPUID cpuID(1);
	char UID[32+49+1], MD5[32+1];
	sprintf(UID, "%08X%08X", cpuID.EDX(), cpuID.EAX());
	for (int i = 0; i < 3; i++) {
		CPUID cpuBrand(0x80000002 + i);
		memcpy(UID + i * 16 + 16, &cpuBrand.EAX(), 16);
	}
	MD5_String(UID, MD5);
	safestrncpy(code, MD5, len);
}

void server_info(char *path) {
	char signature[24];
	ShowInfo("" CL_BOLD "BetterRA Customized Information: " CL_NORMAL "(date: %s)\n", C_CUSTOMIZEDDATE);
	ShowMessage("          - development version: " CL_BOLD "%s" CL_NORMAL "\n", C_BETTERRA_BETA ? "yes" : "no");
	ShowMessage("          - packet ver: " CL_BOLD "%d" CL_NORMAL "\n", C_PACKETVER);
#ifdef C_SOCKET_ENCRYPT
	ShowMessage("          - packet encrypt: " CL_BOLD "%s" CL_NORMAL "\n", "yes");
#else
	ShowMessage("          - packet encrypt: " CL_BOLD "%s" CL_NORMAL "\n", "no");
#endif
	if (path && path[0] != '\0' &&strstr(path, "map-server.exe")) {
		server_machine_code(signature, 24);
		ShowMessage("" CL_BOLD "        Map-server Customized: " CL_NORMAL "(code: %s)\n", signature);
		if (strcmp(C_USEREMAIL, "admin@mail.betterra.cn"))
			ShowMessage("          - Customized email: " CL_BOLD "%s" CL_NORMAL "\n", C_USEREMAIL);
		if (C_PACKET_OBFUSCATION != 0)     ShowMessage("          - packet obfuscation: " CL_BOLD "%s" CL_NORMAL "\n", C_PACKET_OBFUSCATION ? "yes" : "no");
		if (C_PRERE != 0)                  ShowMessage("          - pre-re ver: " CL_BOLD "%s" CL_NORMAL "\n", C_PRERE ? "yes" : "no");
		if (C_VIP_ENABLE != 0)             ShowMessage("          - vip enable: " CL_BOLD "%s" CL_NORMAL "\n", C_VIP_ENABLE ? "yes" : "no");
		if (C_SHOW_SERVER_STATS != 0)      ShowMessage("          - show server stats: " CL_BOLD "%s" CL_NORMAL "\n", C_SHOW_SERVER_STATS ? "yes" : "no");
		if (C_MAX_ZENY != 1000000000)      ShowMessage("          - max zeny: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_ZENY);
		if (C_MAX_INVENTORY != 100)        ShowMessage("          - max inventory: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_INVENTORY);
		if (C_MIN_STORAGE != 300)          ShowMessage("          - min storage: " CL_BOLD "%d" CL_NORMAL "\n", C_MIN_STORAGE);
		if (C_DEFAULT_WALK_SPEED != 150)   ShowMessage("          - default walk speed: " CL_BOLD "%d" CL_NORMAL "\n", C_DEFAULT_WALK_SPEED);
		if (C_MAX_PARTY != 12)             ShowMessage("          - max party member: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_PARTY);
		if (MAX_GUILD_STORAGE != 600)      ShowMessage("          - max guild storage: " CL_BOLD "%d" CL_NORMAL "\n", MAX_GUILD_STORAGE);
		if (C_MIN_GUILD != 16)             ShowMessage("          - initial guild member: " CL_BOLD "%d" CL_NORMAL "\n", C_MIN_GUILD);
		if (C_GUILD_SKILL_NUMBER_ADD != 6) ShowMessage("          - guild skill add member: " CL_BOLD "%d" CL_NORMAL "\n", C_GUILD_SKILL_NUMBER_ADD);
		if (C_MAX_PETLOOT_SIZE != 30)      ShowMessage("          - max pet loot quantity: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_PETLOOT_SIZE);
		if (C_MAX_PET_DB != 600)           ShowMessage("          - max pet db: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_PET_DB);
		if (C_MAX_MOB_DB != 3999)          ShowMessage("          - max mob db: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_MOB_DB);
		if (C_MAX_NPC_CLASS2_END != 0)     ShowMessage("          - max npc class: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_NPC_CLASS2_END);
		if (C_MAX_MERCENARY_CLASS != 61)   ShowMessage("          - max mercenary class: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_MERCENARY_CLASS);
		if (C_MAX_FAME_LIST != 10)         ShowMessage("          - max fame list: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_FAME_LIST);
		if (C_MAX_REFINE != 0)             ShowMessage("          - max refine: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_REFINE);
		if (C_MAX_EVENTQUEUE != 200)       ShowMessage("          - max event queue: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_EVENTQUEUE);
		if (C_MAX_HATEFFECT != 0)          ShowMessage("          - max hateffect: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_HATEFFECT);
		if (C_MAX_RC2 != 0)                ShowMessage("          - max race2: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_RC2);
		if (C_MAX_SKILLUNITTEMP != 20)     ShowMessage("          - max skill unit temp: " CL_BOLD "%d" CL_NORMAL "\n", C_MAX_SKILLUNITTEMP);
	}
	ShowMessage("\n");
}

/*======================================
 *	CORE : MAINROUTINE
 *--------------------------------------*/
int main (int argc, char **argv)
{
	{// initialize program arguments
		char *p1;
		if((p1 = strrchr(argv[0], '/')) != NULL ||  (p1 = strrchr(argv[0], '\\')) != NULL ){
			char *pwd = NULL; //path working directory
			int n=0;
			SERVER_NAME = ++p1;
			n = p1-argv[0]; //calc dir name len
			pwd = safestrncpy((char*)malloc(n + 1), argv[0], n);
			if(chdir(pwd) != 0)
				ShowError("Couldn't change working directory to %s for %s, runtime will probably fail",pwd,SERVER_NAME);
			free(pwd);
		}else{
			// On Windows the .bat files have the executeable names as parameters without any path seperator [Lemongrass]
			SERVER_NAME = argv[0];
		}
	}

#ifdef _WIN32
	SetUnhandledExceptionFilter(ExceptionFilter);
#endif

#ifdef MINICORE // minimalist Core
	malloc_init();// needed for Show* in display_title() [FlavioJS]
	display_title();
	usercheck();
	do_init(argc,argv);
	do_final();
#else// not MINICORE
	db_init();
	showmsg_init();
	malloc_init();// needed for Show* in display_title() [FlavioJS]
	set_server_type();
	display_title();
	server_info(argv[0]);
	usercheck();

	Sql_Init();
	signals_init();

#ifdef _WIN32
	cevents_init();
#endif

	timer_init();
	socket_init();

	rc4_init2();
	do_init(argc,argv);

	// Main runtime cycle
	while (runflag != CORE_ST_STOP) { 
		t_tick next = do_timer(gettick_nocache());
		do_sockets(next);
	}

	do_final();

	timer_final();
	socket_final();
	showmsg_final();
	db_final();
	ers_final();
#endif

	malloc_final();

#if defined(BUILDBOT)
	if( buildbotflag ){
		exit(EXIT_FAILURE);
	}
#endif

	return 0;
}
